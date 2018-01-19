#include "usart3.h"
#include "gizwits_product.h"

void USART3_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{
		res = USART_ReceiveData(USART3);
		gizPutData(&res,1); //数据写入到缓存区
	}
}

//初始化IO串口3
void usart3_Init(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//使能GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//使能USART3时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	
	USART_DeInit(USART3); //复位串口3
	
	//GPIOB11 复用为 USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	
	//GPIOB10 复用为 USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	
	//GPIOB11 和 GPIOB10 初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度 50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化 GPIOB11，和 GPIOB10
	
	USART_InitStructure.USART_BaudRate = bound;//波特率
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	//字长为 8 位数据格式
	USART_InitStructure.USART_StopBits=USART_StopBits_1; //一个停止位
	USART_InitStructure.USART_Parity=USART_Parity_No; //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl=
	USART_HardwareFlowControl_None;
	//无硬件数据流控制
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	//收发模式
	USART_Init(USART3,&USART_InitStructure); //初始化串口 3
	USART_Cmd(USART3,ENABLE);//使能串口
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //开启中断
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //子优先级 3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ 通道使能
	NVIC_Init(&NVIC_InitStructure);//根据指定的参数初始化 VIC 寄存器
}