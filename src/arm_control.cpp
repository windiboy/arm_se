#include "../include/ArmDriver.h"
#include "ros/ros.h"
#include "arm_se/ArmControl.h"
#include <iostream>
#include <unistd.h>

#define PI 3.1415926

ArmDriver arm;

void controlCallback(const arm_se::ArmControl::ConstPtr& msg){
    // arm.write_pos(msg->target_x,msg->target_y,msg->rotation,msg->gripper,msg->platform);
    if(msg->target_x==0 && msg->target_y ==0)
        arm.write_platform_pos(msg->platform);
    else
        arm.interpolation(current_pos[0],current_pos[1],msg->target_x,msg->target_y,20,msg->rotation,msg->gripper,msg->platform);
//    std::cout <<"( "<<msg->target_x <<"," << msg->target_y <<")" << std::endl;

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
    double ready_pos[5]={PI/2,-PI*100/180,0,1,0.1};
    arm.write_angle(ready_pos);

//    ros::spin();
    while (ros::ok())
    {
        arm.read_pos();
        // std::cout <<"( "<<current_pos[0] <<"," << current_pos[1] <<")" << std::endl;
        loop_rate.sleep();
        ros::spinOnce();
    }

    return 0;
}

