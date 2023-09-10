import rospy

from nav_msgs.msg import Path, OccupancyGrid as OG
from geometry_msgs.msg import Pose, Point, Quaternion, PoseStamped

import numpy as np
from math import *
import sys
import time

cmd_vels = []

with open("path.txt", 'r') as f:
	data = f.read().split('\n')
	for line in data:
		try:
			cmd_vels.append(tuple(map(float, line.split(' '))))
		except Exception as e:
			print(type(e), e)

path = Path()
path.header.frame_id = "map"
pose = PoseStamped()
pose.pose.position = Point(0, 0, 0)
pose.pose.orientation = Quaternion(0, 0, 0, 1)
path.poses.append(pose)

t = 0
for (v, w) in cmd_vels[::-1]:
	t += 1
	print(f"{t}/{len(cmd_vels)}", end='\r')
	pose = PoseStamped()
	pose.pose.position = Point(v, w, 0)
	pose.pose.orientation = Quaternion(0, 0, 0, 1)
	path.poses.append(pose)

print("\nDone!\n")

rospy.init_node("yo_mama")
pub = rospy.Publisher("/path", Path, queue_size=1)

while not rospy.is_shutdown():
	print(f"[{int(time.time())}]: publishing", end='\r')
	pub.publish(path)

print("\n")	



