#! /usr/bin/python3

import rospy
import json

from math import cos, pi
import numpy as np

from sensor_msgs.msg import NavSatFix
from geometry_msgs.msg import PoseStamped
from nav_msgs.msg import Odometry

waypoints = json.load(open("waypoints.json", "r"))["waypoints"]

R = 6.3781e6 #radius of earth

def flatten(cor2, cor1, T):
	dy = (cor2.real - cor1.real) * R * pi/180
	r = R * cos((cor1 + cor2).real * pi / 360)
	dx = (cor2.imag - cor1.imag)*r*pi/180
	return dx + 1j*dy

class gps:
	def __init__(self):
		self.gps_sub = rospy.Subscriber("/gps", NavSatFix, self.get_gps)
		self.bot_sub = rospy.Subscriber("/odom", Odometry, self.get_bot)

		self.send_goal = rospy.Publisher("/move_base_simple/goal", PoseStamped, queue_size=100)
		self.point_number = 0

		self.T = None

		self.bot = None
		self.bot_gps = None
		self.r = None
		self.initial = None
		self.initial_gps = None

	def get_gps(self, x):
		cor2 = waypoints[self.point_number]["latitude"] + 1j*waypoints[self.point_number]["longitude"]
		cor1 = x.latitude + 1j*x.longitude
		self.bot_gps = cor1
		if (self.initial_gps is None):
			self.initial_gps = cor1
		self.r = flatten(cor2, cor1)
		if ((self.r * self.r.conjugate()).real and self.initial is not None):
			goal = PoseStamped()
			goal.header.frame_id = "map"
			goal.pose.position.x = self.initial.position.x + self.r.real
			goal.pose.position.y = self.initial.position.y + self.r.imag
			goal.pose.position.z = 0
			goal.pose.orientation.x = 0
			goal.pose.orientation.y = 0
			goal.pose.orientation.z = 0
			goal.pose.orientation.w = 1
			self.send_goal.publish(goal)

	def get_bot(self, x):
		if (self.initial is None):
			self.initial = x.pose.pose
		self.bot = x.pose.pose


rospy.init_node("midnight")

g = gps()

while not rospy.is_shutdown():
	if (g.bot is None or g.initial_gps is None):
		continue
	r = flatten(g.bot_gps, g.initial_gps)
	print(f"[midnight]: {r.real}, {r.imag}")


