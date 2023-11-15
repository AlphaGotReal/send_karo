#! /usr/bin/python3

import pyproj
import rospy
import json

from sensor_msgs.msg import NavSatFix
from std_msgs.msg import Float64MultiArray

file_ = open("/home/straw/Documents/Solo/src/cloudy/src/waypoints.json", "r")
data = json.load(file_)

got_easting_and_northing = False

def publish_goals():
	waypoints_en = Float64MultiArray()
	waypoints_en.data = []
	for r in data["waypoints"]:
		e, n = gps_to_utm(r["latitude"], r["longitude"])
		waypoints_en.data += [e, n]

	send_goal.publish(waypoints_en)

def gps_to_utm(latitude, longitude):
	wgs84 = pyproj.CRS('EPSG:4326') 
	utm = pyproj.CRS('EPSG:32633')

	transformer = pyproj.Transformer.from_crs(wgs84, utm, always_xy=True)

	utm_easting, utm_northing = transformer.transform(longitude, latitude)

	return [utm_easting, utm_northing]

def gps_callback(gps_fix):
	global got_easting_and_northing

	latitude = gps_fix.latitude
	longitude = gps_fix.longitude

	en = Float64MultiArray()
	en.data = gps_to_utm(latitude, longitude)
	send_en.publish(en)

	got_easting_and_northing = True

if __name__ == '__main__':
	rospy.init_node('gps_listener', anonymous=True)
	rospy.Subscriber('/gps', NavSatFix, gps_callback)
	send_en = rospy.Publisher("/easting_and_northing", Float64MultiArray, queue_size=100)
	send_goal = rospy.Publisher("/gps_waypoints", Float64MultiArray, queue_size=100)
	while not rospy.is_shutdown():
		if (not got_easting_and_northing):
			print(f"[midnight]: getting the gps coordinates")
			continue
		publish_goals()
