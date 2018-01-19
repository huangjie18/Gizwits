#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "usart3.h"
#include "key.h"
#include "gizwits_product.h"

//ALIENTEK ̽����STM32F407������ ʵ��8
//��ʱ���ж�ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK  
/**�û�����ǰ�豸״̬�ṹ��**/
extern dataPoint_t currentDataPoint;
u8 wifi_sta=0; //1:������ 0���Ͽ�
//Э���ʼ��
void Gizwits_Init(void)
{
	TIM3_Int_Init(10-1,8400-1); //1MSϵͳ��ʱ
	usart3_Init(9600);//WIFI��ʼ��
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	gizwitsInit();//��������ʼ��
}



int main(void)
{ 
	int key;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);        //��ʼ����ʱ����
	LED_Init();				//��ʼ��LED�˿�
	uart_init(115200);      //��ʼ��USART
	KEY_Init();             //��ʼ������
	Gizwits_Init();         //Э���ʼ��
 	printf("---������IOT-LED����ʵ��--------\r\n");
	printf("KEY1:AirLink����ģʽ\tKEY_UP:��λ\r\n\r\n");
	while(1)
	{
		userHandle();//�û��ɼ�
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��
		key=KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1����
		{
			printf("WIFI����AirLink����ģʽ\r\n");
			gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-linkģʽ����
		}
		if(key==WKUP_PRES)//KEY_UP����
		{
			printf("WIFI��λ,��������������\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ
		}
		delay_ms(200);
//		LED0=!LED0;
			
	};
}

