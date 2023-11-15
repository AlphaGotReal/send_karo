#include <iostream>
#include <cmath>
#include <vector>

#include <ros/ros.h>

#include <std_msgs/Bool.h>

#include <nav_msgs/Path.h>

#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

std_msgs::Bool nomans;
nav_msgs::Path gps_coor;

void get_gps(const nav_msgs::Path::ConstPtr &goals){
	gps_coor = *goals;
}

int main(int argc, char **argv){

	ros::init(argc, argv, "night");
	ros::NodeHandle node;

	ros::Subscriber gps_sub = node.subscribe("/gps_goals", 100, get_gps);

	nomans.data = false;

	ros::AsyncSpinner spinner(4);
	spinner.start();

	while (ros::ok()){
		
	}

	return 0;
}


