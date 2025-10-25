#include "motor.h"



void can_filter_init(void)
{
	CAN_FilterTypeDef sFilterConfig;

	sFilterConfig.FilterBank = 0;                      
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;  
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT; 
	sFilterConfig.FilterIdHigh = 0x0000;               
	sFilterConfig.FilterIdLow = 0x0000;                
	sFilterConfig.FilterMaskIdHigh = 0x0000;           
	sFilterConfig.FilterMaskIdLow = 0x0000;            
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0; 
	sFilterConfig.FilterActivation = ENABLE;           
	sFilterConfig.SlaveStartFilterBank = 14;           

	if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
	{
			Error_Handler();
	}
	}

void Motor_send(uint8_t ID,uint8_t len,uint8_t *TxData)
{
	CAN_TxHeaderTypeDef Txhead;
	uint32_t TxMailbox;
	Txhead.StdId=ID;
	Txhead.ExtId=0x00;
	Txhead.IDE=CAN_ID_STD;
	Txhead.RTR=CAN_RTR_DATA;
	Txhead.DLC=len;
	Txhead.TransmitGlobalTime= DISABLE;
	HAL_CAN_AddTxMessage(&hcan1, &Txhead, TxData, &TxMailbox);
	
}

void data_decode(uint8_t *raw,uint16_t *output)
{
	output[0]=(raw[1]<<8)+raw[0];
	output[1]=(raw[3]<<8)+raw[2];
	output[2]=(raw[5]<<8)+raw[4];
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
