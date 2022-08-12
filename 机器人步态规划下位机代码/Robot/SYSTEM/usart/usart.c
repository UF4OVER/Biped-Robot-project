#include "usart.h"
#include "dma.h"
#include "lcd.h"
#include "servo.h"
u8 Receive_cnt=0,Matlab_cnt=0,Servo_No=0,cnt=0;
u8 Receive_data[10]={0}; //���ܵ�������
u8 Matlab_data[21]={0};// matlab ��������
u8 angle_7[20]={0};
u8 angle_8[20]={0};  
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//����USART3��Ϊ����շ����ݴ���
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	  
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->ISR&0X40)==0);//ѭ������,ֱ���������   
  USART2->TDR=(u8)ch;	
	return ch;
}
#endif 
UART_HandleTypeDef UART2_Handler; //UART���
UART_HandleTypeDef UART1_Handler; //UART���
//��ʼ��IO ����2 
//bound:������
void uart_init(u32 bound)
{		
 	//UART2 ��ʼ������   
	UART2_Handler.Instance=USART2;					    //USART2
	UART2_Handler.Init.BaudRate=bound;				    //������
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART2_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	UART1_Handler.Instance=USART1;					    //USART3
	UART1_Handler.Init.BaudRate=128000;				    //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);
	
}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIOB�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART2)//����Ǵ���2�����д���2 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART2_CLK_ENABLE();			//ʹ��USART2ʱ��
	  
		GPIO_Initure.Pin=GPIO_PIN_2;			//PA2
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART2;	//����ΪUSART2
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA2

		GPIO_Initure.Pin=GPIO_PIN_3;			//PA3��
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA3			
		#if EN_USART2_RX
	 __HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);		//���������ж
		 __HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж��
		HAL_NVIC_EnableIRQ(USART2_IRQn);				//ʹ��USART3�ж�ͨ��
		HAL_NVIC_SetPriority(USART2_IRQn,3,4);			//��ռ���ȼ�3�������ȼ�3
#endif	
		
	}
		if(huart->Instance==USART1)//����Ǵ���2�����д���2 MSP��ʼ��
		{
		
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART3ʱ��
	  
		GPIO_Initure.Pin=GPIO_PIN_10;			//PB10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//����ΪUSART3
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PB10

		GPIO_Initure.Pin=GPIO_PIN_9;			//PB11
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA3			
		#if EN_USART1_RX
	 __HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);		//���������ж
		 __HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж��
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART3�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
#endif	
		}
   }


//����2�жϷ������
void USART2_IRQHandler(void)                	
{  
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
  	OSIntEnter();    
#endif
if((__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�	
{
    HAL_UART_Receive(&UART2_Handler,&Receive_data[Receive_cnt], 1, 0xfff);
	   Receive_cnt++;

}
if((__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_IDLE)!=RESET))  //�����ж�
{
   	Receive_cnt=0;
	  __HAL_UART_CLEAR_IT(&UART2_Handler, UART_CLEAR_IDLEF);//����жϱ�־λ
//	  Check_Receivedata(&Receive_data[0],&Servo_State[0][0]);                                 //����ѯ�����Ķ���Ĳ���������position��
}
	  
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit(); 									 
#endif
}


//����3�жϷ������
void USART1_IRQHandler(void)                	
{  
	u8 res=0;
	u8 *matlab_data=&res;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
  	OSIntEnter();    
#endif
if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�	
{
    HAL_UART_Receive(&UART1_Handler,matlab_data, 1, 0xfff);
	  if(*matlab_data==207)   Servo_No=7;			
	  if(*matlab_data==208)   Servo_No=8;
//	LCD_ShowxNum(40,80,Servo_No,2,32,0);
//	LCD_ShowxNum(40,160,*matlab_data,2,32,0);
		switch(Servo_No)
		{
			case 7: angle_7[Matlab_cnt]=*matlab_data;break;			
			case 8: angle_8[Matlab_cnt]=*matlab_data;break;
			default: break;
		}
	   Matlab_cnt++;
}
if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_IDLE)!=RESET))  //�����ж�
{
   	Matlab_cnt=0;
	  __HAL_UART_CLEAR_IT(&UART1_Handler, UART_CLEAR_IDLEF);//����жϱ�־λ
//	  Check_Receivedata(&Receive_data[0],&Servo_State[0][0]);                                 //����ѯ�����Ķ���Ĳ���������position��
}
	  
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit(); 									 
#endif
}
void Check_Receivedata(u8 *ReceiveStata,u8 *ServoStata)  //�����ص�λ�����ݷ�����λ�ÿ�
{
	u8 mid;
	u8 *RECEIVE_DATA[10],*SERVODATA[11][7];
	RECEIVE_DATA[0]=ReceiveStata;
	SERVODATA[0][0]=ServoStata;
	
	if(*RECEIVE_DATA[3]==0x04)//�������ݳ���
	{
		mid=*RECEIVE_DATA[2];    //ID
		mid--;
		*SERVODATA[mid][1]=*RECEIVE_DATA[5];  
		*SERVODATA[mid][2]=*RECEIVE_DATA[6];
	}
}
