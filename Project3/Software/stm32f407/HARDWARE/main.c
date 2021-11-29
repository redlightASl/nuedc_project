
//���ߣ�SCL-PB6,SDA-PB7
//����ʱyaw��0
//���ȷ���Ű����ȶ��ɿ����ӽ��ܣ���������������������
//���ʮ����Ҫ����������������������������������������
//�����ȫ��0��ʱ������ȷ���ǲ����ߵ����⣡����������
//�����ʽ��roll,yaw,pitch

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "mpu9250.h"
#include "inv_mpu.h"

int main(void){ 
	u8 t=0;			
	float pitch,roll,yaw; 		//ŷ����
	delay_init(168);      //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200	
	MPU9250_Init();//��ʼ��MPU9250
	mpu_dmp_init();//��ʼ��MPU9250 DMP
  
	printf("Start running.\r\n");
	
	while(1) {		 
		if(mpu_mpl_get_data(&pitch,&roll,&yaw)==0){	
			if((t%10)==0){ 
				printf("%.2f,%.2f,%.2f\r\n",roll,yaw,pitch); 
				t=0;
			}
		}
		t++; 
	} 
}
