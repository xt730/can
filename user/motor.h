#ifndef _motor_H_
#define _motor_H_

#include "main.h"
#include "can.h"

extern float motor_data[4];
static uint8_t volt_val[8];

void can_filter_init(void);
void Motor_send(uint16_t ID, uint8_t len, uint8_t *TxData);
void data_decode(uint8_t *raw,float *output);
void volt_gen(int16_t val0, int16_t val1, int16_t val2, int16_t val3,uint8_t *volt_val);

#endif
