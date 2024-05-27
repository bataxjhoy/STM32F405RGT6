#include "sx126x_hal.h"
#include "stm32f4xx.h" 
#include "delay.h"
#include "math.h"
static void sx126x_SpiInit(void)
{
	
	SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    
    RCC_AHB1PeriphClockCmd( SPI_PIN_MISO_PORT_CLK | SPI_PIN_MOSI_PORT_CLK | SPI_PIN_SCK_PORT_CLK, ENABLE );
    RCC_APB2PeriphClockCmd( SPI_CLK, ENABLE );

    
    GPIO_PinAFConfig( SPI_PIN_SCK_PORT, SPI_PIN_SCK_AF_SOURCE, SPI_PIN_SCK_AF );
    GPIO_PinAFConfig( SPI_PIN_MOSI_PORT, SPI_PIN_MOSI_AF_SOURCE, SPI_PIN_MISO_AF );
    GPIO_PinAFConfig( SPI_PIN_MISO_PORT, SPI_PIN_MISO_AF_SOURCE, SPI_PIN_MOSI_AF );

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

    GPIO_InitStructure.GPIO_Pin = SPI_PIN_SCK;
    GPIO_Init( SPI_PIN_SCK_PORT, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = SPI_PIN_MOSI;
    GPIO_Init( SPI_PIN_MOSI_PORT, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = SPI_PIN_MISO;
    GPIO_Init( SPI_PIN_MISO_PORT, &GPIO_InitStructure );

    /* SPI_INTERFACE Config -------------------------------------------------------------*/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // 10 MHz
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init( SPI_INTERFACE, &SPI_InitStructure );
    SPI_Cmd( SPI_INTERFACE, ENABLE );
	
	#ifdef USART_DEBUG
	printf("%s \r\n",__FUNCTION__);
	#endif
}

static void sx126x_IoInit(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB| RCC_AHB1Periph_GPIOC, ENABLE );

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

	// Configure NSS as output

	GPIO_InitStructure.GPIO_Pin = NSS_PIN;
	GPIO_Init( NSS_IOPORT, &GPIO_InitStructure );
	GPIO_WriteBit( NSS_IOPORT, NSS_PIN, Bit_SET );

	// Configure RESET as output
	GPIO_InitStructure.GPIO_Pin = RESET_PIN;
	GPIO_Init(RESET_IOPORT, &GPIO_InitStructure );

	// Configure TXEN as output
	GPIO_InitStructure.GPIO_Pin = TXEN_PIN;
	GPIO_Init( TXEN_IOPORT, &GPIO_InitStructure );
	
	// Configure RXEN as output
	GPIO_InitStructure.GPIO_Pin = RXEN_PIN;
	GPIO_Init( RXEN_IOPORT, &GPIO_InitStructure );
	//GPIO_SetBits( RXEN_IOPORT, RXEN_PIN );
	// Configure radio DIO1 as inputs// Configure DIO1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin =  DIO1_PIN;
	GPIO_Init( DIO1_IOPORT, &GPIO_InitStructure );

	// Configure radio DIO2 as inputs// Configure DIO2
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Pin =  DIO2_PIN;
//	GPIO_Init( DIO2_IOPORT, &GPIO_InitStructure );

	// Configure radio BUSY as inputs// Configure BUSY
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin =  BUSY_PIN;
	GPIO_Init( BUSY_IOPORT, &GPIO_InitStructure );


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//创建外部中断结构体

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource8);

	
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	#ifdef USART_DEBUG
	printf("%s \r\n",__FUNCTION__);
	#endif
}


static void sx126x_HwResourceInit(void)
{
	sx126x_IoInit();
	sx126x_SpiInit();
	
	#ifdef USART_DEBUG
	printf("%s \r\n",__FUNCTION__);
	#endif

}
void sx126x_HwInit(void)
{
	sx126x_HwResourceInit();	
}

static void sx126x_CsSet(CS_STATUS enable)
{
	if(CS_LOW == enable)
		GPIO_ResetBits( NSS_IOPORT, NSS_PIN );
	else if(CS_HIGH == enable)
		GPIO_SetBits(NSS_IOPORT, NSS_PIN );
	
	WAIT_FOR_WHILE();
	
}

void wait(void)
{
	unsigned int timeout;
	
	while(1 == GPIO_ReadInputDataBit(BUSY_IOPORT,BUSY_PIN))
	{
		///printf("busy...\r\n");
		timeout++;	
	};
	
	#ifdef USART_DEBUG
		#ifdef SX126X_HAL_DEBUG
		printf("%s %d \r\n",__FUNCTION__,timeout);
		#endif
	#endif

}

