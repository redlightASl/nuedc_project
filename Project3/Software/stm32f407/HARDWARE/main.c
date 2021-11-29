
//接线：SCL-PB6,SDA-PB7
//启动时yaw置0
//务必确保杜邦线稳定可靠连接紧密！！！！！！！！！！！
//这点十分重要！！！！！！！！！！！！！！！！！！！！
//当输出全是0的时候请再确认是不是线的问题！！！！！！
//输出格式：roll,yaw,pitch

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "mpu9250.h"
#include "inv_mpu.h"

int main(void){ 
	u8 t=0;			
	float pitch,roll,yaw; 		//欧拉角
	delay_init(168);      //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200	
	MPU9250_Init();//初始化MPU9250
	mpu_dmp_init();//初始化MPU9250 DMP
  
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
