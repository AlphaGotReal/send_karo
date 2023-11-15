#include <iostream>
#include <vector>
#include <cmath>

#include <ros/ros.h>

#include <nav_msgs/OccupancyGrid.h>

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Point32.h>
#include <geometry_msgs/TransformStamped.h>

#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/UInt32MultiArray.h>
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/Float32MultiArray.h>

#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

ros::Publisher send_lines;
ros::Publisher send_points;
ros::Publisher send_goal;

tf2_ros::Buffer buffer;

sensor_msgs::PointCloud2 cloud;
std_msgs::Int16MultiArray mask;
nav_msgs::OccupancyGrid map;
std::vector<float> plane(5, 0);

geometry_msgs::PoseStamped goal;
geometry_msgs::Pose zero;
geometry_msgs::Pose bot;

int width;
int height;

float acceptance = 1.5;

bool got_mask = false;
bool got_cloud = false;
bool got_plane = false;
bool got_map = false;

bool is_obstacle(float x, float y, float z){
	float distance = abs(plane[0]*x + plane[1]*y + plane[2]*z + plane[3]);
	return distance >= plane[4]*(1 + acceptance);
}

void process(){

	union{
		float val;
		unsigned char bytes[4];
	}X, Y, Z;

	sensor_msgs::PointCloud safe;
	geometry_msgs::Point32 point;

	safe.header.frame_id = "map";

	geometry_msgs::TransformStamped zedToMap;
	try{
		zedToMap = buffer.lookupTransform(
			"map",
			"zed2i_depth",
			cloud.header.stamp,
			ros::Duration(0.4)
		);
	}catch (tf2::LookupException e){
		ROS_ERROR("%s", e.what());
		return ;
	}catch (tf2::ConnectivityException e){
		ROS_ERROR("%s", e.what());
		return ;
	}catch (tf2::ExtrapolationException e){
		ROS_ERROR("%s", e.what());
		return ;
	}

	int inc = 5;
	int jump = 5;

	bool sent_goal = false;

	geometry_msgs::Pose pose;

	int total = 0;

	for (int y = height/7; y < height; y+=jump){
		bool start = false;
		for (int x = 0; x < width; x+=inc){
			int n = x + y * width;

			X.bytes[0] = cloud.data[32*n];
			X.bytes[1] = cloud.data[32*n+1];
			X.bytes[2] = cloud.data[32*n+2];
			X.bytes[3] = cloud.data[32*n+3];

			Y.bytes[0] = cloud.data[32*n+4];
			Y.bytes[1] = cloud.data[32*n+5];
			Y.bytes[2] = cloud.data[32*n+6];
			Y.bytes[3] = cloud.data[32*n+7];

			Z.bytes[0] = cloud.data[32*n+8];
			Z.bytes[1] = cloud.data[32*n+9];
			Z.bytes[2] = cloud.data[32*n+10];
			Z.bytes[3] = cloud.data[32*n+11];

			if (Z.val >= 20.0) continue;

			if (is_obstacle(X.val, Y.val, Z.val)){
				if (!start) break;
				continue;
			}

			if (mask.data[n] != 0 && mask.data[n+inc] == 0){
				start = true;
				continue;
			}

			if (start){
				pose.position.x = X.val;
				pose.position.y = Y.val;
				pose.position.z = Z.val;
				tf2::doTransform(pose, pose, zedToMap);
				
				float dx = pose.position.x - bot.position.x;
				float dy = pose.position.y - bot.position.y;

				point.x = pose.position.x;
				point.y = pose.position.y;
				point.z = 0.5;

				int nx = (point.x - map.info.origin.position.x)/map.info.resolution;
				int ny = (point.y - map.info.origin.position.y)/map.info.resolution;

				if (map.data[nx + ny*map.info.width] != 0){
					start = false;
					break;
				}

				safe.points.push_back(point);
				if (mask.data[n+inc] != 0) break;
			}
		}
	}
	send_points.publish(safe);
	if (safe.points.size() < 5){
		std::cout << "[evening]: in no-man's land" << std::endl;
		return ;
	}
	std::cout << "[evening]: within the lanes" << std::endl;
}

bool reached(geometry_msgs::PoseStamped goal){
	float dx = goal.pose.position.x - bot.position.x;
	float dy = goal.pose.position.y - bot.position.y;
	float d = dx*dx + dy*dy;
	return d < 0.3*0.3;
}

void get_lane(const std_msgs::Int16MultiArray::ConstPtr &lane){
	mask = *lane;
	int n = lane->data.size();
	width = lane->data[n-2];
	height = lane->data[n-1];
	got_mask = true;
}

void get_plane(const std_msgs::Float32MultiArray::ConstPtr &p){
	plane[0] = p->data[0];
	plane[1] = p->data[1];
	plane[2] = p->data[2];
	plane[3] = p->data[3];
	plane[4] = p->data[4];
	got_plane = true;
}

void get_cloud(const sensor_msgs::PointCloud2::ConstPtr &c){
	cloud = *c;
	got_cloud = true;
}

void get_map(const nav_msgs::OccupancyGrid::ConstPtr &m){
	map = *m;
	got_map = true;
}

int main(int n, char **args){
	ros::init(n, args, "evening");
	ros::NodeHandle node;

	zero.position.x = 0;
	zero.position.y = 0;
	zero.position.z = 0;

	zero.orientation.x = 0;
	zero.orientation.y = 0;
	zero.orientation.z = 0;
	zero.orientation.w = 1;

	ros::Subscriber lane_sub = node.subscribe("/lane", 100, get_lane);
	ros::Subscriber plane_sub = node.subscribe("/plane", 100, get_plane);
	ros::Subscriber cloud_sub = node.subscribe("/zed/depth/points", 100, get_cloud);
	ros::Subscriber map_sub = node.subscribe("/move_base/global_costmap/costmap", 100, get_map);

	send_lines = node.advertise<std_msgs::Int16MultiArray>("/lines", 100, true);
	send_goal = node.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal", 100, true);
	send_points = node.advertise<sensor_msgs::PointCloud>("/potential_goal", 100, true);

	ros::AsyncSpinner spinner(4);
	spinner.start();

	goal.header.frame_id = "map";
	goal.pose.orientation.x = 0;
	goal.pose.orientation.y = 0;
	goal.pose.orientation.z = 0;
	goal.pose.orientation.w = 1;

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
	
		if (!(got_mask && got_plane && got_cloud && got_map)){
			std::cout << "[evening]: " << got_mask << got_plane << got_cloud << got_map << std::endl;
			continue;
		}
		process();
	}

	return 0;
}

