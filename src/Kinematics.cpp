#include "Kinematics.h"
#include <iostream>

//以机器人正前方为x轴正方向  竖直向上为y轴正方向
int ang2pos(double *x, double *y , double thet1, double thet2){
    *x = L1*cos(thet1)+L2*cos(thet1+thet2);
    *y = L1*sin(thet1)+L2*sin(thet1+thet2);
    return 1;
}
int pos2ang(double x, double y , double *thet1, double *thet2){
    double c2,s2;
    c2 = (x*x+y*y-L1*L1-L2*L2)/(2*L1*L2);
    if(c2 >= -1 && c2 <= 1) {
        s2 = -1 * sqrt(1 - c2 * c2);
        *thet2 = atan2(s2, c2);
        *thet1 = atan2(y, x) - atan2(L2 * s2, L1 + L2 * c2);
        return 1;
    } else
        return -1;
}