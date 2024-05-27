#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "key.h"
#include "led.h"
#include "wm8978.h"
#include "recorder.h" 
#include "malloc.h"
#include "timer.h"	
#include "speex.h"
#include "speex_preprocess.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "i2s.h"  
#include "w25qxx.h" 
//FreeRTOS related header files
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "MemQueue.h"
#include "lora.h"
#include "radio.h"
//#define DENOISE_VAD
#include "voice.h"

extern pt_LORA_RADIO lora_radio;
//Task handle     //任务句柄
TaskHandle_t StartTask_Handler;
//Task function   //任务函数
void start_task(void *pvParameters);
void SendData_task(void *pvParameters);
void RecData_task(void *pvParameters);

int16_t EnCodeBufA[DMA_BUF_SIZE];
int16_t EnCodeBufB[DMA_BUF_SIZE];
int16_t DeCodeBufA[DMA_BUF_SIZE];
int16_t DeCodeBufB[DMA_BUF_SIZE];
int16_t* i2srecbuf1 ;
int16_t* i2srecbuf2 ; 
int16_t* i2splaybuf1 ;
int16_t* i2splaybuf2 ;
u8 rec_sta=0;		//录音状态
					//[7]:0,没有开启录音;1,已经开启录音;
					//[6:1]:保留
					//[0]:0,正在录音;1,暂停录音;
u8 record_flag=-1;//缓存标志	
u8 recNoDataTimes=0;
u8 recNoDataTimesThreshold=50;
#define ENCODED_FRAME_SIZE      20
#define QUEUEMINSIZE  40
	SpeexPreprocessState * m_st; 
SpeexBits bitsEncode;  
SpeexBits bitsDecode; 
void *enc_state, *dec_state;/*维持编码解码状态*/
int quality = 4;//质量
int complexity=1;//复杂度
int vbr=0;//可变比特率
int enh=1;//启用后置滤波

unsigned char OutBytes[20];
unsigned char InBytes[20];

u8 play_flag=-1;//缓存标志
u8 playRunVoice=0;
u32 baseAddrFlash=0*1024*1024;
u8 readBuf[320];
int voiceCount=0;
void Speex_Init(void)
{	
	#ifdef DENOISE_VAD	
		int denoise = 0, noiseSuppress = -90; /*noiseSuppress Preprocess*/
		int vad = 1, vadProbStart = 88, vadProbContinue = 100; 
		m_st=speex_preprocess_state_init(160, 8000); 
		speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_DENOISE, &denoise); //降噪 开关
		//speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noiseSuppress); //设置噪声dB

		speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_VAD, &vad); //静音检测  
		speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_PROB_START , &vadProbStart); //设置VAD从静音到声音的概率 //Set probability required for the VAD to go from silence to voice   
		speex_preprocess_ctl(m_st, SPEEX_PREPROCESS_SET_PROB_CONTINUE, &vadProbContinue); //保持语言的概率//Set probability required for the VAD to stay in the voice state (integer percent) 
	#endif
  /* 编码初始化 */ 
  speex_bits_init(&bitsEncode);	
	
	enc_state = speex_encoder_init(&speex_nb_mode);
  speex_encoder_ctl(enc_state, SPEEX_SET_VBR, &vbr);
  speex_encoder_ctl(enc_state, SPEEX_SET_QUALITY,&quality);
  speex_encoder_ctl(enc_state, SPEEX_SET_COMPLEXITY, &complexity);
  /* 解码初始化 */	
	speex_bits_init(&bitsDecode);
	dec_state = speex_decoder_init(&speex_nb_mode);
  speex_decoder_ctl(dec_state, SPEEX_SET_ENH, &enh);
}
int volume_adjust(short  * in_buf, short  * out_buf, float in_vol)
{
    int  tmp;
    tmp = (*in_buf)*in_vol; // 乘以倍数

    // 溢出判断
    if(tmp > 32767)
        tmp = 32767;
    else if(tmp < -32768)
        tmp = -32768;
    *out_buf = tmp;

    return 0;
}

void DecodeA(void)
{	
		speex_bits_read_from(&bitsDecode, (char *)InBytes, ENCODED_FRAME_SIZE);
		speex_decode_int(dec_state, &bitsDecode, (spx_int16_t*)i2splaybuf1);
//		for(int i=0;i<160;i++)
//		{
//			volume_adjust(&i2splaybuf1[i], &i2splaybuf1[i], 2);
//		}
}


void DecodeB(void)
{
		speex_bits_read_from(&bitsDecode, (char *)InBytes, ENCODED_FRAME_SIZE);
		speex_decode_int(dec_state, &bitsDecode, (spx_int16_t*)i2splaybuf2);
//		for(int i=0;i<160;i++)
//		{
//			volume_adjust(&i2splaybuf2[i], &i2splaybuf2[i], 2);
//		}
}



