#include "radio.h"
#include "string.h"
#include "stm32f4xx.h"
//#define USE_CONFIG_PUBLIC_NETOWRK

#define RADIO_DEBUG	

#define RF_FREQUENCY 		915000000
#define TX_BUFFER_BASE 		0x0
#define RX_BUFFER_BASE 		0x0



unsigned char radio_irq_flag = 0;

t_RADIO_MSG radio_msg_rx;
t_RADIO_MSG radio_msg_tx;

t_MODULATION_PARAMS radio_modulation_params = {

	.sf = SF6,
	.bw = LORA_BW_250,
	.cr = LORA_CR_4_5,
	.ldro = LowDataRateOptimize_OFF,

};

t_PACKET_PARAMS_LORA radio_packet_params_lora = {
	
	.preamble_Length = 8,
	.header_type = 0,
	.payload_length = 64,
	.crc_type = 1,
	.invert_IQ = 0,

};

t_RADIO_TIMEOUT radio_timeout_lora = {

	.timeout_tx = TIMEOUT_ENABLE,
	.timeout_rx = TIMEOUT_DISABLE, 
};


t_PA_OPTIMAL_SETTINGS radio_pa_optimal_setting = {


//+22dBm
	.paDutyCycle = 0x04,
	.hpMax	= 0x07,
	.deviceSel = 0x0,
	.paLut = 0x1,
	
#if 0
	//+20dBm
	.paDutyCycle = 0x03,
	.hpMax	= 0x05,
	.deviceSel = 0x0,
	.paLut = 0x1,
	
	//+17dBm
	.paDutyCycle = 0x02,
	.hpMax	= 0x03,
	.deviceSel = 0x0,
	.paLut = 0x1,
	
	//+14dBm
	.paDutyCycle = 0x04,
	.hpMax	= 0x06,
	.deviceSel = 0x0,
	.paLut = 0x1,
	
	//+10dBm
	.paDutyCycle = 0x00,
	.hpMax	= 0x03,
	.deviceSel = 0x0,
	.paLut = 0x1,
#endif	
};

t_PACKET_STATUS radio_packet_status;


t_RADIO_INST radio = {

	.name = "lora",
	.modulation_params = &radio_modulation_params,
	.packet_params_lora = &radio_packet_params_lora,
	.pa_optimal_setting = &radio_pa_optimal_setting,
	.packet_status = &radio_packet_status,
	.radio_irq_flag = 0,
	.irq_mask = IRQ_TX_DONE|RX_DONE|IRQ_TIMEOUT,
	.status_running = IDLE,
};


void radio_Init(void)
{
	sx126x_HwInit();
	
	sx126x_Reset();
	
	sx126x_SetStandby(STDBY_RC);
	sx126x_SetRegulatorMode(0x01);
//	sx126x_SetDIO3AsTCXOCtrl(DIO3_3V3,0x1F4);//有源晶振使用
//	
//	sx126x_Calibrate(0x7f);//Calibrate all
	
	sx126x_SetStandby(STDBY_XOSC);

	sx126x_SetPacketType(PACKET_TYPE_LORA);//0:GFSK; 1:LORA
	
	#ifdef USE_CONFIG_PUBLIC_NETOWRK
    // Change LoRa modem Sync Word for Public Networks   
		sx126x_WriteRegisterByte(SyncWordMSB,0x34);
		sx126x_WriteRegisterByte(SyncWordLSB,0x44);
	#else
    // Change LoRa modem SyncWord for Private Networks
    sx126x_WriteRegisterByte(SyncWordMSB,0x14);
		sx126x_WriteRegisterByte(SyncWordLSB,0x24);
	#endif


	sx126x_SetDIO2AsRfSwitchCtrl(0x1);//控制PA 和LNA
	
	sx126x_SetRfFrequency(RF_FREQUENCY);
	sx126x_CalibrateImage(RF_FREQUENCY);	
	sx126x_SetTxParams(dBm6_P,SET_RAMP_40U);
	
	sx126x_SetPacketParams(radio.packet_params_lora);
	
	sx126x_SetBufferBaseAddress(TX_BUFFER_BASE,RX_BUFFER_BASE);
	
	sx126x_SetModulationParams(radio.modulation_params);
	
	sx126x_SetDioIrqParams(radio.irq_mask,radio.irq_mask,0x000,0x000);//IRQ_TX_DONE|RX_DONE|IRQ_TIMEOUT
	
	sx126x_SetPaConfig(radio.pa_optimal_setting);
	
	radio_StartRx();//default rx mode
	
}

void radio_Reset(void)
{
	sx126x_Reset();
	
	sx126x_SetStandby(STDBY_RC);
	
//	sx126x_SetDIO3AsTCXOCtrl(DIO3_3V3,0x1F4 );
//	
//	sx126x_Calibrate(0x7f);//Calibrate all

	sx126x_SetStandby(STDBY_XOSC);

	sx126x_SetPacketType(PACKET_TYPE_LORA);
	
	#ifdef USE_CONFIG_PUBLIC_NETOWRK
    // Change LoRa modem Sync Word for Public Networks
   
		sx126x_WriteRegisterByte(SyncWordMSB,0x34);
		sx126x_WriteRegisterByte(SyncWordLSB,0x44);
	#else
    // Change LoRa modem SyncWord for Private Networks
    sx126x_WriteRegisterByte(SyncWordMSB,0x14);
		sx126x_WriteRegisterByte(SyncWordLSB,0x24);
	#endif
	
	sx126x_SetDIO2AsRfSwitchCtrl(0x1);//控制PA 和LNA

	
	
	sx126x_SetRfFrequency(RF_FREQUENCY);
		
	sx126x_SetTxParams(dBm6_P,SET_RAMP_40U);
	
	sx126x_SetPacketParams(radio.packet_params_lora);
	
	sx126x_SetBufferBaseAddress(TX_BUFFER_BASE,RX_BUFFER_BASE);
	
	sx126x_SetModulationParams(radio.modulation_params);
	
	sx126x_SetDioIrqParams(radio.irq_mask,radio.irq_mask,0x000,0x000);
	
	sx126x_SetPaConfig(radio.pa_optimal_setting);
	
	radio_StartRx();//default rx mode
}



