#ifndef __SX126X_HAL_H
#define __SX126X_HAL_H

#define BOARD_F405


#ifdef BOARD_F405
#define SPI_INTERFACE                               SPI1
#define SPI_CLK                                     RCC_APB2Periph_SPI1

#define SPI_PIN_SCK_PORT                            GPIOA
#define SPI_PIN_SCK_PORT_CLK                        RCC_AHB1Periph_GPIOA
#define SPI_PIN_SCK                                 GPIO_Pin_5
#define SPI_PIN_SCK_AF_SOURCE                       GPIO_PinSource5
#define SPI_PIN_SCK_AF                              GPIO_AF_SPI1

#define SPI_PIN_MISO_PORT                           GPIOA
#define SPI_PIN_MISO_PORT_CLK                       RCC_AHB1Periph_GPIOA
#define SPI_PIN_MISO                                GPIO_Pin_6
#define SPI_PIN_MISO_AF_SOURCE                      GPIO_PinSource6
#define SPI_PIN_MISO_AF                             GPIO_AF_SPI1

#define SPI_PIN_MOSI_PORT                           GPIOA
#define SPI_PIN_MOSI_PORT_CLK                       RCC_AHB1Periph_GPIOA
#define SPI_PIN_MOSI                                GPIO_Pin_7
#define SPI_PIN_MOSI_AF_SOURCE                      GPIO_PinSource7
#define SPI_PIN_MOSI_AF                             GPIO_AF_SPI1


#define RESET_IOPORT                                GPIOA
#define RESET_PIN                                   GPIO_Pin_1

#define NSS_IOPORT                                  GPIOA
#define NSS_PIN                                     GPIO_Pin_4

#define BUSY_IOPORT                                 GPIOA
#define BUSY_PIN                                    GPIO_Pin_0

#define DIO1_IOPORT                                 GPIOB
#define DIO1_PIN                                    GPIO_Pin_8
//#define DIO3_IOPORT                                 GPIOB
//#define DIO3_PIN                                    GPIO_Pin_9
#define TXEN_IOPORT                                 GPIOA
#define TXEN_PIN                                    GPIO_Pin_2
#define RXEN_IOPORT                                 GPIOA
#define RXEN_PIN                                    GPIO_Pin_3
#endif

//#define SX126X_HAL_DEBUG  
typedef enum 
{
	SET_SLEEP					= 0x84, 		
	SET_STANDBY 				= 0x80, 		
	SET_FS						= 0xC1, 		
	SET_TX						= 0x83, 		
	SET_RX						= 0x82,		
	STOP_TIMER_ON_PREAMBLE 		= 0x9F, 		
	SET_RX_DUTY_CYCLE			= 0x94, 	
	SET_CAD						= 0xC5,	
	SET_TX_CONTINUOUS_WAV		= 0xD1,  	
	SET_TX_INFINITE_PREAMBLE	= 0xD2,
	SET_REGULATOR_MODE			= 0x96,

	CALIBRATE					= 0x89,		
	CALIBRATE_IMAGE				= 0x98,		
	SET_PA_CONFIG				= 0x95,		
	SET_RXTX_FALLBACK_MODE		= 0x93,	
	WRITE_REGISTER				= 0x0D,	
	READ_REGISTER				= 0x1D,
	WRITE_BUFFER				= 0x0E,	
	READ_BUFFER 				= 0x1E,	


	SET_DIO_IRQ_PARAMS			= 0x08,		

	GET_IRQ_STATUS				= 0x12,		
	CLEAR_IRQ_STATUS			= 0x02,		
	SET_DIO2_AS_RFSWITCH_CTRL	= 0x9D,		
	SET_DIO_AS_TCXOCTRL			= 0x97, 	


	SET_RF_FREQUENCY			= 0x86, 		
	SET_PACKET_TYPE				= 0x8A,
	GET_PACKET_TYPE				= 0x11,
	SET_TX_PARAMS				= 0x8E,		
	SET_MODULATION_PARAMS		= 0x8B,		

	SET_PACKET_PARAMS			= 0x8C,

	SET_CAD_PARAMS				= 0x88,


	SET_BUFFER_BASE_ADDRESS 	= 0x8F, 		
	SET_LORA_SYMBNUM_TIMEOUT 	= 0xA0,

	GET_STATUS					= 0xC0,		
	GET_RSSI_INST				= 0x15,	
	GET_RX_BUFFER_STATUS		= 0x13,		
	GET_PACKET_STATUS			= 0x14,		
	GET_DEVICE_EERORS			= 0x17,		
	CLEAR_DEVICE_ERRORS			= 0x07,		
	GET_STATS					= 0x10,		
	RESET_STATS					= 0x00,		
}OPCODE;

typedef enum{

	SyncWordMSB			= 0x0740,
	SyncWordLSB			= 0x0741,
	RandomNumberGen0 	= 0x0819,
	RandomNumberGen1 	= 0x081A,
	RandomNumberGen2	= 0x081B,
	RandomNumberGen3	= 0x081C,
	OCP					= 0x08E7,
	XTA					= 0x0911,
	XTB					= 0x0912,
	
}REG_ADDRESS;

typedef enum{

	CS_LOW = 0x0,
	CS_HIGH = 0x1,
}CS_STATUS;

#define WAIT_FOR_WHILE()          for( int counter = 0; counter < 1024; counter++ ) \
                                  {  __NOP( ); }


void sx126x_HwInit(void);
								  
void sx126x_Reset(void);

void sx126x_WriteCmd(OPCODE opcode ,unsigned char *buf,unsigned char len);

void sx126x_ReadCmd(OPCODE opcode ,unsigned char *buf,unsigned char len);

void sx126x_WriteRegister(unsigned short address ,unsigned char *buf,unsigned char len);

void sx126x_ReadRegister(unsigned short address ,unsigned char *buf,unsigned char len);

void sx126x_WriteRegisterByte(unsigned short address ,unsigned char val);

unsigned char sx126x_ReadRegisterByte(unsigned short address );
								  
void sx126x_WriteBuffer(unsigned char offset,unsigned char *buf,unsigned char len);

void sx126x_ReadBuffer(unsigned char offset,unsigned char *buf,unsigned char len);
#endif



