#ifndef __PID_H
#define __PID_H
#include "includes.h"
#include "math.h"

#define IMU_UPDATE_FREQ                     200
#define IMU_UPDATE_T                        1.0f/IMU_UPDATE_FREQ

typedef struct{
  float p;
	float i;
	float d;
	
	float desire;
	float error;
	float pre_error;
	
	float integ;
	float deriv;
	
	float p_out;
	float i_out;
	float d_out;
	
	float i_limit;
	float more_pre_error;
}PID;

extern PID oula_roll_pid;//外环欧拉角的Pid
extern PID oula_pitch_pid;
extern PID oula_yaw_pid;

extern PID rate_roll_pid;//内环角速度的pid
extern PID rate_pitch_pid;
extern PID rate_yaw_pid;

extern PID height_pid;//高度pid

void pid_init(PID* pid,float kp,float ki,float kd,float limit);
float pid_update(PID* pid,float desire,float measure);
float height_pid_update(PID* pid,float desire,float measure);







#endif



