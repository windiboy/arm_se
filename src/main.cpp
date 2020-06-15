#include "MotorDriver.h"
#include "Kinematics.h"
#include <unistd.h>
#include <iostream>

#define PI 3.1415926


uchar id[4] = {11, 12, 13, 14};
int max_speed[4] = {1000};
int max_acc[4] = {1000};
int home_pos[4] = {0,0,0,0};
int ratio[4] = {40,40,40,40};

int Baudrate = 1000000;
double current_pos[4];
double target_x,target_y;
double target_thet1,target_thet2;

MotorDriver motor;

int read_pos();
int write_pos(double *target);
int motor_init(){
    for(int i=0;i<=3;i++){
//        motor.setMaxSpeed(id[i],max_speed[i]);
//        motor.setAcc(id[i],max_acc[i]);
        motor.enable(id[i]);
        motor.mSleep(10);
    }
    for(int i=0;i<=3;i++) {
        motor.setPos(id[i],home_pos[i]);
        motor.mSleep(10);
    }
    read_pos();
}
int read_pos(){
    int pos[4];
    for(int i=0;i<=3;i++)
    {
        if(!motor.getPos(id[i], &pos[i])){
            std::cout << id[i] <<"motor: getpos failed!!!" <<std::endl;
            return -1;
        }
        motor.mSleep(10);
        current_pos[i] = pos[i]*PI*2/(262144*ratio[i]);
        std::cout << id[i] <<"motor pos: " << current_pos[i] << std::endl;
    }

}
int write_pos(double *target){
    int pos[4];
    for(int i=0;i<=3;i++) {

        pos[i] = target[i]*262144*ratio[i]/(PI*2);

        pos[0] = -pos[0];

        std::cout << id[i] <<"target pos: " << pos[i] << std::endl;
        motor.setPos(id[i], pos[i]);
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

int cartesian_pos(double target_x, double target_y){
    double target[4];
    pos2ang(target_x,target_y, &target[0],&target[1]);
    target[2] = 0;
    target[3] = 0;

    std::cout << target[0] << "  " << target[1] << std::endl;
    write_pos(target);
}


int main(int argumentCount, const char* argumentValues[])
{
    if(connect_check()) {
        //motor_init();
        sleep(1);
        //cartesian_pos(0.25,0);
    }
    //二连杆正逆运动学测试
//    ang2pos(&target_x,&target_y,3.14/3,0);
//    std::cout << target_x << "   " << target_y <<std::endl;
//    pos2ang(1,-0.5, &target_thet1,&target_thet2);
//    std::cout << target_thet1 << "   " << target_thet2 <<std::endl;
    return 0;
}