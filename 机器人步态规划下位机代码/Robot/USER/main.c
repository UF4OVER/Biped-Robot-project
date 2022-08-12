#include "allconfig.h"
#define Servo_init 0   //��ʼ�����
#define SIZE sizeof(TEXT_Buffer)
void Control_Servo(u8 ID,float angle,u8 point,int time);
//void Flash_Angle(void);
extern u8 angle_7[20];
extern u8 angle_8[20];
const u8 TEXT_Buffer[]={"Apollo STM32F7 QSPI TEST"};
//extern u8 SERVO_ExpectPosition[17][7];
	u32 FLASH_SIZE;
	u8 datatemp[SIZE];
int main(void)
{
  u16 i,j,k=0;
	u8 t=0;

	
	Cache_Enable();                 //��L1-Cache
	HAL_Init();				        //��ʼ��HAL��
	Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
	delay_init(216);                //��ʱ��ʼ��
//	LED_Init();                     //��ʼ��LED
//	KEY_Init();                     //��ʼ������
//	SDRAM_Init();                   //��ʼ��SDRAM
//	LCD_Init();                     //LCD��ʼ��    
//	W25QXX_Init();		            //��ʼ��W25QXX
	MYDMA_Config(DMA1_Stream6,DMA_CHANNEL_4);//��ʼ��DMA	
	POINT_COLOR=RED; 
	uart_init(1000000);//���ͨ��
//  LCD_ShowString(20,50,200,16,16,"Biped Robot Control System");
	#if Servo_init  
			        //���ڳ�ʼ�� 
//		ReadServo_state(0xfe,1);                              //��ȡ״̬
		while(1)
		{
			t=KEY_Scan(0);
//			if(t==WKUP_PRES)  //KEY0����
//	  	{
//			 delay_ms(5);
//		 	 LCD_ShowString(30,150,200,16,16,"Start Transimit....");                                     			          
      if(k==0)
			{			
         
			  Servo_Lock(DISABLE);//�ر�������		
        EnableTorque(0xfe);//��Ť��				
			  Set_Servo_ID(10);
			  Servo_Lock(ENABLE);//������
      k++;
			}
			delay_ms(100);
			if(k==1)
			{ 
		    Write_Position(2,502,0,1000);    //ת��ָ��λ�� �趨�ٶ� ʱ��                                       			          		   
		 	  delay_ms(120);				
   			}			  
//    }
    j=20;
	 for(i=0;i<=5;i++)
  	{								  
	    delay_ms(10);
	    LCD_ShowxNum(j,230,Receive_data[i],3,16,0x80);
	    j+=30;
		}
		LED1_Toggle;	
		}
	
	
	
	#endif

//	delay_ms(100);
//  W25QXX_Check();		
//  FLASH_SIZE=32*1024*1024;	//FLASH ��СΪ32M�ֽ�	
//	Flash_Angle();//ȡ������
//SYNC_Write_Buff(SERVO_ExpectPosition,17);
//		Write_Position(20,512,0,1000); 

  while(1)
  {	  
//		LCD_ShowxNum(80,80,angle_7[0],2,32,0);
//		  LCD_ShowxNum(120,80,angle_7[1],2,32,0);
//		  LCD_ShowxNum(80,120,angle_8[0],2,32,0);
//		  LCD_ShowxNum(120,120,angle_8[1],2,32,0);
		  delay_ms(20);
//			t=KEY_Scan(0);			
//		  LED0_Toggle;
//		if(t==WKUP_PRES)
//		{

//				Control_Servo(1,14,0,500);  //��б
//				Control_Servo(5,8.6,0,500);
//				 
//				Control_Servo(6,18.6,0,500);
//		    Control_Servo(10,2.7,0,500);
//				SYNC_Write_Buff(SERVO_ExpectPosition,17);	
       Write_Position(15,400,0,500); 		
//		    delay_ms(500);
//        for(i=0;i<20;i++)    //̧��
//			{
//				Control_Servo(1,angle_7[i],1,25);
//				Control_Servo(8,angle_8[i],0,25);
//				Control_Servo(9,angle_8[i]-angle_7[i],1,25);
//				SYNC_Write_Buff(SERVO_ExpectPosition,17);		
//				delay_ms(25);
//			}

//			
//	   } 
//		 if(t==KEY1_PRES)  
//		 {
//			      for(i=19;i>0;i--)    //���
//			{
//				Control_Servo(1,angle_7[i],1,25);
//				Control_Servo(8,angle_8[i],0,25);
//				Control_Servo(9,angle_8[i]-angle_7[i],1,25);
//				SYNC_Write_Buff(SERVO_ExpectPosition,17);		
//				delay_ms(25);
//		 }			 
//	}
//		 if(t==KEY2_PRES)//KEY1����,д��W25Q128
//		{
//			LCD_Fill(0,170,239,319,WHITE);//�������    
// 			LCD_ShowString(30,170,200,16,16,"Start Write QSPI....");
//			W25QXX_Write((u8*)TEXT_Buffer,FLASH_SIZE-100,SIZE);		//�ӵ�����100����ַ����ʼ,д��SIZE���ȵ�����
//			LCD_ShowString(30,170,200,16,16,"QSPI Write Finished!");	//��ʾ�������
//		}
//		if(t==WKUP_PRES)//KEY0����,��ȡ�ַ�������ʾ
//		{
// 			LCD_ShowString(30,170,200,16,16,"Start Read QSPI.... ");
//			W25QXX_Read(datatemp,FLASH_SIZE-100,SIZE);					//�ӵ�����100����ַ����ʼ,����SIZE���ֽ�
//			LCD_ShowString(30,170,200,16,16,"The Data Readed Is:   ");	//��ʾ�������
//			LCD_ShowString(30,190,200,16,16,datatemp);					//��ʾ�������ַ���
//		} 

}
	}

void Control_Servo(u8 ID,float angle,u8 point,int time)
{
	u8 i;
	u16 number;
	i=ID-1;
	 	
	if(ID==7||ID==4)
	{
	  if(point==1)
			point=0;
		else
			point=1;
	}
	if(ID==8) point=1;
	
	number=Together_Number(&Servo_State[i][1],&Servo_State[i][2]);//ȡ����׼������
	if (point==1)//ǰ��
	number=number+(int)(angle*5.12);
	else
	number=number-(int)(angle*5.12);	
	Split_Number(&SERVO_ExpectPosition[i][1],&SERVO_ExpectPosition[i][2], number);
	Split_Number(&SERVO_ExpectPosition[i][3],&SERVO_ExpectPosition[i][4], time);
}
//void Flash_Angle(u8 *datatemp)
//{
//	u8 i;
//	LCD_ShowString(30,170,200,16,16,"Start Read QSPI.... ");
//	for	(i=1;i<2;i++)
//	{//����һ���Ƕ����� robto_angle[17][20]
//		W25QXX_Read(datatemp,FLASH_SIZE-(20*(21-i)),20);					//�ӵ�����100����ַ����ʼ,����SIZE���ֽ�
//	}
//	LCD_ShowString(30,170,200,16,16,"The Data Readed Is:   ");	//��ʾ�������
//	LCD_ShowString(30,190,200,16,16,datatemp);					//��ʾ�������ַ���
//}
