#include "delay.h"
#include "sys.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS  
#include "FreeRTOS.h"    //The header file used by FreeRTOS //FreeRTOS??????          
#include "task.h"     //The header file used by FreeRTOS //FreeRTOS??????  
#endif
//////////////////////////////////////////////////////////////////////////////////  
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���(֧��ucosii)
//����delay_us,delay_ms
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//********************************************************************************
//�޸�˵��
//��
////////////////////////////////////////////////////////////////////////////////// 
 
static u8  fac_us=0;//us��ʱ������			   
static u16 fac_ms=0;//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��

static u32 sysTickCnt=0;    
extern void xPortSysTickHandler(void);

			   
//��ʼ���ӳٺ���
//��ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��

void delay_init(u8 SYSCLK)
{

	u32 reload;
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=SYSCLK/8;		//�����Ƿ�ʹ��ucos,fac_us����Ҫʹ��
	  
	reload=SYSCLK/8;		//ÿ���ӵļ������� ��λΪK	   
	reload*=1000000/configTICK_RATE_HZ;//����OS_TICKS_PER_SEC�趨���ʱ��
							//reloadΪ24λ�Ĵ���,���ֵ:16777216,��168M��,Լ��0.7989s����	
	fac_ms=1000/configTICK_RATE_HZ;//����ucos������ʱ�����ٵ�λ	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 	//ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK

}								    


//��ʱnus
//nus:Ҫ��ʱ��us��.		    								   
void delay_us(u32 nus)
{        
    u32 ticks;
    u32 told,tnow,tcnt=0;
    u32 reload=SysTick->LOAD;    //The value of the LOAD //LOAD??             
    ticks=nus*fac_us;                 //The number of beats required //?????? 
    told=SysTick->VAL;        //The value of the counter on entry //?????????
    while(1)
    {
        tnow=SysTick->VAL;    
        if(tnow!=told)
        {        
            if(tnow<told)tcnt+=told-tnow;    //SysStick is a decrement counter //??????SYSTICK?????????????
            else tcnt+=reload-tnow+told;        
            told=tnow;
            if(tcnt>=ticks)break;            //If the delay time exceeds or equals the delay time, exit //??????????????,???
        }  
    };                                            
}  
void delay_ms(u32 nms)
{    
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED) //The system is running //??????
    {        
        if(nms>=fac_ms)    //The duration of the delay is greater than the OS minimum duration //???????OS??????? 
        { 
               vTaskDelay(nms/fac_ms);    //FreeRTOS latency //FreeRTOS??
        }
        nms%=fac_ms;    //OS is no longer able to provide such a small delay, use normal delay //OS?????????????,????????    
    }
    delay_us((u32)(nms*1000));    //Normal mode delay    //??????
}
/**************************************************************************
Function: Tick timer interrupt service function
Input   : Duration of delay
Output  : none
**************************************************************************/  
void SysTick_Handler(void)
{    
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED) //The system is running //??????
    {
        xPortSysTickHandler();    
    }
    else
    {
        sysTickCnt++;    //Counting before scheduling is enabled //????????
    }
}

/**************************************************************************
Function: Get the time of the last execution function
Input   : none
Output  : The last time the program was executed

**************************************************************************/ 
u32 getSysTickCnt(void)
{
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)    //The system is running //??????
        return xTaskGetTickCount();
    else
        return sysTickCnt;
}



































