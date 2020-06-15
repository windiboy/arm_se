#ifndef ARM_SE_KINEMATICS_H
#define ARM_SE_KINEMATICS_H

#include "math.h"

double L1 = 0.15;
double L2 = 0.2;
int ang2pos(double *x, double *y , double thet1, double thet2);
int pos2ang(double x, double y , double *thet1, double *thet2);

#endif //ARM_SE_KINEMATICS_H
