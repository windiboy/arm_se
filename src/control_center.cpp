#include "ros/ros.h"
#include "arm_se/ArmControl.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "tf/transform_datatypes.h"
#include "../include/ArmDriver.h"
#include <iostream>
#include "math.h"
#include <unistd.h>
#include <deque>

using namespace std;

arm_se::ArmControl arm_msg;
geometry_msgs::Twist zoo_msg;
double object_pos[3];
double zoo_pos[3];//x y yaw
MovingAverage x_win,y_win,z_win;
std_msgs::String audio_data;


class MovingAverage {
public:
    /*
    * @param size: An integer
    */
    int size;
    deque<double> mydeque;
    double sum = 0;
    void init(int size) {
        this->size = size;
    }

    /*
     * @param val: An integer
     * @return:
     */
    double next(double val)
    {
        // write your code here
        if(mydeque.size() <= size)
        {
            mydeque.push_back(val);
            sum += val;
        }
        else
        {
            sum -= mydeque.front();
            sum += val;
            mydeque.pop_front();
            mydeque.push_back(val);
        }
        return (sum*1.0) / mydeque.size();
    }
};

void cameraCallback(const geometry_msgs::Point::ConstPtr& msg){
    object_pos[0] = x_win.next(msg->x)-0.15;
    object_pos[1] = y_win.next(msg->y);
    object_pos[2] = z_win.next(msg->z)-0.076;
//    ROS_INFO("hello");

}
void audioCallback(const std_msgs::String::ConstPtr& msg){
    audio_data = msg->data;
    ROS_INFO(audio_data);

}
void odomCallback(const nav_msgs::Odometry::ConstPtr& msg){
    zoo_pos[0] = msg->pose.pose.position.x;
    zoo_pos[1] = msg->pose.pose.position.y;
    tf::Quaternion quat;
    tf::quaternionMsgToTF(msg->pose.pose.orientation, quat);

    
    double roll, pitch, yaw;//定义存储r\p\y的容器
    tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);//进行转换
    zoo_pos[2] = yaw;

}
bool zooMove();
void tryPick(const ros::Subscriber arm_pub, const ros::Subscriber zoo_pub){
    if(zooMove() || temp<100){
        zoo_pub.publish(zoo_msg);
        cout<<"Zoo Moving !!!!!!! "<<endl;
    } else{
        cout<<"####### Try Pick ####### "<<endl;
        double offset = -0.;
        arm_msg.target_x = 0;
        arm_msg.target_y = 0;
        arm_msg.rotation = 0;
        arm_msg.gripper = 1;
        arm_msg.platform = object_pos[1]+offset;
        arm_pub.publish(arm_msg);
        sleep(5);

        arm_msg.target_x = object_pos[0];
        arm_msg.target_y = 0;
        arm_msg.rotation = 0;
        arm_msg.gripper = 1;
        arm_msg.platform = object_pos[1]+offset;
        arm_pub.publish(arm_msg);
        sleep(3);

        arm_msg.target_x = object_pos[0];
        arm_msg.target_y = 0;
        arm_msg.rotation = 0;
        arm_msg.gripper = 0.3;
        arm_msg.platform = object_pos[1]+offset;
        arm_pub.publish(arm_msg);
        sleep(3);
        arm_msg.target_x = object_pos[0];
        arm_msg.target_y = 0;
        arm_msg.rotation = 0;
        arm_msg.gripper = 0.3;
        arm_msg.platform = 0.3;
        arm_pub.publish(arm_msg);
        sleep(3);
    }
}
int main(int argc, char** argv){
    ros::init(argc, argv, "control_center");
    ros::NodeHandle n;

    ros::Publisher arm_pub = n.advertise<arm_se::ArmControl>("arm_control", 50);
    ros::Publisher zoo_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 50);
    ros::Subscriber cam_sub = n.subscribe("camera_point",1000,cameraCallback);
    ros::Subscriber odom_sub = n.subscribe("odom",1000,odomCallback);
    ros::Subscriber audio_sub = n.subscribe("audio_oub",1000,audioCallback);
    ros::Rate loop_rate(50);

    x_win.init(10);
    y_win.init(10);
    z_win.init(10);

    while (ros::ok()){
        ros::spinOnce();

        cout<<"Object position In Robot Coordinate "<<"( "<<object_pos[0] <<"," << object_pos[1] <<"," << object_pos[2] <<" )"<<endl;
        cout<<"Zoo position"<<"( "<<zoo_pos[0] <<"," << zoo_pos[1] <<"," << zoo_pos[2] <<" )"<<endl;
        cout<<"Arm position"<<"( "<<current_pos[0] <<"," << current_pos[1] <<" )"<<endl;

        cout<<"Audio data:"<< audio_data << endl;

        loop_rate.sleep();
    }
    return 0;
}
bool zooMove()
{
    double front = 0.55;
    double rear = 0.45;
    
    if(object_pos[0]>front)
        zoo_msg.linear.x = 0.01;
    if(object_pos[0]<rear)
        zoo_msg.linear.x = -0.01;
    if(object_pos[2]>0.01)
        zoo_msg.linear.y = -0.01;
    if(object_pos[2]<-0.01)
        zoo_msg.linear.y = 0.01;
    if(object_pos[2]>-0.01 && object_pos[2]<0.01)
    {
        zoo_msg.linear.y = 0.0;
    }
    if(object_pos[0]>rear && object_pos[0]<front)
    {
        zoo_msg.linear.x = 0.0;
    }
    if(object_pos[2]>-0.005 && object_pos[2]<0.005 && object_pos[0]>rear && object_pos[0]<front)
    {
        zoo_msg.linear.y = 0.0;
        zoo_msg.linear.x = 0.0;
        return false;
    }
    // cout<<zoo_msg<<endl;
    return true;
}
