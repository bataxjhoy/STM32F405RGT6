#include "recorder.h" 
#include "malloc.h"
#include "usart.h"
#include "wm8978.h"
#include "i2s.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "string.h" 

extern	u8 rec_sta;			
extern int16_t *i2srecbuf1;
extern int16_t *i2srecbuf2; 
extern int16_t EnCodeBufA[DMA_BUF_SIZE];
extern int16_t EnCodeBufB[DMA_BUF_SIZE];

extern int16_t *i2splaybuf1;
extern int16_t *i2splaybuf2;
extern u8 record_flag;
extern u8 play_flag;
//录音 I2S_DMA接收中断服务函数.在中断里面写入数据
void i2s_dma_rx_callback(void) 
{    
	u16 bw;
	u8 res;
	if(rec_sta==0X80)//录音模式
	{  
		if(DMA1_Stream0->CR&(1<<19))
		{
			//memcpy ( i2splaybuf1, i2srecbuf1, DMA_BUF_SIZE);//自录自放
			//memcpy ( &EnCodeBufA, &i2srecbuf1[0], DMA_BUF_SIZE);
			record_flag=0;
			
		}else
		{
			//memcpy ( i2splaybuf2, i2srecbuf2, DMA_BUF_SIZE);//自录自放
			//memcpy ( &EnCodeBufB, &i2srecbuf2[0], DMA_BUF_SIZE );
			record_flag=1;
		}
	}
	else record_flag=-1;
}  

//WAV播放时,I2S DMA传输回调函数
void i2s_dma_tx_callback(void) 
{   
	u16 i;
	if(DMA1_Stream7->CR&(1<<19))
	{
		play_flag=1;
	}else 
	{
		play_flag=0;
	}
} 




//进入PCM 录音模式 		  
void recoder_enter_rec_mode(void)
{
	WM8978_ADDA_Cfg(1,1);		//开启ADC
	//WM8978_Input_Cfg(1,1,0);	//开启输入通道(MIC&LINE IN)按键控制
	WM8978_Output_Cfg(1,0);		//未开启BYPASS输出 
	WM8978_MIC_Gain(55);		//MIC增益设置 
	
	
	WM8978_I2S_Cfg(2,0);		//飞利浦标准,16位数据长度
	I2S3_Init(I2S_Standard_Phillips,I2S_Mode_MasterTx,I2S_CPOL_Low,I2S_DataFormat_16b);			//飞利浦标准,主机发送,时钟低电平有效,16位帧长度 
	I2S3ext_Init(I2S_Standard_Phillips,I2S_Mode_SlaveRx,I2S_CPOL_Low,I2S_DataFormat_16b);		//飞利浦标准,从机接收,时钟低电平有效,16位帧长度	
	I2S3_SampleRate_Set(22050);	//设置采样率 
 	I2S3_TX_DMA_Init( ); 		//配置TX DMA 
	//DMA1_Stream7->CR&=~(1<<7);	//关闭传输完成中断(这里不用中断送数据) 
	I2S3ext_RX_DMA_Init( ); 	//配置RX DMA
	i2s_rx_callback=i2s_dma_rx_callback;//回调函数指wav_i2s_dma_callback
	i2s_tx_callback=i2s_dma_tx_callback;			//回调函数指wav_i2s_dma_callback
 	
	I2S_Rec_Start(); 	//开始I2S数据接收(从机)
	I2S_Play_Start();	//开始I2S数据发送(主机)
}  



























