#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "sdram.h"
#include "dma.h"
#include "servo.h"
#include "math.h"
#include "arm_math.h"
#include "stdio.h"

#define Servo_init 1   //��ʼ�����
/*********Readme*************/
//control three servo
//usart2: set out position data
 
int main(void)
{
    u16 i,j,t,k=0;   //used in initializing   

    double angle_1;     //rotation angle
	  double px,py;       //desired position	
    char buffer[10];	
    Cache_Enable();                 //��L1-Cache
    HAL_Init();				        //��ʼ��HAL��
    Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
    delay_init(216);                //��ʱ��ʼ��
	 
    LED_Init();                     //��ʼ��LED
    KEY_Init();                     //��ʼ������
    SDRAM_Init();                   //��ʼ��SDRAM
    LCD_Init();                     //LCD��ʼ��    
    MYDMA_Config(DMA1_Stream6,DMA_CHANNEL_4);//��ʼ��DMA	
	  POINT_COLOR=RED; 
	  LCD_ShowString(20,50,200,16,16,"Biped Robot Control System");
	  uart_init(1000000);	
	  
	#if Servo_init  
		while(1)
		{
			t=KEY_Scan(0);
			if(t==KEY0_PRES)  //KEY0����
	  	{
			 delay_ms(5);
		 	 LCD_ShowString(30,150,200,16,16,"Start Transimit....");                                     			          
      if(k==0)
			{			
         
			  Servo_Lock(DISABLE);//�ر�������		
        EnableTorque(0xfe);//��Ť��				
			  Set_Servo_ID(18);
			  Servo_Lock(ENABLE);//������
        k++;
			}
			delay_ms(100);
			if(k==1)
			{ 
		    Write_Position(18,512,0,1000);    //ת��ָ��λ�� �趨�ٶ� ʱ��                                       			          		   
		 	  delay_ms(120);	
			
		  	LCD_ShowString(30,150,200,16,16,"Transimit Finished!");//��ʾ�������
			}			  
    }
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
 
	
//      px=-2;
//      py=1;	
//  ReadServo_Position(0x01); 

		while(1)
	{	
	    for(i=200;i<900;i++)
		{
			px=i>>8;
			py=i&0x00ff;
			Servo_State[0][1]=px;
			Servo_State[0][2]=py;
	  	Servo_State[1][1]=px;
			Servo_State[1][2]=py;
			SYNC_Write_Buff(Servo_State,2); 
			delay_ms(4);
		}
		delay_ms(1000);
	      for(i=900;i>200;i--)
		{
			px=i>>8;
			py=i&0x00ff;
			Servo_State[0][1]=px;
			Servo_State[0][2]=py;
	  	Servo_State[1][1]=px;
			Servo_State[1][2]=py;
			SYNC_Write_Buff(Servo_State,2);
			delay_ms(4); 
		}
		delay_ms(1000);
//		px=Receive_data[0];
//    py=Receive_data[1]; 	
	 	
//    angle_1=atan(py/px);
//		angle_1=angle_1*180/3.1415926;
//    for(i=0;i<=5;i++)
//  	{								  
//      delay_ms(10);
//	   LCD_ShowxNum(j,230,Receive_data[i],1,16,0x80);
//	    j+=30;
//		}
     
		  LED1_Toggle;
//    sprintf(buffer,"%5.6f",angle_1);
//		LCD_ShowString(30,230,16,16,16,buffer);
//		delay_ms(100);
//		HAL_UART_Transmit_IT(&UART2_Handler, buffer,5);
//		HAL_UART_Transmit(&UART2_Handler, buffer, 5, 0xfff);		
//		delay_ms(100);
		
		
	} 
}