double sum = 0.0;int db = 0;
int stay_times=0;//保持次数
u16 threshold=1500;  
u8 IsValidVoice(short  * in_buf, size_t size)
{
		sum = 0.0;
		
		for (int i = 0; i < size/4; i++)
		{
			sum += abs(in_buf[i]);  // 取size/4 个绝对值求和
		}
		sum = sum / (size/4) ;  // 平均值

		if (sum > threshold||stay_times>0)//振幅超过，保持10次
		{		
			if(sum > threshold-100)
				stay_times=20;
			else
				stay_times--;
			return 1;
		}
		else
		{			
			return 0;
		}
}


void EncodeA(void)
{
		if(IsValidVoice(i2srecbuf1, 160))
		//if(speex_preprocess_run(m_st, (spx_int16_t*)&i2srecbuf1[0]))//打开静音检测和降噪 
		{
			/* Flush all the bits in the struct so we can encode a new frame */
			speex_bits_reset(&bitsEncode);
			/* Encode the frame */
			//speex_encode_int(enc_state, (spx_int16_t*)EnCodeBufA, &bitsEncode);
			speex_encode_int(enc_state, (spx_int16_t*)&i2srecbuf1[0], &bitsEncode);
			/* Copy the bits to an array of char that can be decoded */
			speex_bits_write(&bitsEncode, (char *)OutBytes, ENCODED_FRAME_SIZE);
			if( recNoDataTimes==recNoDataTimesThreshold)
				lora_radio->SetTxPacket(OutBytes,ENCODED_FRAME_SIZE);//传输音频	
			//自压自解播放测试
//		speex_bits_read_from(&bitsDecode, (char *)OutBytes, ENCODED_FRAME_SIZE);
//		speex_decode_int(dec_state, &bitsDecode, (spx_int16_t*)i2splaybuf1);
//		for(int i=0;i<160;i++)
//		{
//			volume_adjust(&i2splaybuf1[i], &i2splaybuf1[i], 40);
//		}
		}
}

void EncodeB(void)
{
		if(IsValidVoice(i2srecbuf2, 160))
		//if(speex_preprocess_run(m_st, (spx_int16_t*)&i2srecbuf1[0]))//打开静音检测和降噪 
		{
			/* Flush all the bits in the struct so we can encode a new frame */
			speex_bits_reset(&bitsEncode);
			/* Encode the frame */
			//speex_encode_int(enc_state, (spx_int16_t*)EnCodeBufB, &bitsEncode);
			speex_encode_int(enc_state, (spx_int16_t*)&i2srecbuf2[0], &bitsEncode);
			/* Copy the bits to an array of char that can be decoded */
			speex_bits_write(&bitsEncode, (char *)OutBytes, ENCODED_FRAME_SIZE);
			if( recNoDataTimes==recNoDataTimesThreshold)
				lora_radio->SetTxPacket(OutBytes,ENCODED_FRAME_SIZE);//传输音频	
		
		//自压自解播放
//		speex_bits_read_from(&bitsDecode, (char *)OutBytes, ENCODED_FRAME_SIZE);
//		speex_decode_int(dec_state, &bitsDecode, (spx_int16_t*)i2splaybuf2);
//		for(int i=0;i<160;i++)
//		{
//			volume_adjust(&i2splaybuf2[i], &i2splaybuf2[i], 40);
//		}
		}
}


int main(void){
	
	u8 rval=0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);			//延时初始化
 
	uart_init(115200);		//初始化串口1波特率为115200 
	KEY_Init();
	LED_Init();					//初始化LED	
	
	POWER_ON=1;//打开电源控制 优先上电
	delay_ms(500);
	
	TIM3_Int_Init(999,83);//1ms定时器	
 //射频模块初始化
	lora_Register();
	W25QXX_Init();
	if(W25QXX_ReadID()==W25Q64)								//检测到W25Q64
	{
		printf("welcom to use!\n");		
	}
	
	
	WM8978_Init();				//初始化WM8978
	WM8978_HPvol_Set(40,40);	//耳机音量设置
	WM8978_SPKvol_Set(63);		//喇叭音量设置
	my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMCCM);		//初始化CCM内存池 
	Speex_Init();
	
	i2srecbuf1=mymalloc(SRAMIN,DMA_BUF_SIZE);//I2S录音内存1申请
	i2srecbuf2=mymalloc(SRAMIN,DMA_BUF_SIZE);//I2S录音内存2申请  
			
		i2splaybuf1 = mymalloc(SRAMIN,DMA_BUF_SIZE);
		i2splaybuf2 =mymalloc(SRAMIN,DMA_BUF_SIZE);
		if(!i2srecbuf1||!i2srecbuf2||!i2splaybuf1||!i2splaybuf2)
			rval=1; 
		if(rval==0)		
		{
			recoder_enter_rec_mode();	//进入录音模式
			playRunVoice=1;//播放提示音
			baseAddrFlash=0*512*1024;
		}
	//Create the start task 
    xTaskCreate((TaskFunction_t )start_task,            //Task function   
                            (const char*    )"start_task",  //Task name     
                            (uint16_t       )256,        //Task stack size 
                            (void*          )NULL,       //Arguments passed to the task function //??????????
                            (UBaseType_t    )3,       //Task priority   
                            (TaskHandle_t*  )&StartTask_Handler);   //Task handle                         
    vTaskStartScheduler();  //Enables task scheduling
													
}
//Start task task function 
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL(); //Enter the critical area    
    
		xTaskCreate(SendData_task, "SendData_task", 256, NULL, 4, NULL);    //
		xTaskCreate(RecData_task, "RecData_task", 512, NULL, 4, NULL);   
    vTaskDelete(StartTask_Handler); //Delete the start task 
    taskEXIT_CRITICAL();            //Exit the critical section
}

