#include "ros/ros.h"
#include "arm_se/ArmControl.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Twist.h"
#include <iostream>
#include "math.h"
#include <unistd.h>

using namespace std;

arm_se::ArmControl arm_msg;
geometry_msgs::Twist zoo_msg;
float object_pos[3];

public class MovingAverage {
private final int maxSize;
private final Queue<Integer> window;
private long sum = 0;

public MovingAverage(int maxSize) {
        this.maxSize = maxSize;
        this.window = new ArrayDeque<>(maxSize + 1);
    }

public double next(int val) {
        window.add(val);
        sum += val;
        if (window.size() > maxSize) {
            sum -= window.poll();
        }
        return (double) sum / window.size();
    }
}
MovingAverage x_win = new MovingAverage(10);
MovingAverage y_win = new MovingAverage(10);
MovingAverage y_win = new MovingAverage(10);

void cameraCallback(const geometry_msgs::Point::ConstPtr& msg){
    object_pos[0] = x_win.next(msg->x);
    object_pos[1] = y_win.next(msg->y);
    object_pos[2] = z_win.next(msg->z);
//    ROS_INFO("hello");

}
int main(int argc, char** argv){
    ros::init(argc, argv, "control_center");
    ros::NodeHandle n;

    ros::Publisher arm_pub = n.advertise<arm_se::ArmControl>("arm_control", 50);
    ros::Publisher zoo_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 50);
    ros::Subscriber cam_sub = n.subscribe("camera_point",1000,cameraCallback);
    ros::Rate loop_rate(50);



    while (ros::ok()){
        ros::spinOnce();

        object_pos[0]=object_pos[0]-0.1;
        cout<<"Object position In Robot Coordinate "<<"( "<<object_pos[0] <<"," << object_pos[1] <<"," << object_pos[2] <<" )"<<endl;

        if(object_pos[0]>0.55 && object_pos[0]<0.3){
            cout<<"Too Far Or Too Close !!!!!!! "<<endl;
        } else{

            arm_msg.target_x = object_pos[0];
            arm_msg.target_y = 0;
            arm_msg.rotation = 0;
            arm_msg.gripper = 1;
            arm_msg.platform = object_pos[1];
//            arm_pub.publish(arm_msg);
            cout<<"####### Try Pick ####### "<<endl;

//            sleep(2);
//            arm_msg.target_x = object_pos[0];
//            arm_msg.target_y = 0;
//            arm_msg.rotation = 0;
//            arm_msg.gripper = 0;
//            arm_msg.platform = object_pos[1]+0.1;
//            arm_pub.publish(arm_msg);
        }

        loop_rate.sleep();
    }
    return 0;
}

