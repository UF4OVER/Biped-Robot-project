#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"
#include "includes.h"
//////////////////////////////////////////////////////////////////////////////////	 


////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_UART7_RX 			1	
#define EN_USART3_RX      1
#define EN_UART5_RX 			1		
extern u8 Receive_data[10];	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern UART_HandleTypeDef UART1_Handler; //UART���

#define RXBUFFERSIZE   1 //�����С
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer

extern UART_HandleTypeDef UART3_Handler; //UART���
extern UART_HandleTypeDef UART2_Handler; //UART���
extern UART_HandleTypeDef UART7_Handler; 

#define RXBUFFERSIZE   1 //�����С
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer
extern uint8_t Receive_PC_Data[400]; 
extern uint8_t TxBuffer3[400];  
extern uint8_t RxBuffer4[400];
extern uint8_t TxCounter4;
extern uint8_t Rec_Len;
extern OS_SEM GPS_SEM,SERVO_SEM,PC_SEM ;
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(void);
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);
void uart7_init(u32 bound);
void uart5_init(u32 bound);
u8 Check_Sum_State (void);
void Check_Receivedata(u8 *ReceiveStata,u8 *ServoStata);
#endif
