#include "../include/MotorDriver.h"
#include "../include/Kinematics.h"
#include "../include/gnuplot-iostream.h"
#include "../include/cubicSpline.h"
#include "ros/ros.h"
#include "math.h"
#include "std_msgs/String.h"
#include <unistd.h>
#include <iostream>

#define PI 3.1415926


uchar id[4] = {11, 12, 13, 14};
int max_speed[4] = {1000,1000,1000,1000};
int max_acc[4] = {1000,1000,1000,1000};
int home_pos[4] = {0,0,0,850};
int ratio[4] = {40,40,40,40};

int Baudrate = 1000000;
double current_pos[4];
double target_x,target_y;
double target_thet1,target_thet2;

char command = ' ';
int times = 0;

MotorDriver motor;
Gnuplot gp;

int read_pos();
int write_pos(double *target);
void commandCallback(const std_msgs::String::ConstPtr& msg){
    command = *msg->data.c_str();

}
int motor_init(){
    for(int i=0;i<=3;i++){
        motor.setMaxSpeed(id[i],max_speed[i]);
        motor.setAcc(id[i],max_acc[i]);

        motor.enable(id[i]);
        motor.mSleep(10);
    }
    for(int i=0;i<=3;i++) {
        motor.setPos(id[i],home_pos[i]);
        motor.mSleep(10);
    }
    motor.enable(10);
    motor.mSleep(10);

    read_pos();
}
int read_pos(){
    int pos[3];
    for(int i=0;i<=2;i++)
    {
        if(!motor.getPos(id[i], &pos[i])){
            std::cout << id[i] <<"motor: getpos failed!!!" <<std::endl;
            return -1;
        }
        motor.mSleep(10);
        current_pos[i] = pos[i]*PI*2/(262144*ratio[i]);
        current_pos[0] = -current_pos[0];
        std::cout << id[i] <<"motor pos: " << current_pos[i] << std::endl;
    }

    std::vector<std::pair<double,double>> data1,data2;
    times = times +1;
    data1.push_back(std::make_pair(times,current_pos[0]));
    data2.push_back(std::make_pair(times,current_pos[1]));
    gp << "plot" << gp.file1d(data1) << "with lines title 'current_pos[0]'," <<
    gp.file1d(data2) << "with lines title 'current_pos[1]'," << std::endl;


}
    //target 共五位
    //第一位；11号电机弧度 -PI/2～PI/2
    //第二位：12号电机弧度 -PI*3/2～PI*3/2
    //第三位：13号电机弧度 -PI～PI
    //第四位：夹抓电机 0-1 0加紧 1打开
    //第五位：10号电机 0-0.25 向上为正
int write_pos(double *target){
    int pos[5];

    if(!(target[0]>=-PI/2 && target[0]<=PI/2 &&
       target[1]>=-PI*3/2 && target[1]<=PI*3/2 &&
       target[2]>=-PI && target[2]<=PI &&
       target[3]>=0 && target[3]<=1 &&
       target[4]>=0 && target[4]<=0.25)){
        std::cout << "############### out of range! ###############" <<std::endl;
        return -1;
    }

    for(int i=0;i<=2;i++) {

        pos[i] = target[i]*262144*ratio[i]/(PI*2);
        pos[0] = -pos[0];
        pos[2] = -pos[2];

//        std::cout << "target pos: " << pos[i] << std::endl;
        motor.setPos(id[i], pos[i]);
        motor.mSleep(10);
    }

    pos[3] = target[3]*(850-350)+350;
//    std::cout << "target pos: " << pos[3] << std::endl;
    motor.setPos(id[3], pos[3]);
    motor.mSleep(10);

    pos[4] = target[4]*-262144*500/0.25;
//    std::cout << "target pos: " << pos[4] << std::endl;
    motor.setPos(10, pos[4]);
    motor.mSleep(10);
}
int write_pos_speed_acc(double *target, int *speed , int *acc ){
    int pos[5];

    if(!(target[0]>=-PI/2 && target[0]<=PI/2 &&
         target[1]>=-PI*3/2 && target[1]<=PI*3/2)){
        std::cout << "############### out of range! ###############" <<std::endl;
        return -1;
    }

    for(int i=0;i<=1;i++) {

        pos[i] = target[i]*262144*ratio[i]/(PI*2);
        pos[0] = -pos[0];

        motor.setAcc_MaxSpeed_Pos(id[i], pos[i],speed[i],acc[i]);
        motor.mSleep(10);
    }
}

int connect_check()
{
    std::cout << "############### connect check! ###############" <<std::endl;

    if(!motor.init(10,Baudrate))
        return -1;
    read_pos();
    std::cout << "############### connect check is ok! ###############" <<std::endl;
    return 1;

}

int cartesian_pos(double target_x, double target_y, double rotation, double gripper, double platform){
    double target[5];
    pos2ang(target_x,target_y, &target[0],&target[1]);
    target[2] = rotation;
    target[3] = gripper;
    target[4] = platform;

    std::cout << target[0] << "  " << target[1] << std::endl;
    write_pos(target);
}
int keyboard_control(double x=0.35, double y = 0, double index = 0.02){
    std::cout << "cmd:" << command << x << y <<std::endl;
    if(command == 'i')y = y+index;
    if(command == ',')y = y-index;
    if(command == 'l')x = x+index;
    if(command == 'j')x = x-index;
    if(command == 'k'){
        x=0.35;
        y=0.;
    }

    cartesian_pos(x,y,0,0,0.25);
}
int draw_circle(double center_x=0.25, double center_y=0, double r=0.08){
    double x,y;
    std::vector<std::pair<double,double>> data;

    for(double t=0; t <= 1; t = t+0.02){
        x = r*sin(t*2*PI)+center_x;
        y = r*cos(t*2*PI)+center_y;

//        data.push_back(std::make_pair(x,y));
//        gp << "plot" << gp.file1d(data) << "with lines title 'circle'," << std::endl;

        cartesian_pos(x,y,0,0,0.25);
        motor.mSleep(10);
    }

}
void test1(){
    int POINTS_COUNT = 4;
    double x_data[POINTS_COUNT] = {0, 1, 2};
    double y_data[POINTS_COUNT] = {0, 0.8, 1.6};

    double x_out = 0;
    double y_out = 0;
    Gnuplot gp;
    std::vector<std::pair<double, double> > data;

    cubicSpline spline;
    spline.loadData(x_data, y_data, POINTS_COUNT, 0, 0, cubicSpline::BoundType_First_Derivative);

    x_out = -0.008;
    for(int i=0; i<=250; i++)
    {
        x_out = x_out + 0.008;
        spline.getYbyX(x_out, y_out);
        printf("%f, %0.9f \n", x_out, y_out);
        data.push_back(std::make_pair(x_out, y_out));
        gp << "plot" << gp.file1d(data) << "with lines title 'cubic'," << std::endl;
    }

}

int main(int argumentCount, char* argumentValues[])
{
    ros::init(argumentCount,argumentValues, "arm_se");
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("cmd",1000,commandCallback);
    ros::Rate loop_rate(1);

    if(connect_check()) {
        motor_init();
        sleep(2);
//        cartesian_pos(0.25,0,PI/2,0,0.25);
//        test1();
    }

    while (ros::ok())
    {
        draw_circle();
        loop_rate.sleep();
        ros::spinOnce();
    }

    return 0;
}