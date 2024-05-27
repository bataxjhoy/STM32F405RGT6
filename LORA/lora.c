#include "lora.h"
#include "FreeRTOS.h"
#include "task.h"
#include "radio.h"

extern t_RADIO_INST radio;
//#define TEST1

#define TEST2

#define PACKET_MAX  100

pt_LORA_RADIO lora_radio;

TaskHandle_t Handler_LoraProcess;
TaskHandle_t Handler_LoraPolling;

static LORA_STATUS lora_Init(void)
{
	lora_radio = pvPortMalloc(sizeof(t_LORA_RADIO));//ÉêÇë¿Õ¼ä
	if(!lora_radio)
		return LORA_ERROR;
	
	lora_radio->Init = radio_Init;
	lora_radio->Reset = radio_Reset;
	lora_radio->StartRx = radio_StartRx;
	lora_radio->SetTxPacket = radio_PackgetSend;
	lora_radio->GetRxPacket = radio_PackgetGet;
	lora_radio->Process = radio_Process;
	
	return LORA_OK;
}

static void lora_Remove(void)
{
	if(lora_radio)
		vPortFree(lora_radio);
}

static unsigned char tmp;

void data_cal(unsigned char *buf,unsigned char len)
{
	
	
	for (int i = 0; i < len;i++)
	{
		/*packet head check*/
		if(0 == i)
		{
			if(tmp!= buf[i])
			{
				//printf("error packet head or lost packet %x %x \r\n ",tmp,buf[i]);
				tmp = buf[i]+1;
			}
			else 
			{
				if(tmp == 255)
					tmp = 0;
				else
					tmp++;
			}
		}
		/*packet last data*/
		else if(i == (len-1))
		{
			if(buf[i] != PACKET_MAX)
				printf("error packet last data %x \r\n ",buf[i]);
		}
		else
		{
			if((buf[i+1] - buf[i]) != 1)
				printf("error data %x  %x\r\n",buf[i],buf[i+1]);
		}
	}
}

void LoraProcess(void *pvParameters)
{
	unsigned char buf[256];
	unsigned char len;
	unsigned char rx_flag = 0;
	while(1)
	{

		switch(lora_radio->Process())
		{
			case IDLE:
				break;
			case RX_DONE:
				lora_radio->GetRxPacket(buf,&len);
				
//				#ifdef USART_DEBUG
//					for(int i=0;i<len;i++)
//						printf("%x ",buf[i]);
//			
//					printf("\r\n");
//				#endif
//			#ifdef TEST1
//				data_cal(buf,len);
//				
//			#endif
//				#ifdef TEST2
//				data_cal(buf,len);
//				lora_radio->SetTxPacket(buf,len);
//				#endif
				break;
			case TIMEOUT:
				lora_radio->StartRx();
				break;
			default:
				break;
		}
			
		vTaskDelay(5);
	}
}

void LoraPolling(void *pvParameters)
{
	unsigned char poll_data[16] = {0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9,0xA,0xB,0xC,0xD,0xE,0xF};	

	while(1)
	{

		lora_radio->SetTxPacket(poll_data,sizeof(poll_data));
		
		vTaskDelay(5000);
	}
}


LORA_STATUS lora_Register(void)
{
	
	BaseType_t pTaskStatus;
	
	if(LORA_ERROR == lora_Init())
		goto lora_err_radio_init;
		
	radio_Init();
	radio_StartRx();
		
	
	
	return LORA_OK;

error_LoraProcess:
	if(Handler_LoraProcess)
		vTaskDelete(Handler_LoraProcess);
	
lora_err_radio_init:
	lora_Remove();
	
	return LORA_ERROR;
}


LORA_STATUS lora_Unregister(void)
{
//	if(Handler_LoraPolling)
//		vTaskDelete(Handler_LoraPolling);
	if(Handler_LoraProcess)
		vTaskDelete(Handler_LoraProcess);
	
	lora_Remove();
	return LORA_OK;

}


