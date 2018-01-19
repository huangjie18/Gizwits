#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "usart3.h"
#include "key.h"
#include "gizwits_product.h"

//ALIENTEK 探索者STM32F407开发板 实验8
//定时器中断实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK  
/**用户区当前设备状态结构体**/
extern dataPoint_t currentDataPoint;
u8 wifi_sta=0; //1:已连接 0：断开
//协议初始化
void Gizwits_Init(void)
{
	TIM3_Int_Init(10-1,8400-1); //1MS系统定时
	usart3_Init(9600);//WIFI初始化
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
}



int main(void)
{ 
	int key;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);        //初始化延时函数
	LED_Init();				//初始化LED端口
	uart_init(115200);      //初始化USART
	KEY_Init();             //初始化按键
	Gizwits_Init();         //协议初始化
 	printf("---机智云IOT-LED控制实验--------\r\n");
	printf("KEY1:AirLink连接模式\tKEY_UP:复位\r\n\r\n");
	while(1)
	{
		userHandle();//用户采集
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理
		key=KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1按键
		{
			printf("WIFI进入AirLink连接模式\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link模式接入
		}
		if(key==WKUP_PRES)//KEY_UP按键
		{
			printf("WIFI复位,请重新配置连接\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位
		}
		delay_ms(200);
//		LED0=!LED0;
			
	};
}

