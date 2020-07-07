#include "../include/ArmDriver.h"
#include "ros/ros.h"
#include "arm_se/ArmControl.h"
#include <iostream>
#include <unistd.h>

#define PI 3.1415926

ArmDriver arm;

void controlCallback(const arm_se::ArmControl::ConstPtr& msg){
    arm.write_pos(msg->target_x,msg->target_y,msg->rotation,msg->gripper,msg->platform);
//    std::cout << msg->target_x << std::endl;
    ROS_INFO("hello");

}

int main(int argc, char** argv)
{
    ros::init(argc,argv, "arm_se");
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("arm_control",1000,controlCallback);
    ros::Rate loop_rate(50);
    ROS_INFO("############Arm init#############");

    arm.motor_init();
    sleep(2);
    arm.write_pos(0.3,0,0,0,0.25);

//    ros::spin();
    while (ros::ok())
    {

        loop_rate.sleep();
        ros::spinOnce();
    }

    return 0;
}

