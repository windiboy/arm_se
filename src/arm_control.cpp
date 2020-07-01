#include "../include/ArmDriver.h"
#include "ros/ros.h"
#include "arm_se/ArmControl.h"
#include <iostream>

#define PI 3.1415926

ArmDriver arm;

void controlCallback(const arm_se::ArmControl::ConstPtr& msg){
    arm.write_pos(msg->target_x,msg->target_y,msg->rotation,msg->gripper,msg->platform);
//    std::cout << msg->target_x << std::endl;
//    ROS_INFO("hello");

}

int main(int argumentCount, char* argumentValues[])
{
    ros::init(argumentCount,argumentValues, "arm_se");
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("arm_control",1000,controlCallback);
    ros::Rate loop_rate(50);
    arm.motor_init();

    ros::spin();
//    while (ros::ok())
//    {
//
//        loop_rate.sleep();
//        ros::spinOnce();
//    }

    return 0;
}

