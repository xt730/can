#ifndef _pid_H_
#define _pid_H_

#include "main.h"
#include "motor.h"


struct PID
{
	float kp,ki,kd;
	int epsilon;
	int speed_output;
	int e_int,e_dif,e_k,e_k_1;
};
int abs(int num);
static struct PID speed_k,deg_k;
void speed_pid(int target,struct PID *speed_k,float *motor_data);
void deg_pid(int target, struct PID *deg_k, float *motor_data);

#endif
