#include "my6050data.h"
#include "anbt_i2c.h"
#include "anbt_dmp_mpu6050.h"
#include "anbt_dmp_driver.h"
#include "anbt_dmp_fun.h"

#define q30  1073741824.0f
u8 test_flag;

//6050����������
void mpu6050_init(void){
  ANBT_I2C_Configuration();	//i2c������
	DMP_Delay(5000000);//dmp���õ���ʱ���ȴ�8�����ݲ��ȶ�
	AnBT_DMP_MPU6050_Init();//dmp������	
}


//����dmp���������,���ҽ����˴��������ݵķ���ԭ�������û�г���*(-1)�����ڷ������ֶ���
void yaw_roll_pitch_gyro_data(float* Roll,float* Pitch,float* Yaw,short* Gyro_x,short* Gyro_y,short* Gyro_z)
{
	unsigned long sensor_timestamp;
	short gyro[3], accel[3], sensors;//�����Ǵ�����飬���ٶȴ�����飬����״̬��
	unsigned char more;
	long quat[4];//��Ԫ���������
	//float Yaw=0.00,Roll,Pitch;//ŷ����
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;//������̬�����õ��ı���
		 test_flag=dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);	//��������൱��Ҫ
 		 if ( sensors & INV_WXYZ_QUAT )
		 {
			 q0=quat[0] / q30;
			 q1=quat[1] / q30;
			 q2=quat[2] / q30;
			 q3=quat[3] / q30;
			 *Pitch = -asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
			 *Roll = -atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
			 *Yaw =  atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;		//�о�û�м�ֵ��ע��			 
		 }
		 if(sensors&INV_XYZ_GYRO)//�����ǵ�����
		 {
		   *Gyro_x=-gyro[0];//pitch_rate
			 *Gyro_y=-gyro[1];//roll_rate
			 *Gyro_z=gyro[2];//yaw_rate		 
		 }
}

