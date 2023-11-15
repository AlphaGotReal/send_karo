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

bool noman = true;

ros::Publisher groundPub;
ros::Publisher rampPub;
ros::Publisher test;

sensor_msgs::PointCloud2 thinCloud;
float ground_threshold = 0.2;
float ramp_threshold = 0.1;

plane ground;
plane ramp;

long long int ground_count;
long long int ramp_count;

plane plane1;
plane plane2;

bool first_time = true;
bool groundReady = false;
bool rampReady = false;

bool check_plane(plane one, plane two){
	bool a = abs(one.a - two.a) <= 0.005;
	bool b = abs(one.b - two.b) <= 0.005;
	bool c = abs(one.c - two.c) <= 0.005;
	bool d = abs(one.d - two.d) <= 0.005;
	return a && b && c && d;
}

void segment(sensor_msgs::PointCloud2 cloud_msg)
{

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
    seg.setDistanceThreshold(ground_threshold);

    seg.setInputCloud(cloud);
    seg.segment(*inliers, *coefficients);

    // Extract ground points
    pcl::PointCloud<pcl::PointXYZ>::Ptr ground_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::ExtractIndices<pcl::PointXYZ> extract_ground;
    extract_ground.setInputCloud(cloud);
    extract_ground.setIndices(inliers);
    extract_ground.setNegative(false);  // Extract the inliers (ground points)
    extract_ground.filter(*ground_cloud);


    sensor_msgs::PointCloud2 groundPlaneCloud;    
    pcl::toROSMsg(*ground_cloud, groundPlaneCloud);

    if (!plane1.updateCoeff(groundPlaneCloud)){
        std::cout << "[noon]: failed to get ground" << std::endl;
        return ;
    }if (first_time){
        first_time = false;
        ground = plane1;
        ground_count++;
    }else{
        if (check_plane(ground, plane1)){
            ground.a = (ground.a*(ground_count) + plane1.a)/(ground_count + 1);
            ground.b = (ground.b*(ground_count) + plane1.b)/(ground_count + 1);
            ground.c = (ground.c*(ground_count) + plane1.c)/(ground_count + 1);
            ground.d = (ground.d*(ground_count) + plane1.d)/(ground_count + 1);
            ground_count++;
        }else{
            ramp.a = (ramp.a*ramp_count + plane1.a)/(ramp_count + 1);
            ramp.b = (ramp.b*ramp_count + plane1.b)/(ramp_count + 1);
            ramp.c = (ramp.c*ramp_count + plane1.c)/(ramp_count + 1);
            ramp.d = (ramp.d*ramp_count + plane1.d)/(ramp_count + 1);
            ramp_count++;
        }
    }

    groundReady = true;
    std_msgs::Float32MultiArray groundPlane;
    groundPlane.data.push_back(plane1.a);
    groundPlane.data.push_back(plane1.b);
    groundPlane.data.push_back(plane1.c);
    groundPlane.data.push_back(plane1.d);
    groundPlane.data.push_back(ground_threshold);

    groundPub.publish(groundPlane);

    if (noman){

        // Extract non-ground points (objects including ramp)
        pcl::PointCloud<pcl::PointXYZ>::Ptr objects_cloud(new pcl::PointCloud<pcl::PointXYZ>);
        extract_ground.setNegative(true);  // Extract the outliers (non-ground points)
        extract_ground.filter(*objects_cloud);

        //segment ramp
        seg.setDistanceThreshold(ramp_threshold);
        seg.setInputCloud(objects_cloud);
        seg.segment(*inliers, *coefficients);


        // Extract ramp points
        pcl::PointCloud<pcl::PointXYZ>::Ptr ramp_cloud(new pcl::PointCloud<pcl::PointXYZ>);
        pcl::ExtractIndices<pcl::PointXYZ> extract_ramp;
        extract_ramp.setInputCloud(objects_cloud);
        extract_ramp.setIndices(inliers);
        extract_ramp.setNegative(false);  // Extract the inliers (ground points)
        extract_ramp.filter(*ramp_cloud);

        //Publish segmented ramp plane
        sensor_msgs::PointCloud2 rampPlaneCloud;
        pcl::toROSMsg(*ramp_cloud, rampPlaneCloud);

        if (!plane2.updateCoeff(rampPlaneCloud)){
            std::cout << "[noon]: failed to get ramp" << std::endl;
            return ;
        }if (check_plane(ground, plane2)){
            ground.a = (ground.a*(ground_count) + plane2.a)/(ground_count + 1);
            ground.b = (ground.b*(ground_count) + plane2.b)/(ground_count + 1);
            ground.c = (ground.c*(ground_count) + plane2.c)/(ground_count + 1);
            ground.d = (ground.d*(ground_count) + plane2.d)/(ground_count + 1);
            ground_count++;
        }else{
            ramp.a = (ramp.a*ramp_count + plane2.a)/(ramp_count + 1);
            ramp.b = (ramp.b*ramp_count + plane2.b)/(ramp_count + 1);
            ramp.c = (ramp.c*ramp_count + plane2.c)/(ramp_count + 1);
            ramp.d = (ramp.d*ramp_count + plane2.d)/(ramp_count + 1);
            ramp_count++;
        }

        
        rampReady = true;
        std_msgs::Float32MultiArray rampPlane;
        rampPlane.data.push_back(ramp.a);
        rampPlane.data.push_back(ramp.b);
        rampPlane.data.push_back(ramp.c);
        rampPlane.data.push_back(ramp.d);
        rampPlane.data.push_back(ramp_threshold);

        rampPub.publish(rampPlane);
	test.publish(rampPlaneCloud);
    }
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

    groundPub = node.advertise<std_msgs::Float32MultiArray>("/ground", 10, true);
    rampPub = node.advertise<std_msgs::Float32MultiArray>("/ramp", 10, true);
    test = node.advertise<sensor_msgs::PointCloud2>("/only_ramp", 10, true);

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
