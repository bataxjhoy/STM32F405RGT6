#ifndef __SX126X_H

#define __SX126X_H

//#define SX126X_DEBUG

#define XTAL_STEP 0.95367431640625


typedef enum{

	COLD_START = 0,
	WARM_START = 1,
}SLEEP_MODE;


typedef enum{

	STDBY_RC = 0,
	STDBY_XOSC = 1,
}STANDBY_MODE;

typedef enum{

	TIMEOUT_DISABLE = 0,
	TIMEOUT_ENABLE	= 64*20,//20ms,
	TIMEOUT_MAX = 0xFFFFFF,
	
}TIMEOUT_MODE;

typedef enum{

	SX126X_DISABLE = 0x00,
	SX126X_ENABLE = 0x01,
	
}SX126X_PARAMS_STATUS;

typedef enum{

	LDO = 0x00,
	DCDC_LDO = 0x01,
	
}REGULATOR_MODE;


typedef enum{

	FALLBACK_FS = 0x40,
	FALLBACK_STDBY_XOSC = 0x30,
	FALLBACK_STDBY_RC = 0x20,
	
}FALLBACK_MODE;


typedef enum{

	SF5 = 0x5,	//32
	SF6 = 0x6,	//64
	SF7 = 0x7,	//128
	SF8 = 0x08,	//256
	SF9 = 0x09,	//512
	SF10 = 0x0A,	// 1024
	SF11 = 0x0B,	//2048
	SF12 = 0x0C,	//4096
}LORA_SF;

typedef enum{

	
	LORA_BW_7 		= 0x00,		//(7.81 kHz real)
	LORA_BW_10 		= 0x08, 	//(10.42 kHz real)
	LORA_BW_15 		= 0x01,  	//(15.63 kHz real)
	LORA_BW_20 		= 0x09,  	//(20.83 kHz real)
	LORA_BW_31 		= 0x02,  	//(31.25 kHz real)
	LORA_BW_41 		= 0x0A,  	//(41.67 kHz real)
	LORA_BW_62 		= 0x03,  	//(62.50 kHz real)
	LORA_BW_125		= 0x04,  	//(125 kHz real)
	LORA_BW_250		= 0x05,  	//(250 kHz real)
	LORA_BW_500		= 0x06,  	//(500 kHz real)

}LORA_BW;

typedef enum{

	LORA_CR_4_5 = 0x01,
	LORA_CR_4_6 = 0x02,
	LORA_CR_4_7 = 0x03,
	LORA_CR_4_8 = 0x04,

}LORA_CR;

typedef enum{

	LowDataRateOptimize_OFF = 0x00,
	LowDataRateOptimize_ON = 0x01, 

}LORA_LowDataRateOptimize;

typedef enum{

	IRQ_TX_DONE	= 0x1,
	IRQ_RX_DONE	= 0x2,
	IRQ_PREAMBLE_DETECTED = 0x4,
	IRQ_SYNCWORD_VALID = 0x8,
	IRQ_HEADER_VALID = 0x10,
	IRQ_HEADER_ERR = 0x20,
	IRQ_CRC_ERR = 0x40,
	IRQ_CAD_DONE = 0x80,
	IRQ_CAD_DETECTED = 0x100,
	IRQ_TIMEOUT = 0x200,
}IRQ_MASK;

typedef enum{

	DIO3_1V6 = 0x0,
	DIO3_1V7,
	DIO3_1V8,
	DIO3_2V2,
	DIO3_2V4,
	DIO3_2V7,
	DIO3_3V0,
	DIO3_3V3,
}TCXO_VOLTAGE;

typedef enum{

	PACKET_TYPE_GFSK = 0x00,
	PACKET_TYPE_LORA = 0x01,
}PACKET_TYPE;


typedef enum{

	dBm17_N	= 0xEF,//-17dBm
	dBm16_N	= 0xF0,//-16dBm
	dBm15_N	= 0xF1,//-15dBm
	dBm14_N	= 0xF2,//-14dBm
	dBm13_N	= 0xF3,//-13dBm
	dBm12_N	= 0xF4,//-12dBm
	dBm11_N	= 0xF5,//-11dBm
	dBm10_N	= 0xF6,//-10dBm
	dBm9_N	= 0xF7,//-9dBm
	dBm8_N	= 0xF8,//-8dBm
	dBm7_N	= 0xF9,//-7dBm
	dBm6_N	= 0xFA,//-6dBm
	dBm5_N	= 0xFB,//-5dBm
	dBm4_N	= 0xFC,//-4dBm
	dBm3_N	= 0xFD,//-3dBm
	dBm2_N	= 0xFE,//-2dBm
	dBm1_N	= 0xFF,//-1dBm
	dBm0	= 0x0,//0dBm
	dBm1_P	= 0x01,//+1dBm
	dBm2_P	= 0x02,//+2dBm
	dBm3_P	= 0x03,//+3dBm
	dBm4_P	= 0x04,//+4dBm
	dBm5_P	= 0x05,//+5dBm
	dBm6_P	= 0x06,//+6dBm
	dBm7_P	= 0x07,//+7dBm
	dBm8_P	= 0x08,//+8dBm
	dBm9_P	= 0x09,//+9dBm
	dBm10_P	= 0x0A,//+10dBm
	dBm11_P	= 0x0B,//+11dBm
	dBm12_P	= 0x0C,//+12dBm
	dBm13_P	= 0x0D,//+13dBm
	dBm14_P	= 0x0E,//+14dBm
	dBm15_P	= 0x0F,//+15dBm
	dBm16_P	= 0x10,//+16dBm
	dBm17_P	= 0x11,//+17dBm
	dBm18_P	= 0x12,//+18dBm
	dBm19_P	= 0x13,//+19dBm
	dBm20_P	= 0x14,//+20dBm
	dBm21_P	= 0x15,//+21dBm
	dBm22_P	= 0x16,//+22dBm
	dBm23_P	= 0x17,//+23dBm
	dBm24_P	= 0x18,//+24dBm
	dBm25_P	= 0x19,//+25dBm
	dBm26_P	= 0x1A,//+26dBm
	dBm27_P	= 0x1B,//+27dBm
	dBm28_P	= 0x1C,//+28dBm
	dBm29_P	= 0x1D,//+29dBm
	dBm30_P	= 0x1E,//+30dBm
	
}TX_POWER;

