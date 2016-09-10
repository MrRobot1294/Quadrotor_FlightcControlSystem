#include "pid.h"

PID oula_roll_pid;//�⻷ŷ���ǵ�Pid
PID oula_pitch_pid;
PID oula_yaw_pid;

PID rate_roll_pid;//�ڻ����ٶȵ�pid
PID rate_pitch_pid;
PID rate_yaw_pid;

PID height_pid; //�߶�pid


//��ʼ��pid�Ĳ���
void pid_init(PID* pid,float kp,float ki,float kd,float limit){
  pid->p=kp;
	pid->i=ki;
	pid->d=kd;
	pid->i_limit=limit;
}

float pid_update(PID* pid,float desire,float measure){
	float result;
	pid->desire=desire;//����ֵ��ֵ���ڻ����⻷�������Ϊ����ֵ
  pid->error=pid->desire-measure;
	
	pid->integ+=pid->error*IMU_UPDATE_T;//�����
	
  if(pid->integ>pid->i_limit){  //�趨��������
		pid->integ=pid->i_limit;
	}
	if(pid->integ<-pid->i_limit){
		pid->integ=-pid->i_limit;
	}
	
	pid->deriv=(pid->error-pid->pre_error)*IMU_UPDATE_FREQ;
	
	pid->p_out=pid->error*pid->p;
	pid->i_out=pid->integ*pid->i;
	pid->d_out=pid->deriv*pid->d;
	
	result=pid->p_out+pid->i_out+pid->d_out;
	pid->pre_error=pid->error;//�������
	return result;
}
//�߶�Pid���㷨
float height_pid_update(PID* pid,float desire,float measure){
	float static result;
	pid->desire=desire;
	pid->error=pid->desire-measure;
	pid->integ=pid->error*IMU_UPDATE_T;
	pid->deriv=((pid->error-pid->pre_error)-(pid->pre_error-pid->more_pre_error))*IMU_UPDATE_FREQ;
	
	pid->p_out=(pid->error-pid->pre_error)*pid->p;
	pid->i_out=pid->i*pid->integ;
	pid->d_out=pid->d*pid->deriv;
	
	if(pid->i_out>pid->i_limit){ //�����޷�
	  pid->i_out=pid->i_limit;
	}
	if(pid->i_out<-pid->i_limit){
	  pid->i_out=-pid->i_limit;
	}
	result+=(pid->p_out  -  pid->i_out  +  pid->d_out);
	pid->pre_error=pid->error;
	pid->more_pre_error=pid->pre_error;
  return result;
}

