void SendData_task(void *pvParameters)
{
		u8 key;			
		u32 lastWakeTime = getSysTickCnt();
		while(1){
			
			vTaskDelayUntil(&lastWakeTime, F2T(1000)); //100Hz
			key=KEY_Scan(0);
			switch(key)
			{						
				case S1_PRES:	//REC/PAUSE
					if(rec_sta&0X01)//原来是暂停,继续录音
					{
						rec_sta&=0XFE;//取消暂停
						WM8978_Input_Cfg(1,1,0);	
					}else if(rec_sta&0X80)//已经在录音了,暂停
					{
						rec_sta|=0X01;	//暂停
						WM8978_Input_Cfg(0,0,0);						
						//自录自放清空放音
						memset ( &i2splaybuf1[0], 0, DMA_BUF_SIZE);
						memset ( &i2splaybuf2[0], 0, DMA_BUF_SIZE);						
					}else				//还没开始录音 
					{						
						rec_sta|=0X80;	//开始录音	
						WM8978_Input_Cfg(1,1,0);	
					}
					if(rec_sta&0X01)LED1=1;	//提示正在暂停
					else LED1=0;					
				break;  
			}
			
			switch(record_flag)
			{
					case 0:		
						record_flag=-1;
						EncodeA();
						
						break;
					case 1: 	
						record_flag=-1;	
						EncodeB();    
						break;
					default:
						break;
			}
			
		}	
		myfree(SRAMIN,i2srecbuf1);	//释放内存
		myfree(SRAMIN,i2srecbuf2);	//释放内存  
	
}
void RecData_task(void *pvParameters)
{	
	u8 len;
	int i=0;
	u8 playbufFlag=0;
	u32 lastWakeTime = getSysTickCnt();
	while(1){		
			
		vTaskDelayUntil(&lastWakeTime, F2T(1000)); //100Hz
		if(playRunVoice==0)
		{
			switch(lora_radio->Process())
			{
				case IDLE:		
					if(recNoDataTimes<recNoDataTimesThreshold)//无数据接收
					{
						recNoDataTimes++;
						if(recNoDataTimes==recNoDataTimesThreshold)
						{
								memset ( &i2splaybuf1[0], 0, DMA_BUF_SIZE);
								memset ( &i2splaybuf2[0], 0, DMA_BUF_SIZE);
						}
					}
				break;		
				case RX_DONE:
					lora_radio->GetRxPacket(InBytes,&len);	  //接收无线数据	
					if(len==20)							
					{	
						recNoDataTimes=0;
						if(playbufFlag==0)
						{	
							playbufFlag=1;
							DecodeA();
						}					
						else
						{	
							playbufFlag=0;
							DecodeB();
						}		
					}
				break;
				case TIMEOUT:
					lora_radio->StartRx();
					break;
				default:
					break;
			}
		}
		
		
		else if(playRunVoice==1&&play_flag==1)
		{						
				//读出音频
				W25QXX_Read(readBuf,baseAddrFlash+voiceCount*DMA_BUF_SIZE,DMA_BUF_SIZE);
				for( i=0;i<DMA_BUF_SIZE/2;i++)
				{
					i2splaybuf1[i]=readBuf[i*2] | (readBuf[i*2+1]<<8);//buf1 小端
				}
				play_flag=-1;
				voiceCount++;					
		}
		else if(playRunVoice==1&&play_flag==0)
		{				
				W25QXX_Read(readBuf,baseAddrFlash+voiceCount*DMA_BUF_SIZE,DMA_BUF_SIZE);
				for( i=0;i<DMA_BUF_SIZE/2;i++)
				{
					i2splaybuf2[i]=readBuf[i*2]| (readBuf[i*2+1]<<8) ;//buf2
				}		
				play_flag=-1;	
				voiceCount++;						
		}
		
		if(voiceCount==100*1024/DMA_BUF_SIZE)
		{
			playRunVoice=0;//停止播放
			voiceCount=0;
			I2S3_SampleRate_Set(8000);	//设置采样率
		}
		//vTaskDelay(15);
		//delay_us(1);
	}
		myfree(SRAMIN,i2splaybuf1);	//释放内存
		myfree(SRAMIN,i2splaybuf2);	//释放内存 
}




void _speex_putc(int ch, void *file)
{
  while(1)
  {
  };
}



void _speex_fatal(const char *str, const char *file, int line)
{
  while(1)
  {
  };
}


