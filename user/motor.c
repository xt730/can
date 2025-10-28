#include "motor.h"



void can_filter_init(void)
{
	 CAN_FilterTypeDef can1_filter_st;
	
    can1_filter_st.FilterIdHigh = 0x0000;
    can1_filter_st.FilterIdLow = 0x0000;
    can1_filter_st.FilterMaskIdHigh = 0x0000;
    can1_filter_st.FilterMaskIdLow = 0x0000;
    can1_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
    can1_filter_st.FilterActivation = ENABLE;
    can1_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can1_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can1_filter_st.FilterBank = 0;
    can1_filter_st.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(&hcan1, &can1_filter_st) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_CAN_Start(&hcan1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        Error_Handler();
    }
}

void Motor_send(uint16_t ID, uint8_t len, uint8_t *TxData)
{
    CAN_TxHeaderTypeDef TxHead;
    uint32_t TxMailbox;

    TxHead.StdId = ID;
    TxHead.ExtId = 0x00;
    TxHead.IDE = CAN_ID_STD;
    TxHead.RTR = CAN_RTR_DATA;
    TxHead.DLC = len;
    TxHead.TransmitGlobalTime = DISABLE;

    if (HAL_CAN_AddTxMessage(&hcan1, &TxHead, TxData, &TxMailbox) != HAL_OK)
    {
        Error_Handler();
    }
}

void data_decode(uint8_t *raw,float *output)
{
	output[0]=(int16_t)((raw[0]<<8)|raw[1]);
	output[0]=output[0];
	output[1]=(int16_t)((raw[2]<<8)|raw[3]);
	output[1]=output[1];
	output[2]=(int16_t)((raw[4]<<8)|raw[5]);
	output[3]=raw[6];
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rxhead;
	uint8_t rcvd[8];
	if(hcan==&hcan1)
	{
		HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&rxhead,rcvd);
		data_decode(rcvd,motor_data);
		if(rxhead.StdId==0x205)
		{
			data_decode(rcvd,motor_data);
		}
	}
}

void volt_gen(int16_t val0, int16_t val1, int16_t val2, int16_t val3,uint8_t *volt_val)
{
    volt_val[0] = (uint8_t)(val0 >> 8);
    volt_val[1] = (uint8_t)(val0 & 0xFF);
    volt_val[2] = (uint8_t)(val1 >> 8);
    volt_val[3] = (uint8_t)(val1 & 0xFF);
    volt_val[4] = (uint8_t)(val2 >> 8);
    volt_val[5] = (uint8_t)(val2 & 0xFF);
    volt_val[6] = (uint8_t)(val3 >> 8);
    volt_val[7] = (uint8_t)(val3 & 0xFF);
}
