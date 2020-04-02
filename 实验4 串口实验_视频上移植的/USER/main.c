#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "nvic.h"
#include "dht11.h"
#include "oled_iic.h"
#include "fy_network.h"
#include "fy_mqtt.h"

uint8_t wendu = 0,shidu = 0;
uint8_t string[100];

u8 txbuf[256];
u8 rxbuf[256];

uint8_t cnt_1_5_s = 0;

void ESP_INIT(void);

int main(void){		

    delay_init();	    	 //��ʱ������ʼ��	  
    uart1_init(115200);	 //���ڳ�ʼ��Ϊ115200
    uart3_init(115200);	 //���ڳ�ʼ��Ϊ115200
    LED_Init();			     //LED�˿ڳ�ʼ��
//    KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�

    Timer2_Init(1000,72);	//��ʼ�� Timer2 1ms �ж�һ��
    
    DHT11Configuration();
    
    I2C_Configuration();
	
	OLED_Init();
	OLED_CLS();
	OLED_ON();
	
    
    nvic_init();			//���� �жϵ� ���ȼ� 
    
//    sprintf((char*)string,"shidu");
//	OLED_ShowStr(15,2,string,2);
//    
    printf ("cpu start!\r\n");
    
    ESP_INIT();//ESP8266��ʼ��
 
    while(1)
    {
        if(cnt_1_5_s == 1){
            cnt_1_5_s = 0;
            DHT11_Read_Data(&wendu,&shidu);
            sprintf((char *)string,"%d",wendu);
			_mqtt.PublishData("/cai/wendu",(char *)string,0);
            sprintf((char *)string,"%d",shidu);
			_mqtt.PublishData("/cai/shidu",(char *)string,0);
//            printf("yifadu~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
            
        }
        
        if(_mqtt.rxlen){
			Mqtt_Progress(_mqtt.rxbuf,_mqtt.rxlen);	
			memset(_mqtt.rxbuf,0,_mqtt.rxlen);
			_mqtt.rxlen = 0;
		}
//        DHT11_Read_Data(&wendu,&shidu);
//        delay_ms(1000);
//        printf("wendu = %d , shidu = %d\r\n",wendu,shidu);
    }	 
}

int sta = 0;

void ESP_INIT(void){
	if(_net.Init(rxbuf,sizeof(rxbuf),txbuf,sizeof(txbuf))!=0){
		printf("Net Init OK!\r\n");
		sta++;
	}
	else{
		printf("Net Init Error!\r\n");		sta=0;
	}
	if(sta==1){
		if(_net.ConnectAP("ipone","123456321")!=0){
//		if(_net.ConnectAP("iPhone","88888888")!=0){
			printf("Conncet AP OK!\r\n");
			sta++;
		}
		else {
			printf("Conncet AP Error!\r\n");
			sta=0;
		}
	}
    if(sta==2){	
        if(_net.ConnectServer("TCP","iizyqwf.mqtt.iot.gz.baidubce.com",1883)!=0) {//����TCP
			printf("Conncet Server OK!\r\n");
			sta++;
		}else{
			printf("Conncet Server Error!\r\n");	
			sta=0;
		}
	}
    if(sta==3){
		//��¼MQTT
		_mqtt.Init(rxbuf,sizeof(rxbuf),txbuf,sizeof(txbuf));		
		if(_mqtt.Connect(
			"caisiyu_danpianji",
            "iizyqwf/caisiyu",//Username
			"pAOMcfGxse9O9vZC"//Password
			) != 0){
			printf("Enter MQTT OK!\r\n");
			sta++;
		}else{		
			printf("Enter MQTT Error!\r\n");
			sta=0;
		}	
	}
	if(sta==4){
		if(_mqtt.SubscribeTopic("/cai/controlLED",0,1) != 0){
			printf("SubscribeTopic OK!\r\n");
		}else{
			printf("SubscribeTopic Error!\r\n");
		}
	}
	
}

