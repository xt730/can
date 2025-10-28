#include "pid.h"

int abs(int num)
{
  return ((num >= 0) ? num : (-num));
}
void speed_pid(int target, struct PID *speed_k, float *motor_data)
{
  (*speed_k).e_k = target - motor_data[1];
  (*speed_k).e_dif = (*speed_k).e_k - (*speed_k).e_k_1;
  (*speed_k).e_int += (*speed_k).e_k;
  (*speed_k).speed_output = (int)(
                              (*speed_k).kp * (*speed_k).e_k +
                              (*speed_k).ki * (*speed_k).e_int +
                              (*speed_k).kd * (*speed_k).e_dif
                            );

  if((*speed_k).speed_output >= 24000)
  {
    (*speed_k).speed_output = 24000;
  }

  if((*speed_k).speed_output <= -24000)
  {
    (*speed_k).speed_output = -24000;
  }

  (*speed_k).e_k_1 = (*speed_k).e_k;

}

void deg_pid(int target, struct PID *deg_k, float *motor_data)
{
  int encoder_max = 8192;
  int pos = (int)motor_data[0];
  int error, error_wrap;
  error = target - pos;
  error_wrap = error;

  if (error > encoder_max / 2)
  {
    error_wrap = error - encoder_max;
  }
  else if (error < -encoder_max / 2)
  {
    error_wrap = error + encoder_max;
  }

  (*deg_k).e_k = error_wrap;

  if (abs((*deg_k).e_k) < (*deg_k).epsilon)
  {
    (*deg_k).e_int += (*deg_k).e_k;
  }

  (*deg_k).e_dif = (*deg_k).e_k - (*deg_k).e_k_1;

  (*deg_k).speed_output = (int)(
                            (*deg_k).kp * (*deg_k).e_k  +
                            (*deg_k).ki * (*deg_k).e_int +
                            (*deg_k).kd * (*deg_k).e_dif);
  (*deg_k).e_k_1 = (*deg_k).e_k;

  if((*deg_k).speed_output >= 200)
  {
    (*deg_k).speed_output = 200;
  }

  if((*deg_k).speed_output <= -200)
  {
    (*deg_k).speed_output = -200;
  }
}