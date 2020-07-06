#include "ros/ros.h"
#include "arm_se/ArmControl.h"
#include "geometry_msgs/Point.h"
#include <iostream>

using namespace std;

arm_se::ArmControl arm_msg;
geometry_msgs::Point cam_recognition;


void cameraCallback(const geometry_msgs::Point::ConstPtr& msg){
    cam_recognition = &msg;
//    ROS_INFO("hello");

}
int main(int argc, char** argv){
    ros::init(argc, argv, "control_center");
    ros::NodeHandle n;

    ros::Publisher arm_pub = n.advertise<arm_se::ArmControl>("arm_control", 50);
    ros::Subscriber cam_sub = n.subscribe("camera_point",1000,cameraCallback);
    ros::Rate loop_rate(50);



    while (ros::ok()){
        cout<<"Object position In Robot Coordinate "<<"( "<<cam_recognition.x <<","<<cam_recognition.y<","<< cam_recognition.z <<" )"<<endl;

        loop_rate.sleep();
        ros::spinOnce();
    }
    return 0;
}

