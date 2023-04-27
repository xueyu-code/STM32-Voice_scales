/*

���߶��壺
		VCC--5V
		RXD--PB10
		TXD--PB11
		GND--GND
*/
#include "sys.h"
#include "delay.h"
#include "stdio.h"
#include "usart.h"
#include "syn6288.h"
#include "HX711.h"
#include "oled.h"
#include "usart2.h"
#include "timer.h"
#include "gizwits_product.h"



s32 Weight_Shiwu;
int say_flag=0;//����������־λ
int get_weight_flag=0,over_weight_flag=0;
int count=0;
uint32_t weight=0;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	USART3_Init(9600);
	Init_HX711pin();
	Get_Maopi();
	
	
	OLED_Init();
	OLED_ShowString(0,0,"weight:",16,1);
	OLED_Refresh();
	
	TIM3_Int_Init(71,999);				//һ���붨ʱ
    
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	usart2_init((u32)9600);//WIFI��ʼ��
	gizwitsInit();//��������ʼ��
	gizwitsInit();//��������ʼ��
	
//	gizwitsSetMode(WIFI_RESET_MODE);
	delay_ms(500);
//	gizwitsSetMode(WIFI_AIRLINK_MODE);
	
	gizwitsSetMode(WIFI_SOFTAP_MODE);

	delay_ms(1000);

	SYN6288_Say("���������޵ı�ҵ���");
	delay_ms(5000);
	delay_ms(5000);
	delay_ms(5000);
	SYN6288_Say("�����������ӳ�");
	delay_ms(5000);
	delay_ms(5000);
	SYN6288_Say("��ӭʹ��");
	delay_ms(5000);
	
	while(1)
	{		
		if(Get_Weight()>0)
		{
			weight=Get_Weight();
			OLED_ShowNum(80,0,weight,4,16,1);
			OLED_Refresh();
			get_weight_flag=1;
			count++;	
		}
		
		if(count==20&&say_flag==1)//�����ɼ�24���������ݲŲ���
		{
			SYN6288_Say("%d��",weight);
			delay_ms(4000);
		}
		
		
			//���ؾ���
			if(weight>=5000)
			{
				SYN6288_Say("����");
				delay_ms(3000);
			}
			
			
			//��ȡ������־λ����ͬʱ����
			if(Get_Weight()==0)
			{
				weight=0;
				OLED_ShowString(0,0,"weight:",16,1);
				OLED_ShowNum(80,0,weight,4,16,1);
				OLED_Refresh();
				count=0;
			
			}
			//�����ϱ��ƶ�
		userHandle();
    gizwitsHandle((dataPoint_t *)&currentDataPoint);
				
		
	}
}


