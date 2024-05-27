#ifndef _MEMQUEUE_H_
#define _MEMQUEUE_H_


#define BufSize   200
typedef unsigned char   INT8U;
typedef signed   char   INT8S;
typedef unsigned short  INT16U;
typedef signed   short  INT16S;
typedef unsigned int    INT32U;
typedef signed   int    INT32S;
typedef          int    INT32;
typedef struct _SqQueue
{
	INT8U  gBuf[BufSize];

	INT16U  sFront;
	INT16U  sRear;	
}SqQueue;

typedef enum {FALSE = 0, TRUE = !FALSE} SqQueueSta;



extern SqQueue RxQueue;
extern SqQueue TxQueue;


extern void InitQueue(SqQueue *Q);
extern INT16U QueueLenth(SqQueue *Q);
extern SqQueueSta EnQueue(SqQueue *Q, INT8U Data);
extern SqQueueSta DeQueue(SqQueue *Q, INT8U *Data);
extern SqQueueSta EnQueueStr(SqQueue *Q, INT8U *Data,INT16U No);
extern SqQueueSta DeQueueStr(SqQueue *Q, INT8U *Data,INT16U No);

#endif 

