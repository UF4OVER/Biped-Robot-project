#include "usart.h"
#include "dma.h"
#include "lcd.h"
#include "servo.h"
u8 Receive_cnt=0,Matlab_cnt=0,Servo_No=0,cnt=0;
u8 Receive_data[10]={0}; //接受到的数据
u8 Matlab_data[21]={0};// matlab 仿真数据
u8 angle_7[20]={0};
u8 angle_8[20]={0};  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用os,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//采用USART3作为舵机收发数据串口
//********************************************************************************
//V1.0修改说明 
////////////////////////////////////////////////////////////////////////////////// 	  
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->ISR&0X40)==0);//循环发送,直到发送完毕   
  USART2->TDR=(u8)ch;	
	return ch;
}
#endif 
UART_HandleTypeDef UART2_Handler; //UART句柄
UART_HandleTypeDef UART1_Handler; //UART句柄
//初始化IO 串口2 
//bound:波特率
void uart_init(u32 bound)
{		
 	//UART2 初始化设置   
	UART2_Handler.Instance=USART2;					    //USART2
	UART2_Handler.Init.BaudRate=bound;				    //波特率
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART2_Handler);					    //HAL_UART_Init()会使能UART1
	
	UART1_Handler.Instance=USART1;					    //USART3
	UART1_Handler.Init.BaudRate=128000;				    //波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART1_Handler);
	
}

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIOB端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART2)//如果是串口2，进行串口2 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART2_CLK_ENABLE();			//使能USART2时钟
	  
		GPIO_Initure.Pin=GPIO_PIN_2;			//PA2
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART2;	//复用为USART2
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA2

		GPIO_Initure.Pin=GPIO_PIN_3;			//PA3、
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA3			
		#if EN_USART2_RX
	 __HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);		//开启接收中�
		 __HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//开启接收中断�
		HAL_NVIC_EnableIRQ(USART2_IRQn);				//使能USART3中断通道
		HAL_NVIC_SetPriority(USART2_IRQn,3,4);			//抢占优先级3，子优先级3
#endif	
		
	}
		if(huart->Instance==USART1)//如果是串口2，进行串口2 MSP初始化
		{
		
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART3时钟
	  
		GPIO_Initure.Pin=GPIO_PIN_10;			//PB10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//复用为USART3
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PB10

		GPIO_Initure.Pin=GPIO_PIN_9;			//PB11
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA3			
		#if EN_USART1_RX
	 __HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);		//开启接收中�
		 __HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//开启接收中断�
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART3中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//抢占优先级3，子优先级3
#endif	
		}
   }


//串口2中断服务程序
void USART2_IRQHandler(void)                	
{  
#if SYSTEM_SUPPORT_OS	 	//使用OS
  	OSIntEnter();    
#endif
if((__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_RXNE)!=RESET))  //空闲中断	
{
    HAL_UART_Receive(&UART2_Handler,&Receive_data[Receive_cnt], 1, 0xfff);
	   Receive_cnt++;

}
if((__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_IDLE)!=RESET))  //空闲中断
{
   	Receive_cnt=0;
	  __HAL_UART_CLEAR_IT(&UART2_Handler, UART_CLEAR_IDLEF);//清除中断标志位
//	  Check_Receivedata(&Receive_data[0],&Servo_State[0][0]);                                 //将查询回来的舵机的参数放入舵机position中
}
	  
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit(); 									 
#endif
}


//串口3中断服务程序
void USART1_IRQHandler(void)                	
{  
	u8 res=0;
	u8 *matlab_data=&res;
#if SYSTEM_SUPPORT_OS	 	//使用OS
  	OSIntEnter();    
#endif
if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //空闲中断	
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
if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_IDLE)!=RESET))  //空闲中断
{
   	Matlab_cnt=0;
	  __HAL_UART_CLEAR_IT(&UART1_Handler, UART_CLEAR_IDLEF);//清除中断标志位
//	  Check_Receivedata(&Receive_data[0],&Servo_State[0][0]);                                 //将查询回来的舵机的参数放入舵机position中
}
	  
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit(); 									 
#endif
}
void Check_Receivedata(u8 *ReceiveStata,u8 *ServoStata)  //将返回的位置数据放入舵机位置库
{
	u8 mid;
	u8 *RECEIVE_DATA[10],*SERVODATA[11][7];
	RECEIVE_DATA[0]=ReceiveStata;
	SERVODATA[0][0]=ServoStata;
	
	if(*RECEIVE_DATA[3]==0x04)//返回数据长度
	{
		mid=*RECEIVE_DATA[2];    //ID
		mid--;
		*SERVODATA[mid][1]=*RECEIVE_DATA[5];  
		*SERVODATA[mid][2]=*RECEIVE_DATA[6];
	}
}
