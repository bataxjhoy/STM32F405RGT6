#include "sx126x.h"
#include "sx126x_hal.h"
#include "stdio.h"


void sx126x_SetSleep(unsigned char SleepConfig)
{
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
	
	sx126x_WriteCmd(SET_SLEEP,&SleepConfig,1);
	
}

void sx126x_SetStandby(unsigned char StandbyConfig)
{
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
	
	sx126x_WriteCmd(SET_STANDBY,&StandbyConfig,1);
	
}

void sx126x_SetFs(void)
{
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
	
	sx126x_WriteCmd(SET_FS,NULL,0);
	
}

void sx126x_SetTx(unsigned int timeout)
{
	unsigned char buf[3];
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
	
	buf[0] = (unsigned char)((timeout>>16)&0xFF);
	buf[1] = (unsigned char)((timeout>>8)&0xFF);
	buf[2] = (unsigned char)((timeout)&0xFF);
	
	sx126x_WriteCmd(SET_TX,buf,sizeof(buf));
	
}

void sx126x_SetRx(unsigned int timeout)
{
	unsigned char buf[3];
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
	
	buf[0] = (unsigned char)((timeout>>16)&0xFF);
	buf[1] = (unsigned char)((timeout>>8)&0xFF);
	buf[2] = (unsigned char)((timeout)&0xFF);
	
	sx126x_WriteCmd(SET_RX,buf,sizeof(buf));
	
}

void sx126x_StopTimerOnPreamble(unsigned char enable)
{
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
	
	sx126x_WriteCmd(STOP_TIMER_ON_PREAMBLE,&enable,1);	
}

void sx126x_SetRxDutyCycle(unsigned int rxPeriod,unsigned int sleepPeriod)
{
	unsigned char buf[6];
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
	
	buf[0] = (unsigned char)((rxPeriod >> 16) & 0xFF);
    buf[1] = (unsigned char)((rxPeriod >> 8) & 0xFF);
    buf[2] = (unsigned char)(rxPeriod & 0xFF);
    buf[3] = (unsigned char)((sleepPeriod >> 16) & 0xFF);
    buf[4] = (unsigned char)((sleepPeriod >> 8) & 0xFF);
    buf[5] = (unsigned char)(sleepPeriod & 0xFF);
	
	sx126x_WriteCmd(SET_RX_DUTY_CYCLE,buf,sizeof(buf));
	
}

void sx126x_SetCAD(void)
{
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	sx126x_WriteCmd(SET_CAD,NULL,0);
	
}


void sx126x_SetTxContinuousWave(void)
{
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	sx126x_WriteCmd(SET_TX_CONTINUOUS_WAV,NULL,0);
	
}

void sx126x_SetTxInfinitePreamble(void)
{
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	sx126x_WriteCmd(SET_TX_INFINITE_PREAMBLE,NULL,0);
	
}

void sx126x_SetRegulatorMode(unsigned char regModeParam)
{
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	sx126x_WriteCmd(SET_REGULATOR_MODE,&regModeParam,1);
	
}

void sx126x_Calibrate(unsigned char calibParam)
{
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
	
	sx126x_WriteCmd(CALIBRATE,&calibParam,1);
	
}

void sx126x_CalibrateImage(unsigned int freq)
{
	unsigned char buf[2];
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
	
	 if( freq > 900000000 )
    {
        buf[0] = 0xE1;
        buf[1] = 0xE9;
    }
    else if( freq > 850000000 )
    {
        buf[0] = 0xD7;
        buf[1] = 0xD8;
    }
    else if( freq > 770000000 )
    {
        buf[0] = 0xC1;
        buf[1] = 0xC5;
    }
    else if( freq > 460000000 )
    {
        buf[0] = 0x75;
        buf[1] = 0x81;
    }
    else if( freq > 425000000 )
    {
        buf[0] = 0x6B;
        buf[1] = 0x6F;
    }
	
	sx126x_WriteCmd(CALIBRATE_IMAGE,buf,sizeof(buf));
	
}

void sx126x_SetPaConfig(pt_PA_OPTIMAL_SETTINGS paOptimalSetting)
{
	unsigned char buf[4];
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
	
	 buf[0] = paOptimalSetting->paDutyCycle;
     buf[1] = paOptimalSetting->hpMax;
	 buf[2] = paOptimalSetting->deviceSel;
	 buf[3] = paOptimalSetting->paLut;
	
	sx126x_WriteCmd(SET_PA_CONFIG,buf,sizeof(buf));
	
}