typedef enum{

	SET_RAMP_10U = 0x00, //10us
	SET_RAMP_20U,		//20us
	SET_RAMP_40U,		//40us
	SET_RAMP_80U,		//80us
	SET_RAMP_200U, 		//200us
	SET_RAMP_800U,		//800us
	SET_RAMP_1700U, 	//1700us
	SET_RAMP_3400U,		//3400us
}RAMP_TIME;

typedef enum{

	CAD_ON_1_SYMB = 0x00,	//Number of Symbols used for CAD
	CAD_ON_2_SYMB,
	CAD_ON_4_SYMB,
	CAD_ON_8_SYMB,
	CAD_ON_16_SYMB,
}CAD_SYMNOL_NUM;

typedef enum{

	CAD_ONLY = 0x00,	
	CAD_RX,

}CAD_EXIT_MODE;

typedef struct pa_optimal_settings
{
	unsigned char paDutyCycle;
	unsigned char hpMax; 
	unsigned char deviceSel;
	unsigned char paLut;

}t_PA_OPTIMAL_SETTINGS,*pt_PA_OPTIMAL_SETTINGS;

typedef struct modulation_params{

	unsigned char sf;//;SpreadingFactor;
	unsigned char bw;//BandWidth;
	unsigned char cr;//CodingRate;
	unsigned char ldro; //LowDataRateOptimize;
	
}t_MODULATION_PARAMS,*pt_MODULATION_PARAMS;


typedef struct packet_params_lora{

	unsigned short 	preamble_Length;
	unsigned char 	header_type;
	unsigned char  	payload_length;
	unsigned char  	crc_type;
	unsigned char  	invert_IQ;
}t_PACKET_PARAMS_LORA,*pt_PACKET_PARAMS_LORA;


typedef struct packet_status{

	int RssiPkt;
	int SnrPkt;
	int SignalRssiPkt;
	
}t_PACKET_STATUS,*pt_PACKET_STATUS;


void sx126x_SetSleep(unsigned char SleepConfig);

void sx126x_SetStandby(unsigned char StandbyConfig);

void sx126x_SetFs(void);

void sx126x_SetTx(unsigned int timeout);

void sx126x_SetRx(unsigned int timeout);

void sx126x_StopTimerOnPreamble(unsigned char enable);

void sx126x_SetRxDutyCycle(unsigned int rxPeriod,unsigned int sleepPeriod);

void sx126x_SetCAD(void);

void sx126x_SetTxContinuousWave(void);

void sx126x_SetTxInfinitePreamble(void);

void sx126x_SetRegulatorMode(unsigned char regModeParam);

void sx126x_Calibrate(unsigned char calibParam);

void sx126x_CalibrateImage(unsigned int freq);

void sx126x_SetPaConfig(pt_PA_OPTIMAL_SETTINGS paOptimalSetting);

void sx126x_SetRxTxFallbackMode(unsigned char fallbackModet);

void sx126x_SetDioIrqParams(unsigned short IrqMask,unsigned short DIO1Mask, unsigned short DIO2Mask, unsigned short DIO3Mask);

unsigned short sx126x_GetIrqStatus(void);

void sx126x_ClearIrq(unsigned short irq);

void sx126x_SetDIO2AsRfSwitchCtrl(unsigned char enable);

void sx126x_SetDIO3AsTCXOCtrl(unsigned char tcxoVoltage,unsigned int timeout);

void sx126x_SetRfFrequency(unsigned int Frequency);

void sx126x_SetPacketType(unsigned char PacketType);

unsigned char sx126x_GetPacketType(void);

void sx126x_SetTxParams(unsigned char power,unsigned char RampTime);

void sx126x_SetModulationParams(pt_MODULATION_PARAMS params);

void sx126x_SetPacketParams(pt_PACKET_PARAMS_LORA params);

void sx126x_SetCadParams(unsigned char cadSymbolNum, unsigned char cadDetPeak, unsigned char cadDetMin, unsigned char cadExitMode, unsigned int cadTimeout);

void sx126x_SetBufferBaseAddress(unsigned char txBaseAddress, unsigned char rxBaseAddress);
void sx126x_SetLoRaSymbNumTimeout(unsigned char SymbNum);

void sx126x_GetRxBufferStatus(unsigned char *PayloadLengthRx,unsigned char *RxStartBufferPointer);

void sx126x_GetPacketStatus(pt_PACKET_STATUS PacketStatus);

int sx126x_GetRssiInst(void);

unsigned char sx126x_GetStatus(void);

void sx126x_ResetStats(void);

unsigned short sx126x_GetDeviceErrors(void);

void sx126x_ClearDeviceErrors(void);
#endif

