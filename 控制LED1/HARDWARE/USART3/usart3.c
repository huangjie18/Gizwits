#include "usart3.h"
#include "gizwits_product.h"

void USART3_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{
		res = USART_ReceiveData(USART3);
		gizPutData(&res,1); //����д�뵽������
	}
}

//��ʼ��IO����3
void usart3_Init(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//ʹ��GPIOBʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//ʹ��USART3ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	
	USART_DeInit(USART3); //��λ����3
	
	//GPIOB11 ����Ϊ USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	
	//GPIOB10 ����Ϊ USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	
	//GPIOB11 �� GPIOB10 ��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ� 50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ�� GPIOB11���� GPIOB10
	
	USART_InitStructure.USART_BaudRate = bound;//������
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	//�ֳ�Ϊ 8 λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits=USART_StopBits_1; //һ��ֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No; //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl=
	USART_HardwareFlowControl_None;
	//��Ӳ������������
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	//�շ�ģʽ
	USART_Init(USART3,&USART_InitStructure); //��ʼ������ 3
	USART_Cmd(USART3,ENABLE);//ʹ�ܴ���
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //�����ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);//����ָ���Ĳ�����ʼ�� VIC �Ĵ���
}