void sx126x_SetRxTxFallbackMode(unsigned char fallbackModet)
{
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	sx126x_WriteCmd(SET_RXTX_FALLBACK_MODE,&fallbackModet,1);
	
}

void sx126x_SetDioIrqParams(unsigned short IrqMask,unsigned short DIO1Mask, unsigned short DIO2Mask, unsigned short DIO3Mask)
{
	unsigned char buf[8];
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	buf[0] = ( unsigned char )((IrqMask >> 8 ) & 0xFF);
    buf[1] = ( unsigned char )((IrqMask) & 0xFF);
    buf[2] = ( unsigned char )((DIO1Mask >> 8 ) & 0xFF);
    buf[3] = ( unsigned char )((DIO1Mask) & 0xFF);
	buf[4] = ( unsigned char )((DIO2Mask >> 8 ) & 0xFF);
    buf[5] = ( unsigned char )((DIO2Mask) & 0xFF);
	buf[6] = ( unsigned char )((DIO3Mask >> 8 ) & 0xFF);
    buf[7] = ( unsigned char )((DIO3Mask) & 0xFF);
	
	sx126x_WriteCmd(SET_DIO_IRQ_PARAMS,buf,sizeof(buf));
}

unsigned short sx126x_GetIrqStatus(void)
{
	unsigned char IrqStatus[2] = {0x0,0x0};
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	
	sx126x_ReadCmd(GET_IRQ_STATUS,IrqStatus,2);
	
	return (unsigned short)(IrqStatus[0]<<8|IrqStatus[1]);
}


void sx126x_ClearIrq(unsigned short irq)
{
	unsigned char buf[2];
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	
	buf[0] = (unsigned char)((irq >> 8) & 0xFF); 
	buf[1] = (unsigned char)((irq) & 0xFF);
	
	sx126x_WriteCmd(CLEAR_IRQ_STATUS,buf,sizeof(buf));
}

void sx126x_SetDIO2AsRfSwitchCtrl(unsigned char enable)
{

	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif


	sx126x_WriteCmd(SET_DIO2_AS_RFSWITCH_CTRL,&enable,1);
	
}

void sx126x_SetDIO3AsTCXOCtrl(unsigned char tcxoVoltage,unsigned int timeout)
{
	unsigned char buf[4];
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif


	buf[0] = (tcxoVoltage & 0x7);
	buf[1] = (unsigned char)((timeout>>16)&0xFF);
	buf[2] = (unsigned char)((timeout>>8)&0xFF);
	buf[3] = (unsigned char)((timeout)&0xFF);
	
	sx126x_WriteCmd(SET_DIO_AS_TCXOCTRL,buf,sizeof(buf));
	
}

void sx126x_SetRfFrequency(unsigned int Frequency)
{
	unsigned char buf[4];
	unsigned int freq;
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif


	freq = (unsigned int)((double)(Frequency) / (double)(XTAL_STEP));
	buf[0] = (unsigned char)((freq>>24)&0xFF);
	buf[1] = (unsigned char)((freq>>16)&0xFF);
	buf[2] = (unsigned char)((freq>>8)&0xFF);
	buf[3] = (unsigned char)((freq)&0xFF);
	
	sx126x_WriteCmd(SET_RF_FREQUENCY,buf,sizeof(buf));
	
}


void sx126x_SetPacketType(unsigned char PacketType)
{
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
	
	sx126x_WriteCmd(SET_PACKET_TYPE,&PacketType,1);
	
}

unsigned char sx126x_GetPacketType(void)
{
	unsigned char PacketType;
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	
	sx126x_ReadCmd(GET_PACKET_TYPE,&PacketType,1);

	
	return PacketType;
}

void sx126x_SetTxParams(unsigned char power,unsigned char RampTime)
{
	unsigned char buf[2];
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	
	sx126x_WriteRegisterByte(OCP,0x38);
	
	buf[0] = power;
	buf[1] = RampTime;
	sx126x_WriteCmd(SET_TX_PARAMS,buf,sizeof(buf));
	
}

