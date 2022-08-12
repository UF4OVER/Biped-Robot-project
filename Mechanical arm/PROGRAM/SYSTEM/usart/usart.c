#include "usart.h"
#include "dma.h"
#include "lcd.h"
#include "servo.h"
u8 Receive_cnt=0;
u8 Receive_data[10]={0}; //���ܵ�������
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

//#if EN_USART2_RX   //���ʹ���˽���
//����2�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ

UART_HandleTypeDef UART2_Handler; //UART���
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
   }


//����3�жϷ������
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
//#endif
//����2�жϷ������

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
