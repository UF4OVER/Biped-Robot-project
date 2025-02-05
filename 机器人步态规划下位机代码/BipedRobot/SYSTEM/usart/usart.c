#include "usart.h"
#include "dma.h"
#include "lcd.h"
#include "servo.h"
#include "led.h"
u8 Receive_cnt=0,Receive_PC_cnt=0;uint8_t Receive_PC_Data[400]={0}; 
u8 Receive_data[10]={0}; //接受到的数据
uint8_t TxBuffer3[400]={0};  
uint8_t RxBuffer4[400]={0};
uint8_t TxCounter4;
uint8_t RxCounter4=0;
uint8_t TxBuffer1[400]; 
uint8_t Rec_Len;


 
 
 
uint8_t rec_f,tx_flag,flash_led;
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用os,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//采用USART2作为舵机收发数据串口

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
	while((USART1->ISR&0X40)==0);//循环发送,直到发送完毕   
  USART1->TDR=(u8)ch;	
	return ch;
}
#endif 
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	

u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
//#if EN_USART2_RX   //如果使能了接收
//串口2中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
UART_HandleTypeDef UART1_Handler;
UART_HandleTypeDef UART2_Handler; //UART句柄
UART_HandleTypeDef UART3_Handler; 
UART_HandleTypeDef UART7_Handler; 
UART_HandleTypeDef UART5_Handler;
//初始化IO 串口2 
//bound:波特率
void uart_init(void)
{
	uart1_init(128000);  //connect pc
	uart2_init(1000000); //servo
//	uart3_init(9600);
//	uart7_init(9600);//gps 引脚被用

}
void uart1_init(u32 bound)
{	
	//UART 初始化设置
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()会使能UART1
	
	//HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  
}

void uart2_init(u32 bound)
{		
 	//UART2 初始化设置   
	UART2_Handler.Instance=USART2;					    //USART2
	UART2_Handler.Init.BaudRate=bound;				    //波特率
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART2_Handler);					    //HAL_UART_Init()会使能UART2
}
void uart3_init(u32 bound)
{		
 	//UART3 初始化设置   
	UART3_Handler.Instance=USART3;					    //USART3
	UART3_Handler.Init.BaudRate=bound;				    //波特率
	UART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART3_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART3_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //收模式
	HAL_UART_Init(&UART3_Handler);					    //HAL_UART_Init()会使能UART3
}

void uart7_init(u32 bound)
{		
 	//UART2 初始化设置   
	UART7_Handler.Instance=UART7;					    //USART2
	UART7_Handler.Init.BaudRate=bound;				    //波特率
	UART7_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	UART7_Handler.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	UART7_Handler.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	UART7_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	UART7_Handler.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&UART7_Handler);					    //HAL_UART_Init()会使能UART1
}


//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIOB端口设置
	GPIO_InitTypeDef GPIO_Initure;
		if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//复用为USART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
		
#if EN_USART1_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//开启接收中断
		__HAL_UART_ENABLE_IT(huart,UART_IT_IDLE);		//开启空闲中断
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//抢占优先级3，子优先级3
#endif	
	}
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
		HAL_NVIC_SetPriority(USART2_IRQn,3,2);			//抢占优先级3，子优先级3
#endif	
	}	

	
	if(huart->Instance==UART7)//如果是串口7，进行串口4 MSP初始化
	{
		__HAL_RCC_GPIOF_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_UART7_CLK_ENABLE();			//使能USART2时钟
	  
		GPIO_Initure.Pin=GPIO_PIN_6;			//PF6
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF8_UART7;	//复用为UART7
		HAL_GPIO_Init(GPIOF,&GPIO_Initure);	   	//初始化PF6
		
		GPIO_Initure.Pin=GPIO_PIN_7;			//PF7		
		HAL_GPIO_Init(GPIOF,&GPIO_Initure);	   	//初始化PF7
	#if EN_UART7_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//开启接收中断�
		HAL_NVIC_EnableIRQ(UART7_IRQn);				//使能USART4中断通道
		HAL_NVIC_SetPriority(UART7_IRQn,4,2);			//抢占优先级4，子优先�2
#endif	
	}


 }



//串口1中断服务程序
void USART1_IRQHandler(void)                	
{ 
	OS_ERR err;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断
	{
    HAL_UART_Receive(&UART1_Handler,&Receive_PC_Data[Receive_PC_cnt], 1, 0xfff);
		Receive_PC_cnt++;
	}
		if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_IDLE)!=RESET))  //接收中断
	{
    Receive_PC_cnt=0;
		__HAL_UART_CLEAR_IT(&UART1_Handler, UART_CLEAR_IDLEF);//清除中断标志位
		OSSemPost(&PC_SEM,OS_OPT_POST_1,&err);//发送信号量
		
	}
	
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
}  
 //串口2中断服务程序   servo data
void USART2_IRQHandler(void)                	
{  

		OS_ERR err;
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
		OSSemPost(&SERVO_SEM,OS_OPT_POST_1,&err);//发送信号量

//	  Check_Receivedata(&Receive_data[0],&Servo_State[0][0]);                                 //将查询回来的舵机的参数放入舵机position中
}
	  
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit(); 									 
#endif
}




void UART7_IRQHandler(void)    //GPS            	
{  
	OS_ERR err;
#if SYSTEM_SUPPORT_OS	 	//使用OS
  	OSIntEnter();    
#endif
if((__HAL_UART_GET_FLAG(&UART7_Handler,UART_FLAG_RXNE)!=RESET))  
{
    HAL_UART_Receive(&UART7_Handler,&RxBuffer4[RxCounter4], 1, 0xfff);
	  RxCounter4++;
		if(RxBuffer4[RxCounter4-1]=='$')  //判断起始标志
    {
	   	RxBuffer4[0]='$';
			RxCounter4=1; 
		}    
		if(RxBuffer4[RxCounter4-1]==0x0a) //判断结束标志是否是0x0d 0x0a
		{	  	
	  		Rec_Len=RxCounter4;
	  		RxCounter4=0; 														 
				OSSemPost(&GPS_SEM,OS_OPT_POST_1,&err);//发送信号量
    }
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
