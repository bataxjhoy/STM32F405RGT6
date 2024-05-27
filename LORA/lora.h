#ifndef __LORA_H
#define __LORA_H


#define LORA_PROCESS_PRIO			4
#define LORA_PROCESS_STK			128

#define LORA_POLLING_PRIO			3
#define LORA_POLLING_STK			128


typedef enum{
	LORA_OK,
	LORA_ERROR,
}LORA_STATUS;


typedef struct lora_radio{
	void (*Init)(void);
    void (*Reset )(void);
    void (*StartRx )(void);
    void (*GetRxPacket)(unsigned char *buffer, unsigned char *size);
    void (*SetTxPacket)(unsigned char *buffer, unsigned char size);
    unsigned short (*Process)(void);
}t_LORA_RADIO,*pt_LORA_RADIO;

LORA_STATUS lora_Register(void);
LORA_STATUS lora_Unregister(void);

#endif


