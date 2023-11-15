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

class terrain{
	public:
		bool move;
		float x;
		float y;
		float speed = 0.01;

		terrain(){
			move = false;
		}

		void drop(float x, float y){
			this->move = true;
			this->x = x;
			this->y = y;
		}

		void update_goal(){
			this->update_obstacles();
			if (this->move){
				geometry_msgs::Vector3 grad = this->gradient(x, y);
				this->x = this->x + this->speed * grad.x;
				this->y = this->y + this->speed * grad.y;
				sensor_msgs::PointCloud goal;
				goal.header.frame_id = "map";
				geometry_msgs::Point32 point;
				point.x = this->x;
				point.y = this->y;
				point.z = this->obstacle_cost(x, y) + this->bot_cost(x, y);
				goal.points.push_back(point);
				send_goal.publish(goal);
			}
		}

		void lift(){
			this->move = false;
		}

		void update_obstacles(){
			union{
				float val;
				unsigned char bytes[4];
			}X, Y, Z;

			geometry_msgs::Pose pose;
			geometry_msgs::TransformStamped wrt_map;

			try{
				wrt_map = buffer.lookupTransform(
					"map", "zed2i_depth",
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

			std::vector<geometry_msgs::Point> ob;
			for (int t = 0; t < obstacle_indices.data.size(); t++){
				int r = obstacle_indices.data[t]; //r is the index
			
				X.bytes[0] = cloud.data[32*r];
				X.bytes[1] = cloud.data[32*r+1];
				X.bytes[2] = cloud.data[32*r+2];
				X.bytes[3] = cloud.data[32*r+3];

				Y.bytes[0] = cloud.data[32*r+4];
				Y.bytes[1] = cloud.data[32*r+5];
				Y.bytes[2] = cloud.data[32*r+6];
				Y.bytes[3] = cloud.data[32*r+7];

				Z.bytes[0] = cloud.data[32*r+8];
				Z.bytes[1] = cloud.data[32*r+9];
				Z.bytes[2] = cloud.data[32*r+10];
				Z.bytes[3] = cloud.data[32*r+11];

				pose.position.x = X.val;
				pose.position.y = Y.val;
				pose.position.z = Z.val;

				tf2::doTransform(pose, pose, wrt_map);

				ob.push_back(pose.position);

			}this->obstacles = ob;
		}

		void generate(){
			std::cout << "[night]: generating" << std::endl;
			this->update_obstacles();
			int n = map_data.height * map_data.width;
			sensor_msgs::PointCloud terr;
			geometry_msgs::Point32 point;
			terr.header.frame_id = "map";
			int botx = (bot.position.x - map_data.origin.position.x)/map_data.resolution;
			int boty = (bot.position.y - map_data.origin.position.y)/map_data.resolution;
			for (int Y = boty - 300; Y < boty + 300; Y++){
				for (int X = botx - 300; X < botx + 300; X++){
					int t = X + Y * map_data.width;
					//std::cout << "[night]: node number = " << t << std::endl;
					float x = (t%map_data.width) * map_data.resolution + map_data.origin.position.x;
					float y = (t/map_data.width) * map_data.resolution + map_data.origin.position.y;
					float cost = this->bot_cost(x, y) + this->obstacle_cost(x, y);
					point.x = x;
					point.y = y;
					point.z = cost;
					terr.points.push_back(point);
				}
			}
			send_cost.publish(terr);
		}

	private:

		//params
		float bot_cost_amplitude = 3;
		float bot_cost_std_deviation = 20;

		float obstacle_cost_amplitude = 4;
		float obstacle_cost_std_deviation =  0.4;
		////

		std::vector<geometry_msgs::Point> obstacles;

		float bot_cost(float x, float y){
			float dx = x - bot.position.x;
			float dy = y - bot.position.y;
			return bot_cost_amplitude / exp((dx*dx + dy*dy)/bot_cost_std_deviation);
		}

		float obstacle_cost(float x, float y){
			float cost = 0;
			float max_obstacles_cost = -1;
			for (int t = 0; t < this->obstacles.size(); t++){
				float dx = x - this->obstacles[t].x;
				float dy = y - this->obstacles[t].y;
				cost = 1/exp((dx*dx + dy*dy)/obstacle_cost_std_deviation);
				max_obstacles_cost = std::max(max_obstacles_cost, cost);
			}return obstacle_cost_amplitude * max_obstacles_cost;
		}

		geometry_msgs::Vector3 gradient(float x, float y){

			geometry_msgs::Vector3 grad;
			float bot_c = this->bot_cost(x, y);
			float dx = x - bot.position.x;
			float dy = y - bot.position.y;
			grad.x = -2 * dx * bot_c / this->bot_cost_std_deviation;
			grad.x = -2 * dy * bot_c / this->bot_cost_std_deviation;
			float cost, max_obstacles_cost = -1;
			float max_dx, max_dy;
			for (int t = 0; t < this->obstacles.size(); t++){
				float dx = x - this->obstacles[t].x;
				float dy = y - this->obstacles[t].y;
				cost = 1/exp((dx*dx + dy*dy)/obstacle_cost_std_deviation);
				if (cost > max_obstacles_cost){
					max_obstacles_cost = cost;
					max_dx = dx;
					max_dy = dy;
				}
			}
			//grad.x += -2 * max_dx * max_obstacles_cost / this->obstacle_cost_std_deviation;
			//grad.y += -2 * max_dy * max_obstacles_cost / this->obstacle_cost_std_deviation;

			return grad;
		}
};

terrain surface;

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
		if (!got_bot){
			surface.drop(-4, -23);
		}
		got_bot = true;

		if (!(got_obstacles && got_points && got_bot && got_map)){
			std::cout << "[night]: " << got_obstacles << got_points << got_bot << got_map << std::endl;
			continue;
		}

		//surface.generate();
		//continue;

		surface.update_goal();
		std::cout << "[night]: goal points > " << surface.x << ", " << surface.y << std::endl;
	}

	return 0;
}

