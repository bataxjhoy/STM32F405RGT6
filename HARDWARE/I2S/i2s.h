#ifndef __I2S_H
#define __I2S_H
#include "sys.h"    									
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32F407������
//I2S ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/24
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//********************************************************************************
//V1.1 20140606
//����I2S2ext_Init��I2S2ext_RX_DMA_Init��I2S_Rec_Start��I2S_Rec_Stop�Ⱥ���
////////////////////////////////////////////////////////////////////////////////// 	
 
#define DMA_BUF_SIZE    	320		//����RX DMA �����С

extern void (*i2s_tx_callback)(void);		//IIS TX�ص�����ָ�� 
extern void (*i2s_rx_callback)(void);		//IIS RX�ص�����ָ�� 

void I2S3_Init(u16 I2S_Standard,u16 I2S_Mode,u16 I2S_Clock_Polarity,u16 I2S_DataFormat);
void I2S3ext_Init(u16 I2S_Standard,u16 I2S_Mode,u16 I2S_Clock_Polarity,u16 I2S_DataFormat);
u8 I2S3_SampleRate_Set(u32 samplerate);
void I2S3_TX_DMA_Init( );
void I2S3ext_RX_DMA_Init( );
void I2S_Play_Start(void);
void I2S_Rec_Start(void);
void I2S_Play_Stop(void);
void I2S_Rec_Stop(void);
 

#endif




















