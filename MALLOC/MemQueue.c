/**
  ******************************************************************************
  * @file    MemQueue.C
  * @author  Luoxianhui R&D Driver Software Team
  * @version V1.0.0
  * @date    06/09/2013
  * @brief   PWMÊä³ö¿ØÖÆ
  ******************************************************************************
**/
  
/* Includes ------------------------------------------------------------------*/
#include "MemQueue.h"

/** @addtogroup MemQueue_Driver
  * @{
  */

/** @addtogroup Mem
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SqQueue RxQueue;
SqQueue TxQueue;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void InitQueue(SqQueue *Q);
INT16U QueueLenth(SqQueue *Q);
SqQueueSta EnQueue(SqQueue *Q, INT8U Data);
SqQueueSta DeQueue(SqQueue *Q, INT8U *Data);
SqQueueSta EnQueueStr(SqQueue *Q, INT8U *Data,INT16U No);
SqQueueSta DeQueueStr(SqQueue *Q, INT8U *Data,INT16U No);

void InitQueue(SqQueue *Q)
{
	Q->sRear  = 0;
	Q->sFront = 0;
}


INT16U QueueLenth(SqQueue *Q)
{
	return(((Q->sRear) - (Q->sFront) + BufSize) % BufSize); 
}


SqQueueSta EnQueue(SqQueue *Q, INT8U Data)
{
	if((((Q->sRear)+1) % BufSize) ==  (Q->sFront))  return FALSE;
	(*Q).gBuf[Q->sRear] = Data;
	Q->sRear = (Q->sRear + 1) % BufSize;
	return TRUE;
}


SqQueueSta DeQueue(SqQueue *Q, INT8U *Data)
{
	if(Q->sRear == Q->sFront) return FALSE;
	
	*Data = (*Q).gBuf[Q->sFront];
	Q->sFront = (Q->sFront + 1) % BufSize;
	return TRUE;
}

SqQueueSta EnQueueStr(SqQueue *Q, INT8U *Data,INT16U No)
{
	INT16U i = 0;
	if((QueueLenth(Q)) >= (BufSize-No)) return FALSE;	
	
	for(i=0; i<No; i++)
  {
		EnQueue(Q,Data[i]);
  }	
	return TRUE;
}

SqQueueSta DeQueueStr(SqQueue *Q, INT8U *Data,INT16U No)
{
	INT16U i = 0;
	if((QueueLenth(Q)) <= No) return FALSE;

	for(i=0; i<No; i++)
	{	
	  DeQueue(Q,&(Data[i]));
	}
	return TRUE;
}

