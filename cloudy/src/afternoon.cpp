#include <iostream>
#include <vector>
#include <cmath>

#include <ros/ros.h>
#include <cloudy/gum.hpp>

#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/LaserScan.h>

#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Point32.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/TransformStamped.h>

#include <nav_msgs/OccupancyGrid.h>
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/Float32MultiArray.h>

#include <tf2_msgs/TFMessage.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

bool nomans = false;
double pitch = 0;

ros::Publisher map_pub;
ros::Publisher ramp_pub;

geometry_msgs::Pose bot;
geometry_msgs::Pose zeroPose;

geometry_msgs::TransformStamped zedToMap;

std_msgs::Int16MultiArray mask;
std_msgs::Int16MultiArray ramp_mask;
std::vector<float> ground(5, 0);
std::vector<float> ramp(5, 0);
sensor_msgs::PointCloud2 cloud;

nav_msgs::OccupancyGrid map;
nav_msgs::OccupancyGrid tempMap;
std::vector<long long int> obstacleCount;
std::vector<long long int> totalCount;

tf2_ros::Buffer buffer;
float ground_acceptance = 1.5;
float ramp_acceptance = 0.0;

float ramp_threshold = 0.1;

int width, height;

bool isObstacle(float x, float y, float z){
	float distance = abs(ground[0]*x + ground[1]*y + ground[2]*z + ground[3]);
	return distance >= ground[4]*(1 + ground_acceptance);
}

bool isRamp(float x, float y, float z){
	float distance = abs(ramp[0]*x + ramp[1]*y + ramp[2]*z + ramp[3]);
	return distance <= ramp[4]*(1 + ramp_acceptance);
}

void test_ramp(){
	sensor_msgs::PointCloud ramp_cloud;
	ramp_cloud.header = cloud.header;

	union{
		float val;
		unsigned char bytes[4];
	}X, Y, Z;

	geometry_msgs::Point32 point;

	for (int t = 0; t < cloud.width; t+=7){

		X.bytes[0] = cloud.data[t*32];
		X.bytes[1] = cloud.data[t*32+1];
		X.bytes[2] = cloud.data[t*32+2];
		X.bytes[3] = cloud.data[t*32+3];

		Y.bytes[0] = cloud.data[t*32+4];
		Y.bytes[1] = cloud.data[t*32+5];
		Y.bytes[2] = cloud.data[t*32+6];
		Y.bytes[3] = cloud.data[t*32+7];

		Z.bytes[0] = cloud.data[t*32+8];
		Z.bytes[1] = cloud.data[t*32+9];
		Z.bytes[2] = cloud.data[t*32+10];
		Z.bytes[3] = cloud.data[t*32+11];

		if (Z.val >= 20.0) continue;

		if (!isRamp(X.val, Y.val, Z.val)) continue;
		//if (!isObstacle(X.val, Y.val, Z.val)) continue;

		point.x = X.val;
		point.y = Y.val;
		point.z = Z.val;

		ramp_cloud.points.push_back(point);

	}ramp_pub.publish(ramp_cloud);
	std::cout << "[afternoon]: published ramp with n = " << ramp_cloud.points.size() << std::endl;
}

bool ramp_ready = false;

void update_map(){

	union{
		float val;
		unsigned char bytes[4];
	}X, Y, Z;

	int total = cloud.height * cloud.width;
	int nodex, nodey, n;

	geometry_msgs::Pose pose;
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
	for (int t = total/4; t < total; t+=7){

		X.bytes[0] = cloud.data[t*32];
		X.bytes[1] = cloud.data[t*32+1];
		X.bytes[2] = cloud.data[t*32+2];
		X.bytes[3] = cloud.data[t*32+3];

		Y.bytes[0] = cloud.data[t*32+4];
		Y.bytes[1] = cloud.data[t*32+5];
		Y.bytes[2] = cloud.data[t*32+6];
		Y.bytes[3] = cloud.data[t*32+7];

		Z.bytes[0] = cloud.data[t*32+8];
		Z.bytes[1] = cloud.data[t*32+9];
		Z.bytes[2] = cloud.data[t*32+10];
		Z.bytes[3] = cloud.data[t*32+11];

		if (Z.val >= 20.0){
			continue;
		}

		pose.position.x = X.val;
		pose.position.y = Y.val;
		pose.position.z = Z.val;

		tf2::doTransform(pose, pose, zedToMap);

		nodex = (pose.position.x - map.info.origin.position.x)/map.info.resolution;
		nodey = (pose.position.y - map.info.origin.position.y)/map.info.resolution;

		n = nodex + nodey * map.info.width;

		totalCount[n]++;
		if (mask.data[t] != 0 || ramp_mask.data[t] != 0){
			obstacleCount[n]++;
		}else if (bot.position.z < 0.34){
			if (isObstacle(X.val, Y.val, Z.val)){
				if (nomans){
					if (!isRamp(X.val, Y.val, Z.val)){
						obstacleCount[n]++;
					}
				}else{
					obstacleCount[n]++;
				}
			}
		}

		float probability = 100 * obstacleCount[n]/totalCount[n];
		if (probability < 30){
			map.data[n] = 0;
		}else{
			map.data[n] = 100;
		}
	}
}

//map info
float sidelen = 200; // in meters 
float resolution = 0.05;
int nodelen = sidelen/resolution; //in pixels
std::vector<int> dimension = {nodelen, nodelen};
std::vector<float> origin = {-sidelen/2, -sidelen/2};
//


bool cloud_ready = false;
bool mask_ready = false;
bool ramp_mask_ready = false;
bool ground_ready = false;

