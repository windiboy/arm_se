#include "MotorDriver.h"
#include "Kinematics.h"
#include <unistd.h>
#include <iostream>


uchar id[4] = {11, 12, 13, 14};
int max_speed[4] = {1000};
int max_acc[4] = {1000};
int home_pos[4] = {0};
int ratio[4] = {40};

int Baudrate = 1000000;
int current_pos[4];
double target_x,target_y;
double target_thet1,target_thet2;

MotorDriver motor;

int motor_init(){
    for(int i=1;i<=4;i++){
        motor.setMaxSpeed(id[i],max_speed[i]);
        motor.setAcc(id[i],max_acc[i]);
        motor.enable(id[i]);
    }
    for(int i=1;i<=4;i++) motor.setPos(id[i],home_pos[i]);
    read_pos();
}
int read_pos(){
    for(int i=1;i<=4;i++)
    {
        if(!motor.getPos(id[i], &current_pos[i])){
            std::cout << id[i] <<"motor: getpos failed!!!" <<std::endl;
            return -1;
        }
        std::cout << id[i] <<"motor pos: " << current_pos[i] << std::endl;
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
    double thet1,thet2;
    pos2ang(target_x,target_y, &thet1,&thet2);
    motor.pos_control(id[0],thet1,ratio[0]);
    motor.pos_control(id[1],thet2,ratio[1]);
}


int main(int argumentCount, const char* argumentValues[])
{
    if(connect_check()) {
        //motor_init();
        sleep(1);
        //cartesian_pos(1,0);
    }
    //二连杆正逆运动学测试
//    ang2pos(&target_x,&target_y,3.14/3,0);
//    std::cout << target_x << "   " << target_y <<std::endl;
//    pos2ang(1,-0.5, &target_thet1,&target_thet2);
//    std::cout << target_thet1 << "   " << target_thet2 <<std::endl;
    return 0;
}