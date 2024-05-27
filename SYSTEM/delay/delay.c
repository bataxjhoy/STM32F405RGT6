#include "delay.h"
#include "sys.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS  
#include "FreeRTOS.h"    //The header file used by FreeRTOS //FreeRTOS??????          
#include "task.h"     //The header file used by FreeRTOS //FreeRTOS??????  
#endif
//////////////////////////////////////////////////////////////////////////////////  
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//使用SysTick的普通计数模式对延迟进行管理(支持ucosii)
//包括delay_us,delay_ms
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/5/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//********************************************************************************
//修改说明
//无
////////////////////////////////////////////////////////////////////////////////// 
 
static u8  fac_us=0;//us延时倍乘数			   
static u16 fac_ms=0;//ms延时倍乘数,在ucos下,代表每个节拍的ms数

static u32 sysTickCnt=0;    
extern void xPortSysTickHandler(void);

			   
//初始化延迟函数
//当使用ucos的时候,此函数会初始化ucos的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟

void delay_init(u8 SYSCLK)
{

	u32 reload;
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=SYSCLK/8;		//不论是否使用ucos,fac_us都需要使用
	  
	reload=SYSCLK/8;		//每秒钟的计数次数 单位为K	   
	reload*=1000000/configTICK_RATE_HZ;//根据OS_TICKS_PER_SEC设定溢出时间
							//reload为24位寄存器,最大值:16777216,在168M下,约合0.7989s左右	
	fac_ms=1000/configTICK_RATE_HZ;//代表ucos可以延时的最少单位	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 	//每1/OS_TICKS_PER_SEC秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK

}								    


//延时nus
//nus:要延时的us数.		    								   
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



