//callbacks
void getPc(const sensor_msgs::PointCloud2::ConstPtr &C){
	cloud = *C;
	cloud_ready = true;
}

void getBot(const tf2_msgs::TFMessage::ConstPtr &T){
	if (T->transforms[0].header.frame_id == "odom"){
		geometry_msgs::TransformStamped wrtOdom = T->transforms[0];
		tf2::doTransform(zeroPose, bot, wrtOdom);
	}
}

void getMask(const std_msgs::Int16MultiArray::ConstPtr &L){
	mask = *L;
	mask_ready = true;
	int size = mask.data.size();
	width = mask.data[size-2];
	height = mask.data[size-1];
}

void getRampMask(const std_msgs::Int16MultiArray::ConstPtr &R){
	ramp_mask = *R;
	ramp_mask_ready = true;
	if (!cloud_ready) return ;
	sensor_msgs::PointCloud ramp_lane;
	geometry_msgs::Point32 point;
	union{
		float val;	
		unsigned char bytes[4];
	}X, Y, Z;
	for (int t = 0; t < R->data.size(); t++){
		if (R->data[t] != 0){

			X.bytes[1] = cloud.data[t*32+1];
			X.bytes[2] = cloud.data[t*32+2];
			X.bytes[3] = cloud.data[t*32+3];

			Y.bytes[0] = cloud.data[t*32+4];
			Y.bytes[1] = cloud.data[t*32+5];
			Y.bytes[2] = cloud.data[t*32+6];
			Y.bytes[3] = cloud.data[t*32+7];

			Z.bytes[0] = cloud.data[t*32+8];
			Z.bytes[1] = cloud.data[t*32+9];
			Z.bytes[2] = cloud.data[t*32+10];
			Z.bytes[3] = cloud.data[t*32+11];

			point.x = X.val;
			point.y = Y.val;
			point.z = Z.val;

			ramp_lane.points.push_back(point);
		}
	}if (ramp_lane.points.size() < 10){
		std::cout << "[afternoon]: too less points" << std::endl;
	}

	plane ramp_plane;
	if (!ramp_plane.updateCoeff(ramp_lane)){
		std::cout << "[afternoon]: failed to get the ramp plane" << std::endl;
		return ;
	}
	ramp_ready = true;
	ramp[0] = ramp_plane.a;
	ramp[1] = ramp_plane.b;
	ramp[2] = ramp_plane.c;
	ramp[3] = ramp_plane.d;
	ramp[4] = ramp_threshold;
}

void getGround(const std_msgs::Float32MultiArray::ConstPtr &p){
	ground[0] = p->data[0];
	ground[1] = p->data[1];
	ground[2] = p->data[2];
	ground[3] = p->data[3];
	ground[4] = p->data[4];
	ground_ready = true;
}

int main(int n, char ** args){
	ros::init(n, args, "afternoon");
	ros::NodeHandle node;

	//////////init
	map.header.frame_id = "map";

	map.info.resolution = resolution;
	map.info.width = dimension[1];
	map.info.height = dimension[0];

	map.info.origin.position.x = origin[0];
	map.info.origin.position.y = origin[1];
	map.info.origin.position.z = 0;

	map.info.origin.orientation.x = 0;
	map.info.origin.orientation.y = 0;
	map.info.origin.orientation.z = 0;
	map.info.origin.orientation.w = 1;

	for (int t = 0; t < map.info.width * map.info.height; t++){
		map.data.push_back(-1);
	}

	zeroPose.position.x = 0;
	zeroPose.position.y = 0;
	zeroPose.position.z = 0;

	zeroPose.orientation.x = 0;
	zeroPose.orientation.y = 0;
	zeroPose.orientation.z = 0;
	zeroPose.orientation.w = 1;

	tf2_ros::TransformListener listener(buffer);
	obstacleCount = std::vector<long long int>(map.info.width*map.info.height, 0);
	totalCount = std::vector<long long int>(map.info.width*map.info.height, 0);
	///////////////////

	map_pub = node.advertise<nav_msgs::OccupancyGrid>("/map", 100);
	ramp_pub = node.advertise<sensor_msgs::PointCloud>("/test_ramp", 100);
	//test_pub = node.advertise<sensor_msgs::PointCloud2>("/ground", 100);
	//ros::Publisher goal_pub = node.advertise<geometry_msgs::Pose>("/goal", 100);

	ros::Subscriber pc_sub = node.subscribe("/zed/depth/points", 1, getPc);
	ros::Subscriber mask_sub = node.subscribe("/lane", 1, getMask);
	ros::Subscriber ramp_mask_sub = node.subscribe("/ramp_lane", 1, getRampMask);
	ros::Subscriber ground_sub = node.subscribe("/ground", 100, getGround); 

	ros::AsyncSpinner spinner(4);
	spinner.start();

	float x, y;

	geometry_msgs::TransformStamped wrt_map;
	geometry_msgs::Pose goal;

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

		tf2::doTransform(zeroPose, bot, wrt_map);
		pitch = asin(-2.0*(bot.orientation.x * bot.orientation.z - bot.orientation.w * bot.orientation.y)) * 180/ M_PI; 

		if (!(mask_ready && cloud_ready && ground_ready && ramp_mask_ready)){
			std::cout << "[afternoon]: " << mask_ready << cloud_ready << ground_ready << ramp_mask_ready << std::endl;
			continue ;
		}
		//test_ramp();
		update_map();
		map_pub.publish(map);
	}

	return 0;
}