static unsigned char sx126x_SpiTransfer(unsigned char val)
{

    SPI_I2S_SendData(SPI_INTERFACE, val);
	
    while(SPI_I2S_GetFlagStatus(SPI_INTERFACE, SPI_I2S_FLAG_RXNE) == RESET);
	
    return SPI_I2S_ReceiveData(SPI_INTERFACE);
}


void sx126x_Reset(void)
{
	GPIO_ResetBits(RESET_IOPORT, RESET_PIN);
	
	WAIT_FOR_WHILE();
	delay_ms(10);
		
	GPIO_SetBits(RESET_IOPORT, RESET_PIN);
	delay_ms(20);
	WAIT_FOR_WHILE();
	
	#ifdef USART_DEBUG
		#ifdef SX126X_HAL_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
}

void sx126x_WriteCmd(OPCODE opcode ,unsigned char *buf,unsigned char len)
{
	
	wait();
	
	sx126x_CsSet(CS_LOW);
	
	sx126x_SpiTransfer(opcode);
	
	for(int i = 0;i<len;i++)
	{
		sx126x_SpiTransfer(buf[i]);
	}
	
	sx126x_CsSet(CS_HIGH);
	
}


void sx126x_ReadCmd(OPCODE opcode ,unsigned char *buf,unsigned char len)
{
	
	wait();
	
	sx126x_CsSet(CS_LOW);
	
	sx126x_SpiTransfer(opcode);
	
	sx126x_SpiTransfer(0);
	
	for(int i = 0;i<len;i++)
	{
		buf[i] = sx126x_SpiTransfer(0);
	}
	
	sx126x_CsSet(CS_HIGH);
	
}


void sx126x_WriteRegister(unsigned short address ,unsigned char *buf,unsigned char len)
{
	
	wait();
	
	sx126x_CsSet(CS_LOW);
	
	sx126x_SpiTransfer(WRITE_REGISTER);
	
	sx126x_SpiTransfer((unsigned char )((address >> 8)&0xFF));
	
	sx126x_SpiTransfer((unsigned char )(address&0xFF));
	
	for(int i = 0;i<len;i++)
	{
		sx126x_SpiTransfer(buf[i]);
	}
	
	sx126x_CsSet(CS_HIGH);
	
}


void sx126x_ReadRegister(unsigned short address ,unsigned char *buf,unsigned char len)
{
	wait();
	
	sx126x_CsSet(CS_LOW);
	
	sx126x_SpiTransfer(READ_REGISTER);
	
	sx126x_SpiTransfer((unsigned char )((address >> 8)&0xFF));
	
	sx126x_SpiTransfer((unsigned char )(address&0xFF));
	
	sx126x_SpiTransfer(0);
	
	for(int i = 0;i<len;i++)
	{
		buf[i] = sx126x_SpiTransfer(0);
	}
	
	sx126x_CsSet(CS_HIGH);
	
}

void sx126x_WriteRegisterByte(unsigned short address ,unsigned char val)
{
	sx126x_WriteRegister(address,&val,1);
}



unsigned char sx126x_ReadRegisterByte(unsigned short address )
{
	unsigned char val;
	
	sx126x_ReadRegister(address,&val,1);
	
	return val;
}

void sx126x_WriteBuffer(unsigned char offset,unsigned char *buf,unsigned char len)
{
	wait();
	
	sx126x_CsSet(CS_LOW);
	
	sx126x_SpiTransfer(WRITE_BUFFER);
	
	sx126x_SpiTransfer(offset);
	
	for(int i = 0;i<len;i++)
	{
		sx126x_SpiTransfer(buf[i]);
	}
	
	sx126x_CsSet(CS_HIGH);
	
	#ifdef USART_DEBUG
		#ifdef SX126X_HAL_DEBUG
		printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
}

void sx126x_ReadBuffer(unsigned char offset,unsigned char *buf,unsigned char len)
{
	wait();
	
	sx126x_CsSet(CS_LOW);
	
	sx126x_SpiTransfer(READ_BUFFER);
	
	sx126x_SpiTransfer(offset);
	
	sx126x_SpiTransfer(0);
	for(int i = 0;i<len;i++)
	{
		buf[i] = sx126x_SpiTransfer(0);
	}
	
	sx126x_CsSet(CS_HIGH);
	#ifdef USART_DEBUG
		#ifdef SX126X_HAL_DEBUG
		printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
}

