import rospy

from nav_msgs.msg import Path, OccupancyGrid as OG
from geometry_msgs.msg import Pose, Point, Quaternion, PoseStamped

import numpy as np
from math import *
import sys

cmd_vels = []

with open("out.txt", 'r') as f:
	data = f.read().split('\n')
	for line in data:
		try:
			cmd_vels.append(tuple(map(float, line.split(' '))))
		except Exception as e:
			print(type(e), e)


x, y = 0, 0
theta = 0
dt = float(sys.argv[1])

path = Path()
path.header.frame_id = "map"

pose = PoseStamped()
pose.pose.position.x = x
pose.pose.position.y = y
pose.pose.position.z = 0
pose.pose.orientation = Quaternion(0, 0, 0, 1)

path.poses.append(pose)
t = 0
for (v, w) in cmd_vels:
	t += 1
	print(f"{t}/{len(cmd_vels)}", end='\r')
	x += v*dt*cos(theta)
	y += v*dt*sin(theta)
	theta += w*dt

	pose = PoseStamped()
	pose.pose.position.x = x
	pose.pose.position.y = y
	pose.pose.position.z = 0
	pose.pose.orientation = Quaternion(0, 0, sin(theta/2), cos(theta/2))
	path.poses.append(pose)

print("\nDone\n")

rospy.init_node("yo")
pub = rospy.Publisher("/trajectory", Path, queue_size=1)

while not rospy.is_shutdown():
	print("publishing", end='\r')
	pub.publish(path)