void sx126x_SetModulationParams(pt_MODULATION_PARAMS params)
{
	unsigned char buf[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	
	buf[0] = params->sf;
	buf[1] = params->bw;
	buf[2] = params->cr;
	buf[3] = params->ldro;
	//printf("SpreadingFactor = %x BandWidth = %x CodingRate = %x LowDataRateOptimize = %x \r\n",buf[0],buf[1],buf[2],buf[3]);
	sx126x_WriteCmd(SET_MODULATION_PARAMS,buf,4);//LORA
	
}


void sx126x_SetPacketParams(pt_PACKET_PARAMS_LORA params)
{
	unsigned char buf[9] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	
	buf[0] = (unsigned char)((params->preamble_Length>>8)&0xFF);
	buf[1] = (unsigned char)(params->preamble_Length&0xFF);
	buf[2] = params->header_type;
	buf[3] = params->payload_length;
	buf[4] = params->crc_type;
	buf[5] = params->invert_IQ;
	
	sx126x_WriteCmd(SET_PACKET_PARAMS,buf,6);
	
}

/*Recommended Settings for cadDetPeak and cadDetMin with 4 Symbols Detection
	SF 	cadDetPeak 	cadDetMin
	5 		18 			10
	6 		19 			10
	7 		20 			10
	8 		21 			10
	9 		22 			10
	10 		23 			10
	11 		24 			10
	12 		25 			10
*/
void sx126x_SetCadParams(unsigned char cadSymbolNum, unsigned char cadDetPeak, unsigned char cadDetMin, unsigned char cadExitMode, unsigned int cadTimeout)
{
	unsigned char buf[7] ;
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	
	buf[0] = cadSymbolNum;
	buf[1] = cadDetPeak;
	buf[2] = cadDetMin;
	buf[3] = cadExitMode;
	buf[4] = (unsigned char)(((cadTimeout>>16)&0xFF));
	buf[5] = (unsigned char)(((cadTimeout>>8)&0xFF));
	buf[6] = (unsigned char)((cadTimeout&0xFF));
	
	sx126x_WriteCmd(SET_CAD_PARAMS,buf,sizeof(buf));
	
}

void sx126x_SetBufferBaseAddress(unsigned char txBaseAddress, unsigned char rxBaseAddress)
{
	unsigned char buf[2] ;
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	
	buf[0] = txBaseAddress;
	buf[1] = rxBaseAddress;
	
	sx126x_WriteCmd(SET_BUFFER_BASE_ADDRESS,buf,sizeof(buf));
	
}

void sx126x_SetLoRaSymbNumTimeout(unsigned char SymbNum)
{
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	
	sx126x_WriteCmd(SET_BUFFER_BASE_ADDRESS,&SymbNum,1);
	
}

void sx126x_GetRxBufferStatus(unsigned char *PayloadLengthRx,unsigned char *RxStartBufferPointer)
{
	unsigned char buf[2];
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	
	sx126x_ReadCmd(GET_RX_BUFFER_STATUS,buf,sizeof(buf));
	
	*PayloadLengthRx = buf[0];
	*RxStartBufferPointer = buf[1];
	
}

void sx126x_GetPacketStatus(pt_PACKET_STATUS PacketStatus)
{
	unsigned char buf[3];
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	sx126x_ReadCmd(GET_PACKET_STATUS,buf,3);
	
	PacketStatus->RssiPkt = -(buf[0]/2);
	
	if(buf[1] < 128 )
		PacketStatus->SnrPkt = buf[1]/4; 
	else
		PacketStatus->SnrPkt =(( buf[1] - 256 ) /4 );
	
	PacketStatus->SignalRssiPkt = -(buf[2]/2);
}

int sx126x_GetRssiInst(void)
{
	unsigned char RssiInst;
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif
	
	sx126x_ReadCmd(GET_RSSI_INST,&RssiInst,1);
	
	return (-RssiInst/2);
}

unsigned char sx126x_GetStatus(void)
{
	unsigned char status;
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	
	sx126x_ReadCmd(GET_STATUS,&status,1);
	
	return status;
}

void sx126x_ResetStats(void)
{
	unsigned char buf[6] = {0x0,0x0,0x0,0x0,0x0,0x0};
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	
	sx126x_WriteCmd(GET_STATS,buf,sizeof(buf));
	
}

unsigned short sx126x_GetDeviceErrors(void)
{
	unsigned char OpError[2] = {0x0,0x0};
	
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	
	sx126x_ReadCmd(GET_DEVICE_EERORS,OpError,sizeof(OpError));
	
	return (unsigned short)(OpError[0]<<8|OpError[1]);
}

void sx126x_ClearDeviceErrors(void)
{
	unsigned char buf = 0x0;
	#ifdef USART_DEBUG
		#ifdef SX126X_DEBUG
			printf("%s \r\n",__FUNCTION__);
		#endif
	#endif

	
	sx126x_WriteCmd(CLEAR_DEVICE_ERRORS,&buf,1);
}

