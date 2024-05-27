#ifndef __RADIO_H
#define __RADIO_H
#include "sx126x_hal.h"
#include "sx126x.h"

typedef enum {
	
	IDLE 				= 0x0,
	TX_DONE				= 0x1,
	RX_DONE				= 0x2,
	PREAMBLE_DETECTED 	= 0x4,
	SYNCWORD_VALID 		= 0x8,
	HEADER_VALID 		= 0x10,
	HEADER_ERR 			= 0x20,
	CRC_ERR 			= 0x40,
	CAD_DONE 			= 0x80,
	CAD_DETECTED 		= 0x100,
	TIMEOUT 			= 0x200,
}RADIO_STATUS;


typedef struct radio_msg{

	unsigned char buf[256];
	unsigned char len;
	unsigned char rx_buffer_pointer;
}t_RADIO_MSG,*pt_RADIO_MSG;

typedef struct radio_timeout{

	unsigned int timeout_tx;
	unsigned int timeout_rx;
	
}t_RADIO_TIMEOUT,*pt_RADIO_TIMEOUT;

typedef struct radio_inst{

	char name[16];
	
	pt_RADIO_MSG		 	msg_rx;
	
	pt_RADIO_MSG 			msg_tx;

	pt_MODULATION_PARAMS 	modulation_params;
	
	pt_PACKET_PARAMS_LORA 	packet_params_lora;
	pt_PA_OPTIMAL_SETTINGS 	pa_optimal_setting;
	pt_PACKET_STATUS		packet_status;
	
	unsigned short status_running;
	unsigned char radio_irq_flag;
	unsigned short irq_mask;
	
}t_RADIO_INST,*pt_DADIO_INST;

void radio_Init(void);
void radio_Reset(void);
void radio_StartRx(void);
unsigned short radio_Process(void);
void radio_PackgetGet(unsigned char *buf,unsigned char *len);
void radio_PackgetSend(unsigned char *buf,unsigned char len);
void radio_exit(void);
#endif

