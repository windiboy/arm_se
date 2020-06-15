#include "ros/ros.h"
#include "std_msgs/String.h"
#include <iostream>
#include <sstream>

int main(int argumentCount, char* argumentValues[])
{
    ros::init(argumentCount,argumentValues, "manual_control");
    ros::NodeHandle n;

    ros::Publisher pub = n.advertise<std_msgs::String>("cmd",1);
    ros::Rate loop_rate(1);

    char c;
    while (ros::ok()){

        std_msgs::String msg;

        std::cin >> c;
        msg.data = c;

        pub.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
}

