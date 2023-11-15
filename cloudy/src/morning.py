import rospy
import time

from sensor_msgs.msg import Image, PointCloud2
from std_msgs.msg import Int16MultiArray as arr
from sensor_msgs.point_cloud2 import read_points as rp

import numpy as np
import matplotlib.pyplot as plt
import cv2
from cv_bridge import CvBridge

rgb = None
lane = None

noman = True

X = Y = 0

hsvranges = {
	"lane" : (np.array([0, 0, 231]), np.array([179, 35, 255])),
	"ramp lane": (np.array([30, 0, 52]), np.array([90, 255, 255]))
}

class morning:
	def __init__(self):

		self.br = CvBridge()

		self.rgbSub = rospy.Subscriber("/zed/rgb/image_raw", Image, self.getRgb)

	def getRgb(self, x):
		global rgb, lane
		rgb = np.uint8(self.br.imgmsg_to_cv2(x, desired_encoding="passthrough"))
		h, w = rgb.shape[:2]
		mask, ramp_mask = getLane(rgb)

		lane = arr()
		lane.data = mask.flatten().tolist() + [w, h]
		lane_pub.publish(lane)

		ramp_lane = arr()
		ramp_lane.data = ramp_mask.flatten().tolist() + [w, h]
		ramp_lane_pub.publish(ramp_lane)

		#cv2.waitKey(1)
		#cv2.imshow("mask", ramp_mask)


def getLane(rgb):

	hsv = cv2.cvtColor(rgb, cv2.COLOR_RGB2HSV)
	lower, upper = hsvranges["lane"]
	mask = cv2.inRange(hsv, lower, upper)
	if (noman):
		lower, upper = hsvranges["ramp lane"]
		ramp_lane_mask = cv2.inRange(hsv, lower, upper)
		ramp_lane_mask = cv2.erode(ramp_lane_mask, None, iterations=1)
		ramp_lane_mask = cv2.dilate(ramp_lane_mask, None, iterations=1)
		return mask, ramp_lane_mask
	return mask, np.zeros(mask.shape)


if __name__ == "__main__":
	rospy.init_node("morning")
	lane_pub = rospy.Publisher("/lane", arr, queue_size=1)
	#ramp_lane_pub = rospy.Publisher("/ramp_lane", arr, queue_size=1)
	morning()
	while not rospy.is_shutdown():
		if (rgb is None):
			print("[morning]: 0")


