#include <iostream>
#include <cmath>
#include <vector>

#include <ros/ros.h>

#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/PointCloud.h>
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/UInt32MultiArray.h>

#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/MapMetaData.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Point32.h>
#include <geometry_msgs/Vector3.h>

#include <tf/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

ros::Publisher send_cost;
ros::Publisher send_goal;

tf2_ros::Buffer buffer;
std_msgs::UInt32MultiArray obstacle_indices;
sensor_msgs::PointCloud2 cloud;
geometry_msgs::Pose bot, zero;
nav_msgs::MapMetaData map_data;

bool got_obstacles = false;
bool got_points = false;
bool got_bot = false;
bool got_map = false;

void get_obstacles(const std_msgs::UInt32MultiArray::ConstPtr &ob){
	obstacle_indices = *ob;
	got_obstacles = true;
}

void get_points(const sensor_msgs::PointCloud2::ConstPtr &point2){
	cloud = *point2;
	got_points = true;
}

void get_map(const nav_msgs::OccupancyGrid::ConstPtr &map){
	map_data = map->info;
	got_map = true;
}

int main(int n, char **args){
	ros::init(n, args, "night");
	ros::NodeHandle node;

	zero.position.x = 0;
	zero.position.y = 0;
	zero.position.z = 0;

	zero.orientation.x = 0;
	zero.orientation.y = 0;
	zero.orientation.z = 0;
	zero.orientation.w = 1;

	ros::Subscriber obstacles_sub = node.subscribe("/obstacles", 100, get_obstacles);
	ros::Subscriber point_sub = node.subscribe("/zed/depth/points", 100, get_points);
	ros::Subscriber map_sub = node.subscribe("/map", 100, get_map);

	send_cost = node.advertise<sensor_msgs::PointCloud>("/cost", 100, true);
	send_goal = node.advertise<sensor_msgs::PointCloud>("/goal", 100, true);

	ros::AsyncSpinner spinner(4);
	spinner.start();

	tf2_ros::TransformListener listener(buffer);
	geometry_msgs::TransformStamped wrt_map;

	while (ros::ok()){
		try{
			wrt_map = buffer.lookupTransform(
				"map", "base_link",
				ros::Time(0), ros::Duration(0.4)
			);
		}catch (tf2::LookupException e){
			ROS_ERROR("%s", e.what());
			continue;
		}catch (tf2::ConnectivityException e){
			ROS_ERROR("%s", e.what());
			continue;
		}catch (tf2::ExtrapolationException e){
			ROS_ERROR("%s", e.what());
			continue;
		}

		tf2::doTransform(zero, bot, wrt_map);
		got_bot = true;

		if (!(got_obstacles && got_points && got_bot && got_map)){
			std::cout << "[night]: " << got_obstacles << got_points << got_bot << got_map << std::endl;
			continue;
		}
		

	}

	return 0;
}

