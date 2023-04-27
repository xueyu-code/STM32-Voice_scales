/*

接线定义：
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
int say_flag=0;//语音播报标志位
int get_weight_flag=0,over_weight_flag=0;
int count=0;
uint32_t weight=0;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init();
	USART3_Init(9600);
	Init_HX711pin();
	Get_Maopi();
	
	
	OLED_Init();
	OLED_ShowString(0,0,"weight:",16,1);
	OLED_Refresh();
	
	TIM3_Int_Init(71,999);				//一毫秒定时
    
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	usart2_init((u32)9600);//WIFI初始化
	gizwitsInit();//缓冲区初始化
	gizwitsInit();//缓冲区初始化
	
//	gizwitsSetMode(WIFI_RESET_MODE);
	delay_ms(500);
//	gizwitsSetMode(WIFI_AIRLINK_MODE);
	
	gizwitsSetMode(WIFI_SOFTAP_MODE);

	delay_ms(1000);

	SYN6288_Say("我是张荣洲的毕业设计");
	delay_ms(5000);
	delay_ms(5000);
	delay_ms(5000);
	SYN6288_Say("智能语音电子秤");
	delay_ms(5000);
	delay_ms(5000);
	SYN6288_Say("欢迎使用");
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
		
		if(count==20&&say_flag==1)//连续采集24次重量数据才播报
		{
			SYN6288_Say("%d克",weight);
			delay_ms(4000);
		}
		
		
			//超重警告
			if(weight>=5000)
			{
				SYN6288_Say("超重");
				delay_ms(3000);
			}
			
			
			//获取重量标志位重置同时清零
			if(Get_Weight()==0)
			{
				weight=0;
				OLED_ShowString(0,0,"weight:",16,1);
				OLED_ShowNum(80,0,weight,4,16,1);
				OLED_Refresh();
				count=0;
			
			}
			//数据上报云端
		userHandle();
    gizwitsHandle((dataPoint_t *)&currentDataPoint);
				
		
	}
}


