#ifndef ARM_SE_KINEMATICS_H
#define ARM_SE_KINEMATICS_H

#include "math.h"

double L1 = 1;
double L2 = 1;
int ang2pos(double *x, double *y , double thet1, double thet2);
int pos2ang(double x, double y , double *thet1, double *thet2);

#endif //ARM_SE_KINEMATICS_H
