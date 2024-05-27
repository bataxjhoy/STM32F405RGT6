#include "sx1262.h"
#include "Delay.h"
#include "sys.h" 
#include "rf.h"
#include "i2s.h" 
#include "string.h"
const char *rfName = "SX1262";
 


u16	iSend, iRev;	//���߷��� ���ռ���



//��Ƶģ���ʼ��
void RFInit( void )
{
	Sx1262_Flag.RFCtrl.RFmode = LDRO_ON;        //LoRa ģʽ����
	Sx1262_Flag.RFCtrl.Bandwidth = LORA_BW_250;  //LoRa���ƴ���
	Sx1262_Flag.RFCtrl.CodeRate = LORA_CR_4_5;  //������
	Sx1262_Flag.RFCtrl.SpreadingFactor = SF5;   //��Ƶ����
	
	//timerx_init();        ///30Hz���ж�Ƶ��
	Sx1262_SPI_Init();
	reset_sx1262();		//reset RF
	sx1262_Config();
	RFRxMode(); //��ʼ���������ģʽ
}


//��Ƶģ��������ģʽ
void RFRxMode( void )
{
   Rx_Init();
}


//��Ƶģ���������
u8 RFRevData(u8 *buf)
{
	u8 rbuf[64+1]={0},length,i;
  u8 Irq_Status;
	u8 packet_size,buf_offset;
	if(sizeof(buf)>DMA_BUF_SIZE) return (0x0);
	
  if(!Sx1262_Flag.is_tx)	
	{
		//Wait for the IRQ  RxDone or Timeout
		if(RF_IRQ())//�ȴ������ж�
		{
			Irq_Status = GetIrqStatus();//��ȡ�ж�״̬
			if((Irq_Status&0x02) == RxDone_IRQ)
			{
				//LedGreen( On );
				GetRxBufferStatus(&packet_size, &buf_offset);
				ReadBuffer(buf_offset, rbuf, packet_size);
				Sx1262_Flag.RxRssi = 0-(GetRssiInst())/2;
				ClearIrqStatus(RxDone_IRQ);//Clear the IRQ RxDone flag
				Irq_Status = GetIrqStatus();// 
				RFRxMode();
				length = rbuf[0];
				
				memcpy (&buf[0], &rbuf[1], length);
			
				return length;
			} 
		}
	}
	return (0x0);
}

//ͨ������ģ�鷢�ʹ�СΪsize������
u8 RFSendData( u8 *buf, u8 size )
{
	u8 sendbuf[size+1];  
	u8 ret = 1,i=0;
	sendbuf[0] = size;
	 memcpy ( &sendbuf[1], &buf[0], size);
	
	Tx_Start(sendbuf,size+1);
	
	RFRxMode();	
	return ret;
}

