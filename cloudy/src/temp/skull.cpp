#include <iostream>
#include <vector>
#include <cmath>

#include <ros/ros.h>

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Point32.h>

#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/UInt32MultiArray.h>
#include <std_msgs/Int16MultiArray.h>

#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

ros::Publisher send_lines;
ros::Publisher send_obstalces;

std_msgs::Int16MultiArray mask;
bool got_mask = false;

void process(){
	int m = 0;
	std_msgs::Int16MultiArray line;
	std_msgs::UInt32MultiArray obstacles;
	std::vector<int> temp;
	int n = mask.data.size();
	for (int q = 0; q < n-2; q++){
		if ((q/mask.data[n-2])%5 != 0){
			line.data.push_back(0);
			continue;
		}if (mask.data[q] == 0 && mask.data[q+1] != 0 && false){
			line.data.push_back(255);
			obstacles.data.push_back(q);
		}else if (mask.data[q] != 0 && mask.data[q+1] == 0){
			line.data.push_back(255);
			obstacles.data.push_back(q);
		}else{
			line.data.push_back(0);
		}
	}
	
	std::cout << "[evening]: number of obstacles = " << obstacles.data.size() << std::endl;

	line.data.push_back(mask.data[n-1]);
	line.data.push_back(mask.data[n-2]);
	send_lines.publish(line);
	send_obstalces.publish(obstacles);
}

void get_lane(const std_msgs::Int16MultiArray::ConstPtr &lane){
	mask = *lane;
	got_mask = true;
}

int main(int n, char **args){
	ros::init(n, args, "evening");
	ros::NodeHandle node;

	ros::Subscriber lane_sub = node.subscribe("/lane", 100, get_lane);

	send_lines = node.advertise<std_msgs::Int16MultiArray>("/lines", 100, true);
	send_obstalces = node.advertise<std_msgs::UInt32MultiArray>("/obstacles", 100, true);

	ros::AsyncSpinner spinner(4);
	spinner.start();

	while (ros::ok()){
		if (!(got_mask)){
			std::cout << "[evening]: " << got_mask << std::endl;
			continue;
		}process();
	}

	return 0;
}

