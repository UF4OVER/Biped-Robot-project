#include "usart.h"
#include "dma.h"
#include "lcd.h"
#include "servo.h"
#include "led.h"
u8 Receive_cnt=0,Receive_PC_cnt=0;uint8_t Receive_PC_Data[400]={0}; 
u8 Receive_data[10]={0}; //���ܵ�������
uint8_t TxBuffer3[400]={0};  
uint8_t RxBuffer4[400]={0};
uint8_t TxCounter4;
uint8_t RxCounter4=0;
uint8_t TxBuffer1[400]; 
uint8_t Rec_Len;


 
 
 
uint8_t rec_f,tx_flag,flash_led;
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//����USART2��Ϊ����շ����ݴ���

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
	while((USART1->ISR&0X40)==0);//ѭ������,ֱ���������   
  USART1->TDR=(u8)ch;	
	return ch;
}
#endif 
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	

u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���
//#if EN_USART2_RX   //���ʹ���˽���
//����2�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
UART_HandleTypeDef UART1_Handler;
UART_HandleTypeDef UART2_Handler; //UART���
UART_HandleTypeDef UART3_Handler; 
UART_HandleTypeDef UART7_Handler; 
UART_HandleTypeDef UART5_Handler;
//��ʼ��IO ����2 
//bound:������
void uart_init(void)
{
	uart1_init(128000);  //connect pc
	uart2_init(1000000); //servo
//	uart3_init(9600);
//	uart7_init(9600);//gps ���ű���

}
void uart1_init(u32 bound)
{	
	//UART ��ʼ������
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	//HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}

void uart2_init(u32 bound)
{		
 	//UART2 ��ʼ������   
	UART2_Handler.Instance=USART2;					    //USART2
	UART2_Handler.Init.BaudRate=bound;				    //������
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART2_Handler);					    //HAL_UART_Init()��ʹ��UART2
}
void uart3_init(u32 bound)
{		
 	//UART3 ��ʼ������   
	UART3_Handler.Instance=USART3;					    //USART3
	UART3_Handler.Init.BaudRate=bound;				    //������
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //��ģʽ
	HAL_UART_Init(&UART3_Handler);					    //HAL_UART_Init()��ʹ��UART3
}

void uart7_init(u32 bound)
{		
 	//UART2 ��ʼ������   
	UART7_Handler.Instance=UART7;					    //USART2
	UART7_Handler.Init.BaudRate=bound;				    //������
	UART7_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART7_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART7_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART7_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART7_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART7_Handler);					    //HAL_UART_Init()��ʹ��UART1
}


//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIOB�˿�����
	GPIO_InitTypeDef GPIO_Initure;
		if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA10
		
#if EN_USART1_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�
		__HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);		//���������ж�
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
#endif	
	}
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
		HAL_NVIC_SetPriority(USART2_IRQn,3,2);			//��ռ���ȼ�3�������ȼ�3
#endif	
	}	

	
	if(huart->Instance==UART7)//����Ǵ���7�����д���4 MSP��ʼ��
	{
		__HAL_RCC_GPIOF_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_UART7_CLK_ENABLE();			//ʹ��USART2ʱ��
	  
		GPIO_Initure.Pin=GPIO_PIN_6;			//PF6
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF8_UART7;	//����ΪUART7
		HAL_GPIO_Init(GPIOF,&GPIO_Initure);	   	//��ʼ��PF6
		
		GPIO_Initure.Pin=GPIO_PIN_7;			//PF7		
		HAL_GPIO_Init(GPIOF,&GPIO_Initure);	   	//��ʼ��PF7
	#if EN_UART7_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж��
		HAL_NVIC_EnableIRQ(UART7_IRQn);				//ʹ��USART4�ж�ͨ��
		HAL_NVIC_SetPriority(UART7_IRQn,4,2);			//��ռ���ȼ�4�������ȼ2
#endif	
	}


 }



//����1�жϷ������
void USART1_IRQHandler(void)                	
{ 
	OS_ERR err;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�
	{
    HAL_UART_Receive(&UART1_Handler,&Receive_PC_Data[Receive_PC_cnt], 1, 0xfff);
		Receive_PC_cnt++;
	}
		if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_IDLE)!=RESET))  //�����ж�
	{
    Receive_PC_cnt=0;
		__HAL_UART_CLEAR_IT(&UART1_Handler, UART_CLEAR_IDLEF);//����жϱ�־λ
		OSSemPost(&PC_SEM,OS_OPT_POST_1,&err);//�����ź���
		
	}
	
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
}  
 //����2�жϷ������   servo data
void USART2_IRQHandler(void)                	
{  

		OS_ERR err;
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
		OSSemPost(&SERVO_SEM,OS_OPT_POST_1,&err);//�����ź���

//	  Check_Receivedata(&Receive_data[0],&Servo_State[0][0]);                                 //����ѯ�����Ķ���Ĳ���������position��
}
	  
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit(); 									 
#endif
}




void UART7_IRQHandler(void)    //GPS            	
{  
	OS_ERR err;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
  	OSIntEnter();    
#endif
if((__HAL_UART_GET_FLAG(&UART7_Handler,UART_FLAG_RXNE)!=RESET))  
{
    HAL_UART_Receive(&UART7_Handler,&RxBuffer4[RxCounter4], 1, 0xfff);
	  RxCounter4++;
		if(RxBuffer4[RxCounter4-1]=='$')  //�ж���ʼ��־
    {
	   	RxBuffer4[0]='$';
			RxCounter4=1; 
		}    
		if(RxBuffer4[RxCounter4-1]==0x0a) //�жϽ�����־�Ƿ���0x0d 0x0a
		{	  	
	  		Rec_Len=RxCounter4;
	  		RxCounter4=0; 														 
				OSSemPost(&GPS_SEM,OS_OPT_POST_1,&err);//�����ź���
    }
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