void radio_StartRx(void)
{	
//		sx126x_SetStandby(STDBY_RC);	
//		sx126x_SetBufferBaseAddress(TX_BUFFER_BASE,RX_BUFFER_BASE);
//		sx126x_SetDioIrqParams(RX_DONE|TIMEOUT|CRC_ERR,RX_DONE|TIMEOUT|CRC_ERR,0x000,0x000);
//		sx126x_SetTx(TIMEOUT_ENABLE);
	
		sx126x_SetRx(TIMEOUT_ENABLE);
		GPIO_SetBits(RXEN_IOPORT, RXEN_PIN);
		GPIO_ResetBits( TXEN_IOPORT, TXEN_PIN );
	
}

void radio_exit(void)
{

}

void radio_PackgetGet(unsigned char *buf,unsigned char *len)
{
	
	if(radio_msg_rx.len>20)
	{		*len = radio_msg_rx.len;
			return;
	}
	memcpy(buf,radio_msg_rx.buf,radio_msg_rx.len);
	
	*len = radio_msg_rx.len;
	
	radio_msg_rx.len = 0;
	
	#ifdef USART_DEBUG
		#ifdef RADIO_DEBUG	
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
}

void radio_PackgetSend(unsigned char *buf,unsigned char len)
{
	radio.packet_params_lora->payload_length = len;
	
	sx126x_SetPacketParams(radio.packet_params_lora);
	
	sx126x_WriteBuffer(TX_BUFFER_BASE,buf,radio.packet_params_lora->payload_length);
	
	
//		sx126x_SetStandby(STDBY_XOSC);	
//		sx126x_SetTx(TIMEOUT_ENABLE);
//		sx126x_SetDioIrqParams(TX_DONE|TIMEOUT,TX_DONE|TIMEOUT,0x000,0x000);	

		sx126x_SetTx(TIMEOUT_ENABLE);
		GPIO_ResetBits(RXEN_IOPORT, RXEN_PIN);
		GPIO_SetBits( TXEN_IOPORT, TXEN_PIN );

	
	#ifdef USART_DEBUG
		#ifdef RADIO_DEBUG	
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
}


unsigned short radio_Process(void)
{

	unsigned short status = IDLE;
	
	if(1 == radio.radio_irq_flag)
	{
		status = radio.status_running & 0x3FF;
		switch (radio.status_running)
		{
			case IDLE:
				#ifdef USART_DEBUG
					printf("IDLE \r\n");
				#endif
				radio_StartRx();
				break;
			case TX_DONE:
				#ifdef USART_DEBUG
					printf("TX_DONE \r\n");
				#endif
				radio_StartRx();
				break;
			case RX_DONE:
				#ifdef USART_DEBUG
					printf("RX_DONE \r\n");
				#endif
				sx126x_GetRxBufferStatus(&radio_msg_rx.len,&radio_msg_rx.rx_buffer_pointer);
				sx126x_ReadBuffer(radio_msg_rx.rx_buffer_pointer,radio_msg_rx.buf,radio_msg_rx.len);
				radio_StartRx();
				sx126x_GetPacketStatus(radio.packet_status);
				//printf("RssiPkt = %d SignalRssiPkt = %d SnrPkt = %d \r\n",radio.packet_status->RssiPkt,radio.packet_status->SignalRssiPkt,radio.packet_status->SnrPkt);
				break;
			case PREAMBLE_DETECTED:
				#ifdef USART_DEBUG
					printf("PREAMBLE_DETECTED \r\n");
				#endif
				
				break;
			case SYNCWORD_VALID:
				#ifdef USART_DEBUG
					printf("SYNCWORD_VALID \r\n");
				#endif
				break;
			case HEADER_VALID:
				#ifdef USART_DEBUG
					printf("HEADER_VALID \r\n");
				#endif
				break;
			case HEADER_ERR:
				#ifdef USART_DEBUG
					printf("HEADER_ERR \r\n");
				#endif
				break;
			case CRC_ERR:
				#ifdef USART_DEBUG
					printf("CRC_ERR \r\n");
				#endif
				break;
			case CAD_DONE:
				#ifdef USART_DEBUG
					printf("CAD_DONE \r\n");
				#endif
				break;
			case CAD_DETECTED:
				#ifdef USART_DEBUG
					printf("CAD_DETECTED \r\n");
				#endif
				break;
			case TIMEOUT:
				#ifdef USART_DEBUG
					printf("TIMEOUT \r\n");
				#endif
			radio_StartRx();
				break;
			default:
				break;
		}
		
		radio.radio_irq_flag = 0;

	}
	return status;
}

void EXTI9_5_IRQHandler(void)
{
	
	if (EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		radio.radio_irq_flag = 1;
		radio.status_running = sx126x_GetIrqStatus();
		#ifdef USART_DEBUG
				printf("IRQ = %x \r\n",radio.status_running );
		#endif
		sx126x_ClearIrq(0x3FF);

		EXTI_ClearITPendingBit(EXTI_Line8);
	}
}

