
#ifndef __RF_H__
#define	__RF_H__
#include "stm32f4xx.h"
extern const char *rfName;
extern u16	iSend, iRev;

extern u8	sendBuf[64];
extern u8	revBuf[64];

u8 RFSendData( u8 *buf, u8 size );
u8 RFRevData( u8 *buf );
void RFRxMode( void );
void RFInit( void );

#endif
