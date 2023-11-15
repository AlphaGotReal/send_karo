#include <iostream>
#include <vector>
#include <cmath>

#include <ros/ros.h>

#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/MapMetaData.h>

#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Bool.h>

#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Pose.h>

#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

ros::Publisher send_goal;

tf2_ros::Buffer buffer;

geometry_msgs::Pose bot;
geometry_msgs::Pose zero;

geometry_msgs::Pose spawn_pose;
std_msgs::Float64MultiArray spawn_easting_and_northing;
std_msgs::Float64MultiArray easting_and_northing;

bool got_spawn = false;
bool got_spawn_en = false;
bool got_en = false;

geometry_msgs::PoseStamped calculate_xy(double easting, double northing){
	float de = easting - spawn_easting_and_northing.data[0];
	float dn = northing - spawn_easting_and_northing.data[1];

	float x = spawn_pose.position.x - dn;
	float y = spawn_pose.position.y - de;

	geometry_msgs::PoseStamped pose;
	pose.header.frame_id = "map";

	pose.pose.position.x = x;
	pose.pose.position.y = y;
	pose.pose.position.z = 0;

	pose.pose.orientation = bot.orientation;
	return pose;
}

void get_en(const std_msgs::Float64MultiArray::ConstPtr &en){
	if (!got_spawn) return ;
	easting_and_northing = *en;
	got_en = true;
	if (!got_spawn_en){
		got_spawn_en = true;
		spawn_easting_and_northing = *en;
	}
}

void get_goal_en(const std_msgs::Float64MultiArray::ConstPtr &goal){

	if (!(got_spawn && got_spawn_en)) return ;

	nav_msgs::Path goals;
	goals.header.frame_id = "map";

	geometry_msgs::PoseStamped pose;

	for (int t = 0; t < 4; t++){
		pose = calculate_xy(goal->data[2*t], goal->data[2*t+1]);
		goals.poses.push_back(pose);
	}

	send_goal.publish(goals);
}

int main(int argc, char **argv){
	ros::init(argc, argv, "evening");
	ros::NodeHandle node;

	zero.position.x = 0;
	zero.position.y = 0;
	zero.position.z = 0;

	zero.orientation.x = 0;
	zero.orientation.y = 0;
	zero.orientation.z = 0;
	zero.orientation.w = 1;

	//ros::Subscriber map_sub = node.subscribe("/map", 100, get_map);
	ros::Subscriber en_sub = node.subscribe("/easting_and_northing", 100, get_en);
	ros::Subscriber goal_en_sub = node.subscribe("/gps_waypoints", 100, get_goal_en);

	send_goal = node.advertise<nav_msgs::Path>("/gps_goals", 100, true);

	tf2_ros::TransformListener listener(buffer);
	geometry_msgs::TransformStamped wrt_map;

	ros::AsyncSpinner spinner(4);
	spinner.start();

	bool first_time = true;

	while (ros::ok()){

		if (false){
			std::cout << "[evening]: " << std::endl;
			continue;
		}

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
		if (first_time){
			first_time = false;
			spawn_pose = bot;
			got_spawn = true;
		}
	}

	return 0;
}

