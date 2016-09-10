#ifndef __CONTROL_H
#define __CONTROL_H
#include "includes.h"
//油门的极限值，极大与极小
#define THRUST_MAX    1950 
#define THRUST_MIN    1000
////////////////////////////////////////////////////////////////////////////////////////////////
#define x_rate_pid_integ_limit                     100
#define y_rate_pid_integ_limit                     100
#define z_rate_pid_integ_limit                     100

#define oula_roll_pid_integ_limit                  100
#define oula_pitch_pid_integ_limit                 100
#define oula_yaw_pid_integ_limit                   100

#define pid_height_limit                         400
////外环参数
//#define oula_roll_kp                        95
//#define oula_roll_ki                        0.8
//#define oula_roll_kd                        5.3

//#define oula_pitch_kp                       95
//#define oula_pitch_ki                       0.8
//#define oula_pitch_kd                       5.3

////内环参数
//#define gyro_x_kp                           0.06
//#define gyro_x_ki                           0
//#define gyro_x_kd                           0.000015

//#define gyro_y_kp                           0.06
//#define gyro_y_ki                           0
//#define gyro_y_kd                           0.000015
////欧拉角

//#define oula_yaw_kp                         50
//#define oula_yaw_ki                         0
//#define oula_yaw_kd                         10

//#define gyro_z_kp                           0.04
//#define gyro_z_ki                           0
//#define gyro_z_kd                           0.0001

////高度pid的控制
//#define height_p                            0
//#define height_i                            0
//#define height_d                            0

//外环



void control_motor(float Roll,float Pitch,float Yaw,
	                 short gyrox,short gyroy,short gyroz,
						       float measure_height,
						       float desire_roll,float deisre_pitch,float desire_yaw,float desire_height,
						       float* pid_roll_output,float* pid_pitch_output,float* pid_yaw_output,float* pid_height_output);

void set_motor(u32 base_thrust,u16* motor1,u16* motor2,u16* motor3,u16* motor4,
	               float pid_roll_out,float pid_pitch_out,float pid_yaw_out,float pid_height_out);




#endif





