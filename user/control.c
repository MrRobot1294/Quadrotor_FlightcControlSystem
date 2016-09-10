#include "control.h"
#include "pid.h"
#include "my_pwm.h"
////////////////////////////////////////////////////////////////////////////////////////////////
//初始化pid控制参数，每次调用控制函数只需要调用初始化一边pid的控制参数
u8 control_flag_ok=1;
//油门的修饰量
#define thrust1_modify  265
#define thrust2_modify  95
#define thrust3_modify  265
#define thrust4_modify  95

//串级pid参数
float oula_roll_kp=46;
float oula_roll_ki=8;
float oula_roll_kd=1.8; 

float gyro_x_kp=0.08;
float gyro_x_ki=0;
float gyro_x_kd=0.00032;
//////////////////////////////////////////////////////////////////

float oula_pitch_kp=51;//65;
float oula_pitch_ki=8;//8;
float oula_pitch_kd=1.2;//30;

float gyro_y_kp=0.08;//0.06;
float gyro_y_ki=0;//0.1;
float gyro_y_kd=0.00032;

/////////////////////////////////////////////////////////////////////
float oula_yaw_kp=10;
float oula_yaw_ki=0;
float oula_yaw_kd=0.5;//100

float gyro_z_kp=0.05;//0.06
float gyro_z_ki=0.05;
float gyro_z_kd=0.0002;       

////////////////////////////////////////////////////////////////////////
//位置式pid的参数
//float height_kp=1;
//float height_ki=0;
//float height_kd=30;

//增量式pid的参数
float height_kp=0.2;//0.6
float height_ki=-0.4;
float height_kd=0;

/*
pid_update(1,2,3);
1.带入计算的pid结构体
2.期望值
3.实际测量的值
*/

//不带高度值得pid控制算法
void control_motor(float Roll,float Pitch,float Yaw,
	           short gyrox,short gyroy,short gyroz,
						 float measure_height,
						 float desire_roll,float deisre_pitch,float desire_yaw,float desire_height,
						 float* pid_roll_output,float* pid_pitch_output,float* pid_yaw_output,float* pid_height_output){
		//初始化pid的参数，装载参数值
		if(control_flag_ok){
			pid_init(&oula_roll_pid , oula_roll_kp , oula_roll_ki ,  oula_roll_kd  ,  oula_roll_pid_integ_limit);
			pid_init(&oula_pitch_pid, oula_pitch_kp, oula_pitch_ki,  oula_pitch_kd ,  oula_pitch_pid_integ_limit);
			pid_init(&oula_yaw_pid  , oula_yaw_kp  , oula_yaw_ki  ,  oula_yaw_kd   ,  oula_yaw_pid_integ_limit);
			pid_init(&rate_roll_pid , gyro_x_kp    , gyro_x_ki    ,  gyro_x_kd  ,  x_rate_pid_integ_limit);
			pid_init(&rate_pitch_pid, gyro_y_kp    , gyro_y_ki    ,  gyro_y_kd  ,  y_rate_pid_integ_limit);
			pid_init(&rate_yaw_pid  , gyro_z_kp    , gyro_z_ki    ,  gyro_z_kd  ,  z_rate_pid_integ_limit);
			//高度pid的初始化函数
			pid_init(&height_pid,     height_kp   ,  height_ki    ,height_kd    ,  pid_height_limit);
			control_flag_ok=0;
		}
    *pid_roll_output=pid_update(&rate_roll_pid,pid_update(&oula_roll_pid,desire_roll,Roll),gyrox);
    *pid_pitch_output=pid_update(&rate_pitch_pid,pid_update(&oula_pitch_pid,deisre_pitch,Pitch),gyroy);
		*pid_yaw_output=pid_update(&rate_yaw_pid,pid_update(&oula_yaw_pid,desire_yaw,Yaw),gyroz);

    //位置式pid的输出		
		//*pid_height_output=pid_update(&height_pid,desire_height,measure_height);//高度pid的输出
		//增量式pid输出
		*pid_height_output=height_pid_update(&height_pid,desire_height,measure_height);
		//roll输出量的限幅
		if(*pid_roll_output>200){
		  *pid_roll_output=200;
		}
		if(*pid_roll_output<-200){
		  *pid_roll_output=-200;
	  }
		//pitch输出量的限幅
		if(*pid_pitch_output>200){
		  *pid_pitch_output=200;
		}
		if(*pid_pitch_output<-200){
		  *pid_pitch_output=-200;
	  }
		//yaw输出量的限幅
		if(*pid_yaw_output>150){
		  *pid_yaw_output=150;
		}
		if(*pid_yaw_output<-150){
		  *pid_yaw_output=-150;
	  }
		//高度pid的输出量限幅
		if(*pid_height_output>300){
		    *pid_height_output=300;
		}
		if(*pid_height_output<-300){
		   *pid_height_output=-300;
		}
		
}
//最低和最高油门限制			
u16 limit_thrust(u16 motor){
	if(motor>THRUST_MAX) 
	{motor=THRUST_MAX;}
	if(motor<THRUST_MIN) 
		{motor=THRUST_MIN;}
	return motor;
}

//电机pwm值分配函数，接收PID的输出量
void set_motor(u32 base_thrust,u16* motor1,u16* motor2,u16* motor3,u16* motor4,
	               float pid_roll_out,float pid_pitch_out,float pid_yaw_out,float pid_height_out){
  *motor1=limit_thrust(base_thrust  +  thrust1_modify  + pid_roll_out +  pid_pitch_out  + pid_yaw_out  +  pid_height_out);
	*motor2=limit_thrust(base_thrust  +  thrust2_modify  + pid_roll_out -  pid_pitch_out -  pid_yaw_out  +  pid_height_out);
	*motor3=limit_thrust(base_thrust  +  thrust3_modify  -pid_roll_out  -  pid_pitch_out  + pid_yaw_out  +  pid_height_out);
	*motor4=limit_thrust(base_thrust  +  thrust4_modify  -pid_roll_out  +  pid_pitch_out  - pid_yaw_out  +  pid_height_out);
	set_compare(*motor1,*motor2,*motor3,*motor4);
}

