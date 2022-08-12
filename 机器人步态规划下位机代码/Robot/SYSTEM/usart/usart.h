#ifndef _USART_H
#define _USART_H
#include "sys.h"
#include "stdio.h"	
//////////////////////////////////////////////////////////////////////////////////	 


////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART2_RX 			0		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART1_RX 1
extern u8 Receive_data[10];	  	
extern UART_HandleTypeDef UART1_Handler; //UART���
extern UART_HandleTypeDef UART2_Handler; //UART���
extern u8 Receive_Data[11];
#define RXBUFFERSIZE   1 //�����С
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
u8 Check_Sum_State (void);
void Check_Receivedata(u8 *ReceiveStata,u8 *ServoStata);
#endif
