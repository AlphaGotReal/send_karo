#include <iostream>
#include <cmath>
#include <vector>

#include <ros/ros.h>

#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

class filter{
public:
    filter(int argc, char **argv){
        ros::init(argc, argv, "filter");
        ros::NodeHandle node;

        this->image_sub = node.subscribe("/zed/rgb/image_raw", 10, &filter::get_image, this);
        this->mask_pub = node.advertise<sensor_msgs::Image>("/lane_mask", 10, true);
    }

    void run(){

        ros::AsyncSpinner spinner(4);
        spinner.start();

        while (ros::ok()){

        }
    }

private:

    // subscribers
    ros::Subscriber image_sub;

    // publishers
    ros::Publisher mask_pub;

    // published data
    cv_bridge::CvImage mask;

    void get_image(const sensor_msgs::Image::ConstPtr &image){
        cv_bridge::CvImagePtr cv_ptr;
        try{
            cv_ptr = cv_bridge::toCvCopy(image, sensor_msgs::image_encodings::BGR8);
        }catch (cv_bridge::Exception &e){
            std::cout << e.what() << std::endl;
            return ;
        }

        cv::Mat hsv_image;
        cv::cvtColor(cv_ptr->image, hsv_image, cv::COLOR_BGR2HSV);

        cv::Scalar lower_bound(0, 0, 80);
        cv::Scalar upper_bound(0, 0, 147);
        
        cv::Mat mask_image;
        cv::inRange(hsv_image, lower_bound, upper_bound, mask_image);

        this->mask.header = image->header;
        this->mask.encoding = sensor_msgs::image_encodings::MONO8;
        this->mask.image = mask_image;

        this->mask_pub.publish(this->mask.toImageMsg()); 
        std::cout << "mask published" << std::endl;
    }

};

int main(int argc, char **argv){

    filter node(argc, argv);
    node.run();

    return 0;    
}

