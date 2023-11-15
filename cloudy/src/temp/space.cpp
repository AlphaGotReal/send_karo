#include <iostream>
#include <cmath>
#include <vector>

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/PointCloud.h>
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/Float32MultiArray.h>

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Point32.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TransformStamped.h>

#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Path.h>

#include <tf2_msgs/TFMessage.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

ros::Publisher line_pub;
ros::Publisher path_pub;

std_msgs::Int16MultiArray mask;
sensor_msgs::PointCloud2 cloud;

geometry_msgs::Pose zeroPose;
geometry_msgs::Pose bot;

std::vector<float> plane(5);
std::vector<float> height_ratios {0.2, 0.22, 0.24, 0.26};

float acceptance = 0.4;

int height, width;

bool is_obstacle(int n){
	if (mask.data[n] != 0){
		return true;
	}

	union{
		float val;
		unsigned char bytes[4];
	}X, Y, Z;

	X.bytes[0] = cloud.data[n*32];
	X.bytes[1] = cloud.data[n*32+1];
	X.bytes[2] = cloud.data[n*32+2];
	X.bytes[3] = cloud.data[n*32+3];

	Y.bytes[0] = cloud.data[n*32+4];
	Y.bytes[1] = cloud.data[n*32+5];
	Y.bytes[2] = cloud.data[n*32+6];
	Y.bytes[3] = cloud.data[n*32+7];

	Z.bytes[0] = cloud.data[n*32+8];
	Z.bytes[1] = cloud.data[n*32+9];
	Z.bytes[2] = cloud.data[n*32+10];
	Z.bytes[3] = cloud.data[n*32+11];

	float distance = abs(plane[0]*X.val + plane[1]*Y.val + plane[2]*Z.val + plane[3]);
	return (distance > plane[4]*(1 + acceptance));

}

void line(){

	std::vector<int> waypoints;

	int avg_x = 0;

	for (int r = 0; r < height_ratios.size(); r++){
		int h = height_ratios[r]*height;
		int max_len = 0;
		int point = -1;
		int start, mid;
		bool record = false;
		for (int t = 0; t < width; t++){
			if (!is_obstacle(h*width + t)){
				if (record){
					// lines.data[h*width + t] = 255;
					if (t == width-1) continue;
					if (is_obstacle(h*width + t + 1)){
						mid = (start + t)/2;
						if (t-start > max_len){
							max_len = t-start;
							point = mid;
						}
					}
				}
				continue;
			}
			start = t;
			record = true;
		}waypoints.push_back(point + h*width);
	}

	nav_msgs::Path path;
	geometry_msgs::PoseStamped pose;
	union{
		float val;
		unsigned char bytes[4];
	}X, Y, Z;
	for (int t = 0; t < waypoints.size(); t++){

		int n = waypoints[t];

		X.bytes[0] = cloud.data[n*32];
		X.bytes[1] = cloud.data[n*32+1];
		X.bytes[2] = cloud.data[n*32+2];
		X.bytes[3] = cloud.data[n*32+3];

		Y.bytes[0] = cloud.data[n*32+4];
		Y.bytes[1] = cloud.data[n*32+5];
		Y.bytes[2] = cloud.data[n*32+6];
		Y.bytes[3] = cloud.data[n*32+7];

		Z.bytes[0] = cloud.data[n*32+8];
		Z.bytes[1] = cloud.data[n*32+9];
		Z.bytes[2] = cloud.data[n*32+10];
		Z.bytes[3] = cloud.data[n*32+11];

	}

}

bool got_mask = false;
bool got_cloud = false;
bool got_plane = false;

void getMask(const std_msgs::Int16MultiArray::ConstPtr &M){
	mask = *M;
	int n = mask.data.size();
	height = mask.data[n-1];
	width = mask.data[n-2];
	got_mask = true;
}

void getCloud(const sensor_msgs::PointCloud2::ConstPtr &C){
	cloud = *C;
	got_cloud = true;
}

void getPlane(const std_msgs::Float32MultiArray::ConstPtr &P){
	plane[0] = P->data[0];
	plane[1] = P->data[1];
	plane[2] = P->data[2];
	plane[3] = P->data[3];
	plane[4] = P->data[4];
	got_plane = true;
}

int main(int n, char ** args){
	ros::init(n, args, "space");
	ros::NodeHandle node;

	zeroPose.position.x = 0;
	zeroPose.position.y = 0;
	zeroPose.position.z = 0;

	zeroPose.orientation.x = 0;
	zeroPose.orientation.y = 0;
	zeroPose.orientation.z = 0;
	zeroPose.orientation.w = 1;

	ros::Subscriber cloud_sub = node.subscribe("/zed/depth/points", 100, getCloud);
	ros::Subscriber mask_sub = node.subscribe("/lane", 100, getMask);
	ros::Subscriber plane_sub = node.subscribe("/plane", 100, getPlane);

	line_pub = node.advertise<std_msgs::Int16MultiArray>("/lines", 100, true);
	path_pub = node.advertise<nav_msgs::Path>("/via_points", 100, true);

	ros::AsyncSpinner spinner(4);
	spinner.start();

	while (ros::ok()){
		if (!(got_mask && got_cloud && got_plane)){
			std::cout << "[space]: " << got_mask << got_cloud << got_plane << std::endl;
			continue;
		}
		line();
	}

	return 0;
}


