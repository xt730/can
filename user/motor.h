#ifndef _motor_H_
#define _motor_H_

#include "main.h"
#include "can.h"

static uint16_t motor_data[4];

void can_filter_init(void);
void Motor_send(uint8_t ID,uint8_t len,uint8_t *TxData);
void data_decode(uint8_t *raw,uint16_t *output);

#endif
