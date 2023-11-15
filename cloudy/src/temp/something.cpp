#include <iostream>
#include <cmath>
#include <vector>

#include <ros/ros.h>

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Path.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <navfn/navfn_ros.h>

#include <tf/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

ros::Publisher send_path;

nav_msgs::OccupancyGrid map;
geometry_msgs::Pose bot;
geometry_msgs::Pose zeroPose;

bool got_map = false;
bool got_bot = false;

navfn::NavfnROS planner;

void get_goal(const geometry_msgs::PoseStamped::ConstPtr &goal){
	if (! (got_bot && got_map)) return ;

	geometry_msgs::PoseStamped start, end;

	start.header.frame_id = "map";
	start.pose = bot;

	end.header.frame_id = "map";
	end.pose = goal->pose;

	nav_msgs::Path path;
	path.header.frame_id = "map";

	bool success = planner.makePlan(start, end, path.poses);

	if (success){
		send_path.publish(path);
		std::cout << "[sky]: success......" << std::endl;
	}else{
		std::cout << "[sky]: lol it failed..." << std::endl;
	}
}

void get_map(const nav_msgs::OccupancyGrid::ConstPtr &msg){
	map = *msg;
	got_map = true;

}

int main(int n, char ** args){
	ros::init(n, args, "sky");
	ros::NodeHandle node;


	zeroPose.position.x = 0;
	zeroPose.position.y = 0;
	zeroPose.position.z = 0;

	zeroPose.orientation.x = 0;
	zeroPose.orientation.y = 0;
	zeroPose.orientation.z = 0;
	zeroPose.orientation.w = 1;

	ros::Subscriber map_sub = node.subscribe("/map", 100, get_map);
	ros::Subscriber goal_sub = node.subscribe("/move_base_simple/goal", 100, get_goal);

	send_path = node.advertise<nav_msgs::Path>("/path", 100, true);
	ros::Publisher send_pose = node.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose", 100, true);

	ros::AsyncSpinner spinner(4);
	spinner.start();

	tf2_ros::Buffer buffer;
	tf2_ros::TransformListener listener(buffer);
	costmap_2d::Costmap2DROS costmap("my_costmap", buffer);
	planner.initialize("map", &costmap);

	geometry_msgs::TransformStamped wrtmap;
	geometry_msgs::PoseWithCovarianceStamped initial_pose;

	initial_pose.header.frame_id = "map";

	while (ros::ok()){
		try{
			wrtmap = buffer.lookupTransform(
				"map",
				"base_link",
				ros::Time(0),
				ros::Duration(0.4)
			);
		}catch (tf2::LookupException e){
			ROS_ERROR("%s", e.what());
			continue ;
		}catch (tf2::ConnectivityException e){
			ROS_ERROR("%s", e.what());
			continue ;
		}catch (tf2::ExtrapolationException e){
			ROS_ERROR("%s", e.what());
			continue ;
		}tf2::doTransform(zeroPose, bot, wrtmap);
		initial_pose.pose.pose = bot;
		send_pose.publish(initial_pose);
		got_bot = true;
	}
	return 0;
}
