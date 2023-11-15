#include <iostream>
#include <cmath>

#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Point32.h>
#include <geometry_msgs/Vector3.h>

#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/PointCloud.h>

class quaternion{
	public:
		float x, y, z, w;
		quaternion(){
			this->w = 0;
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}

		quaternion(float x, float y, float z, float w){
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		quaternion(geometry_msgs::Quaternion q){
			this->x = q.x;
			this->y = q.y;
			this->z = q.z;
			this->w = q.w;
		}

		quaternion(geometry_msgs::Point p){
			this->x = p.x;
			this->y = p.y;
			this->z = p.z;
			this->w = 0;
		}

		quaternion(geometry_msgs::Vector3 v){
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
			this->w = 0;
		}

		void display(){
			std::cout << "[" << this->x << ", ";
			std::cout << this->y << ", ";
			std::cout << this->z << ", ";
			std::cout << this->w << "]" << std::endl;
		}

		quaternion operator+(const quaternion &q){
			return quaternion(this->x + q.x, this->y + q.y, this->z + q.z, this->w + q.w);
		}

		quaternion operator-(const quaternion &q){
			return quaternion(this->x - q.x, this->y - q.y, this->z - q.z, this->w - q.w);
		}

		quaternion operator*(const float t){
			return quaternion(this->x*t, this->y*t, this->z*t, this->w*t);
		}

		quaternion operator*(const quaternion &q){
			float X = this->w*q.x + this->x*q.w + this->y*q.z - this->z*q.y;
			float Y = this->w*q.y + this->y*q.w + this->z*q.x - this->x*q.z;
			float Z = this->w*q.z + this->z*q.w + this->x*q.y - this->y*q.x;
			float W = this->w*q.w - this->x*q.x - this->y*q.y - this->z*q.z;
			return quaternion(X, Y, Z, W);
		}

		float abs(){
			return sqrt(this->x*this->x + this->y*this->y + this->z*this->z + this->w*this->w);
		}

		quaternion unit(){
			float m = this->abs();
			return quaternion(this->x/m, this->y/m, this->z/m, this->w/m);
		}

		quaternion inverse(){
			float m = this->abs();
			m = m*m;
			return quaternion(-this->x/m, -this->y/m, -this->z/m, this->w/m);
		}

		geometry_msgs::Quaternion toQuaternion(){
			geometry_msgs::Quaternion Q;
			Q.x = this->x;
			Q.y = this->y;
			Q.z = this->z;
			Q.w = this->w;
			return Q;
		}

		geometry_msgs::Vector3 toVector(){
			geometry_msgs::Vector3 v;
			v.x = this->x;
			v.y = this->y;
			v.z = this->z;
			return v;
		}

		geometry_msgs::Point toPoint(){
			geometry_msgs::Point P;
			P.x = this->x;
			P.y = this->y;
			P.z = this->z;
			return P;
		}
};

class plane{
    public:
        float a;
        float b;
        float c;
        float d;

        //plane = ax + by + cz + d

        plane(){
            this->a = 0;
            this->b = 0;
            this->c = 0;
            this->d = 0;
        }

        plane(float a, float b, float c, float d){
            this->a = a;
            this->b = b;
            this->c = c;
            this->d = d;
        }

        bool updateCoeff(sensor_msgs::PointCloud2 cloud){
            int N = cloud.height * cloud.width;
            if (N < 50){
                std::cout << "[noon]: too less points" << std::endl;
                return false;
            }
            union{
                float val;
                unsigned char bytes[4];
            }X, Y, Z;

            //n sets
            quaternion fcoeff;
            quaternion coeff;
            std::vector<quaternion> points = std::vector<quaternion>(3, quaternion(0, 0, 0, 0));
            int n = 4;
            int m = 0;

            for (int t = 0; t < n; t++){
                int y;
                for (int r = 0; r < 3; r++){
                    y = rand()%N;
                    X.bytes[0] = cloud.data[y*16];
                    X.bytes[1] = cloud.data[y*16+1];
                    X.bytes[2] = cloud.data[y*16+2];
                    X.bytes[3] = cloud.data[y*16+3];

                    Y.bytes[0] = cloud.data[y*16+4];
                    Y.bytes[1] = cloud.data[y*16+5];
                    Y.bytes[2] = cloud.data[y*16+6];
                    Y.bytes[3] = cloud.data[y*16+7];

                    Z.bytes[0] = cloud.data[y*16+8];
                    Z.bytes[1] = cloud.data[y*16+9];
                    Z.bytes[2] = cloud.data[y*16+10];
                    Z.bytes[3] = cloud.data[y*16+11];
                
                    points[r] = quaternion(X.val, Y.val, Z.val, 0);
                }
                coeff = (points[0] - points[2]) * (points[1]-points[2]);
                coeff.w = (points[0]*coeff).w;
                if (!this->isValid(coeff)){
                    m++;
                    continue;
                }
                fcoeff = fcoeff + coeff;                
            }
            if (n == m){
                return false;
            }
            fcoeff = this->normPlane(fcoeff);
            a = fcoeff.x;
            b = fcoeff.y;
            c = fcoeff.z;
            d = fcoeff.w;
            return true;
        }

        bool updateCoeff(sensor_msgs::PointCloud cloud){
            //n sets
            quaternion fcoeff;
            quaternion coeff;
            std::vector<quaternion> points(3, quaternion(0, 0, 0, 0));
            geometry_msgs::Point32 point;

            int n = 4;
            int m = 0;

            int N = cloud.points.size();

            for (int t = 0; t < n; t++){
                int y;
                for (int r = 0; r < 3; r++){
                    y = rand()%N;
                    point = cloud.points[y];              
                    points[r] = quaternion(point.x, point.y, point.z, 0);
                }
                coeff = (points[0] - points[2]) * (points[1]-points[2]);
                coeff.w = (points[0]*coeff).w;
                if (!this->isValid(coeff)){
                    m++;
                    continue;
                }
                fcoeff = fcoeff + coeff;                
            }
            if (n == m){
                return false;
            }
            fcoeff = this->normPlane(fcoeff);
            a = fcoeff.x;
            b = fcoeff.y;
            c = fcoeff.z;
            d = fcoeff.w;
            return true;
        }

        bool isValid(quaternion plane){
            if (abs(plane.x) <= 1e-3 && abs(plane.y) <= 1e-3 && abs(plane.z) <= 1e-3){
                return false;
            }return true;
        }

    private:
        quaternion normPlane(quaternion q){
            float factor = sqrt(q.x*q.x + q.y*q.y + q.z*q.z) * q.w / abs(q.w);
            return quaternion(q.x/factor, q.y/factor, q.z/factor, q.w/factor);
        }
};
