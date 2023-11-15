#include <iostream>
#include <time.h>
#include <cmath>
#include <cloudy/gum.hpp>

#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/Float32MultiArray.h>

#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/filters/extract_indices.h>

bool nomans = true;
int times = 0;

ros::Publisher planePub;

sensor_msgs::PointCloud2 thinCloud;
float threshold = 0.1;

plane ground;

bool planeReady = false;

void segment(sensor_msgs::PointCloud2 cloud_msg)
{

    if(!nomans || times < 10){
        times++;
        // Convert ROS PointCloud2 message to PCL PointCloud type
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
        pcl::fromROSMsg(cloud_msg, *cloud);
    
        // THE SEGMENTATION MODEL
        pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);
        pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
        // Create the segmentation object
        pcl::SACSegmentation<pcl::PointXYZ> seg;
        // Optional
        seg.setOptimizeCoefficients(true);
        // Mandatory
        seg.setModelType(pcl::SACMODEL_PLANE);
        seg.setMethodType(pcl::SAC_RANSAC);
        seg.setDistanceThreshold(threshold);
    
        seg.setInputCloud(cloud);
        seg.segment(*inliers, *coefficients);
    
        // Extract ground points
        pcl::PointCloud<pcl::PointXYZ>::Ptr ground_cloud(new pcl::PointCloud<pcl::PointXYZ>);
        pcl::ExtractIndices<pcl::PointXYZ> extract_ground;
        extract_ground.setInputCloud(cloud);
        extract_ground.setIndices(inliers);
        extract_ground.setNegative(false);  // Extract the inliers (ground points)
        extract_ground.filter(*ground_cloud);
    
    
        sensor_msgs::PointCloud2 planeCloud;    
        pcl::toROSMsg(*ground_cloud, planeCloud);
    
        if (!ground.updateCoeff(planeCloud)){
            std::cout << "[noon]: failed" << std::endl;
            return ;
        }
    
        planeReady = true;

    }

    std_msgs::Float32MultiArray groundPlane;
    groundPlane.data.push_back(ground.a);
    groundPlane.data.push_back(ground.b);
    groundPlane.data.push_back(ground.c);
    groundPlane.data.push_back(ground.d);
    groundPlane.data.push_back(threshold);
    planePub.publish(groundPlane);
    // Extract non-ground points (objects)
    //pcl::PointCloud<pcl::PointXYZ>::Ptr objects_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    //extract_ground.setNegative(true);  // Extract the outliers (non-ground points)
    //extract_ground.filter(*objects_cloud);

    // Publish segmented ground plane
    /*sensor_msgs::PointCloud2 ground_msg;
    pcl::toROSMsg(*ground_cloud, ground_msg);
    ground_msg.header = cloud_msg.header;
    ground_pub.publish(ground_msg);*/

    // Publish objects point cloud
    //sensor_msgs::PointCloud2 objects_msg;
    //pcl::toROSMsg(*objects_cloud, objects_msg);
    //objects_msg.header = cloud_msg.header;
    //objects_pub.publish(objects_msg);
}

bool gotCloud = false;

//callbacks
void getCloud(const sensor_msgs::PointCloud2::ConstPtr &C){
    int step = 5;
	union{
	    float val;
	    unsigned char bytes[4];
	}P;

	sensor_msgs::PointCloud2 cloud;

	int total = C->height * C->width;

	cloud.height = 1;
	cloud.header = C->header;
	cloud.fields = C->fields;
	cloud.is_bigendian = C->is_bigendian;
	cloud.point_step = C->point_step;
	cloud.row_step = C->row_step;
	cloud.is_dense = C->is_dense;

	int i = 0;
	for (int t = 0; t < total; t += step){
            P.bytes[0] = C->data[t*32 + 8];
            P.bytes[1] = C->data[t*32 + 9];
            P.bytes[2] = C->data[t*32 + 10];
            P.bytes[3] = C->data[t*32 + 11];
            
            if (P.val == 20.0){
            	continue;
            }for (int r = 0; r < 32; r++){
            	cloud.data.push_back(C->data[t*32 + r]);
            }i++;
	}cloud.width = i;
        thinCloud = cloud;
	gotCloud = true;
}

int main(int argc, char** argv){
    ros::init(argc, argv, "noon");
    ros::NodeHandle node;

    // Subscribe to the point cloud topic
    ros::Subscriber cloudSub = node.subscribe<sensor_msgs::PointCloud2>("/zed/depth/points", 1, getCloud);

    planePub = node.advertise<std_msgs::Float32MultiArray>("/ground", 10, true);

    ros::AsyncSpinner spinner(4);
    spinner.start();

    ros::Rate rate(30);

    while (ros::ok()){
    	if (!gotCloud){
            std::cout << "[noon]: " << gotCloud << std::endl;
            continue;
    	}
    	segment(thinCloud);
    	rate.sleep();
    }

    return 0;
}
