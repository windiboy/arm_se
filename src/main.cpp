#include "MotorDriver.h"
#include <iostream>


uchar id[4] = {11, 12, 13, 14};
int Baudrate = 1000000;
int pos[4];

MotorDriver motor[4];

int motor_init(){
    int max_speed[4] = {};
    int max_acc[4] = {};
    int home_pos[4] = {0};
    int ratio[4] = {41};

    for(int i=1;i<=4;i++){
        motor[i].setMaxSpeed(id[i],max_speed[i]);
        motor[i].setAcc(id[i],max_acc[i]);
    }
    for(int i=1;i<=4;i++) motor[i].pos_control(id[i],home_pos[i],ratio[i]);
}
int connect_check()
{
    std::cout << "############### connect check! ###############" <<std::endl;
    for(int i=1;i<=4;i++)
    {
        if(!motor[i].init(id[i],Baudrate)){
            std::cout << id[i] <<"motor: connect failed!!!" <<std::endl;
            return -1;
        }

    }
    for(int i=1;i<=4;i++)
    {
        if(!motor[i].getPos(id[i], &pos[i])){
            std::cout << id[i] <<"motor: getpos failed!!!" <<std::endl;
            return -1;
        }
        std::cout << id[i] <<"motor pos: " << pos[i] << std::endl;
    }
    std::cout << "############### connect check is ok! ###############" <<std::endl;
    return 1;

}
int main(int argumentCount, const char* argumentValues[])
{
    if(connect_check())
        motor_init();
    return 0;
}