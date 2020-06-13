#include "Kinematics.h"

//以机器人正前方为x轴正方向  竖直向上为y轴正方向
int ang2pos(double *x, double *y , double thet1, double thet2){
    *x = L1*cos(thet1)+L2*cos(thet1+thet2);
    *y = L1*sin(thet1)+L2*sin(thet1+thet2);
}
int pos2ang(double x, double y , double *thet1, double *thet2){
    double thetp = atan2(y,x);
    *thet1 = acos((x*x+y*y+L1*L1-L2*L2)*cos(thetp)/(2*L1*x))+thetp;
    *thet2 = acos((x*x+y*y-L1*L1+L2*L2)*cos(thetp)/(2*L2*x))+thetp-*thet1;
}