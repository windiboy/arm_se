#include "ros/ros.h"
#include "arm_se/ArmControl.h"
#include "geometry_msgs/Point.h"
#include <iostream>

using namespace std;

arm_se::ArmControl arm_msg;
float object_pos[3];


void cameraCallback(const geometry_msgs::Point::ConstPtr& msg){
    object_pos[0] = msg->x;
    object_pos[1] = msg->y;
    object_pos[2] = msg->z;
//    ROS_INFO("hello");

}
int main(int argc, char** argv){
    ros::init(argc, argv, "control_center");
    ros::NodeHandle n;

    ros::Publisher arm_pub = n.advertise<arm_se::ArmControl>("arm_control", 50);
    ros::Subscriber cam_sub = n.subscribe("camera_point",1000,cameraCallback);
    ros::Rate loop_rate(50);



    while (ros::ok()){
        cout<<"Object position In Robot Coordinate "<<"( "<<object_pos[0] <<"," << object_pos[1] <<"," << object_pos[2] <<" )"<<endl;

        loop_rate.sleep();
        ros::spinOnce();
    }
    return 0;
}

