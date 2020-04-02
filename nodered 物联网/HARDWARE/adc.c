#include "adc.h"

float AD_Read(void){
	float temp;
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ��ָ���� ADC �����ת����������
	temp = ADC_GetConversionValue(ADC1)*3.3/0xfff;// stm32 ��ADC��12λ�� ���������� 0xfff��3.3/0xfff�õ��ֱ���
	return temp;
}

void AD_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef  ADC_InitStructure;
	
	//ʹ�� ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	//GPIOB.0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // �����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//�涨��ģ��ת���������ǵ��Σ���ͨ����ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//ģ��ת������������ģʽ��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;// ADC �������ұ߶���
	ADC_InitStructure.ADC_NbrOfChannel = 1;//�涨��˳����й���ת���� ADC ͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);
	//����ָ�� ADC �Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_13Cycles5);//ADC1,ADC ͨ��8,���������˳��,����ʱ��Ϊ 13.5 ����
	ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);//AWDOG �ж�����
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);//����ָ���� ADC ��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));//��ȡ ADC ����У׼�Ĵ�����״̬
	ADC_StartCalibration(ADC1);//��ʼָ�� ADC ��У׼״̬
	while(ADC_GetCalibrationStatus(ADC1));//��ȡָ�� ADC ��У׼����
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ��ָ���� ADC �����ת����������
}




