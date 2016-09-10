#include "prepare.h"
extern float ROLL,PITCH,YAW;
extern short GYROX,GYROY,GYROZ;
extern float DESIRE_ROLL;
extern float DESIRE_PITCH;
extern float DESIRE_YAW;
extern float DESIRE_HEIGHT;
extern float distance;

extern u32 thrust_base;
extern int fly_time;


extern float height_kp;
extern float height_ki;
extern float height_kd;

u8 key_ok=1;
u8 fly_time_ok=1;
u8 fly_gesture_roll=1;
u8 fly_gesture_pitch=1;
u8 fly_gesture_yaw=1;
u8 fly_height_ok=1;

void set_key(void){
  		//按键设置的循环当按键3按下的时候按键设置结束
	while(key_ok){
	  if(read_key1()==0){
		  thrust_base+=1;
		}
		
		if(read_key2()==0){
		  thrust_base-=1;
		}
//		
		if(read_key3()==0){
		  key_ok=0;
			led1_on();
		}
	//	thrust_base=ADC_ValueGet(ADC0_BASE,3);
		LCD_Draw_String("current thrust:", 0,0,0);
		LCD_Draw_Num(thrust_base,2,2,2,0);	//显示的数据，列数，行数，翻转
	}
	//设置飞行时间
	LCD_ScreenClr();
	while(fly_time_ok){
		if(read_key1()==0){
		  fly_time+=200;
	  }
		
		if(read_key2()==0){
			fly_time-=200;
			if(fly_time<0){
				fly_time=0;
			}
		}
		
		if(read_key3()==0){
			led1_off();
			fly_time_ok=0;
			led2_on();	
		}
		LCD_Draw_String("fly time", 0,0,0);
		LCD_Draw_Num(fly_time,2,2,2,0);	//显示的数据，列数，行数，翻转
	}
	LCD_ScreenClr();
	//设置飞行姿态DESIRE_ROLL
	while(fly_gesture_roll){
		if(read_key1()==0){  //设置
		  DESIRE_ROLL+=0.2f;
	  }
		
		if(read_key2()==0){
      DESIRE_ROLL-=0.2f;
		}
		
		if(read_key3()==0){
			fly_gesture_roll=0;
			led2_off();
			led3_on();
		}
		LCD_Draw_String("DESIRE_ROLL", 0,0,0);
    if(DESIRE_ROLL<0){
		LCD_Draw_String("-",2,1,0);
		LCD_Draw_Num(-DESIRE_ROLL,2,2,2,0);	
		}else{		
		LCD_Draw_String(" ",2,1,0);
		LCD_Draw_Num(DESIRE_ROLL,2,2,2,0);	//显示的数据，列数，行数，翻转
		}
	}
	LCD_ScreenClr();
	//设置飞行的姿态DESIRE_PITCH
	while(fly_gesture_pitch){
		if(read_key1()==0){  //设置
		  DESIRE_PITCH+=0.2f;
	  }
		
		if(read_key2()==0){
      DESIRE_PITCH-=0.2f;
		}
		
		if(read_key3()==0){
			fly_gesture_pitch=0;
			led3_off();
			led1_on();
		}
		LCD_Draw_String("DESIRE_PITCH", 0,0,0);
		if(DESIRE_PITCH<0){
		LCD_Draw_String("-",2,1,0);
		LCD_Draw_Num(-DESIRE_PITCH,2,2,2,0);	
		}else{	
    LCD_Draw_String(" ",2,1,0);			
		LCD_Draw_Num(DESIRE_PITCH,2,2,2,0);	//显示的数据，列数，行数，翻转
		}
	}
	
	LCD_ScreenClr();		
	///设置飞行DEIRE_YAW/////////////////
	while(fly_gesture_yaw){
		if(read_key1()==0){  //设置
		  DESIRE_YAW+=0.2f;
	  }
		
		if(read_key2()==0){
      DESIRE_YAW-=0.2f;
		}
		
		if(read_key3()==0){
			fly_gesture_yaw=0;
			led1_off();
			led2_on();
		}
		LCD_Draw_String("DESIRE_YAW", 0,0,0);
		if(DESIRE_YAW<0){
		LCD_Draw_String("-",2,1,0);
		LCD_Draw_Num(-DESIRE_YAW,2,2,2,0);	
		}else{		
		LCD_Draw_String(" ",2,1,0);
		LCD_Draw_Num(DESIRE_YAW,2,2,2,0);	//显示的数据，列数，行数，翻转
		}
	}	
	LCD_ScreenClr();
	////////设置飞行高度///////////////////////////
	while(fly_height_ok){
	  if(read_key1()==0){  //设置
      DESIRE_HEIGHT+=1.0f;
	  }
		
		if(read_key2()==0){
      DESIRE_HEIGHT-=1.0f;
		}
		
		if(read_key3()==0){
      fly_height_ok=0;
			led2_off();
			led3_on();
		}
    LCD_Draw_String("DESIRE_HEIGHT ",0,0,0);			
		if(DESIRE_HEIGHT<0){
		LCD_Draw_String("-",2,1,0);
		LCD_Draw_Num(-DESIRE_HEIGHT,2,2,2,0);	
		}else{	
		LCD_Draw_String(" ",2,1,0);
		LCD_Draw_Num(DESIRE_HEIGHT,2,2,2,0);	//显示的数据，列数，行数，翻转
		}
		
	}
	
	///////////////////////////////////////////////////////////
	led1_on();//按键设置成功
	led2_on();
	led3_on();
}

void lcd_screen_init(void){
//显示屏的初始化
  LCD_Enable();//显示屏相关引脚的使能以及初始化						
	LCD_Init();	//显示屏相关引脚的使能以及初始化							
  LCD_ScreenClr();//显示屏相关引脚的使能以及初始化
	LCD_Draw_String("waiting.......", 0,0,0);
}


//得到初始的期望值姿态的函数
void get_desire_value(void){
	u8 i=0;
  double temp1=0;
	double temp2=0;
	double temp3=0;
	led1_on();
	send_signal();
	delay_ms(500);
	led1_off();
	delay_ms(500);
	led2_on();
	send_signal();
	delay_ms(500);
	led2_off();
	delay_ms(500);
	led3_on();
	send_signal();
	delay_ms(500);
	led3_off();
	delay_ms(500);
	for(i=0;i<50;i++){//空读50次数据
		yaw_roll_pitch_gyro_data(&ROLL,&PITCH,&YAW,&GYROX,&GYROY,&GYROZ);
	}
 
	
	for(i=0;i<10;i++){
		yaw_roll_pitch_gyro_data(&ROLL,&PITCH,&YAW,&GYROX,&GYROY,&GYROZ);
		temp1+=ROLL;
		temp2+=PITCH;
		temp3+=YAW;
	}
	 DESIRE_ROLL=(float)(temp1/10);
	 DESIRE_PITCH=(float)(temp2/10);
	 DESIRE_YAW=(float)(temp3/10);
	 DESIRE_HEIGHT=distance;
//	  yaw_roll_pitch_gyro_data(&DESIRE_ROLL,&DESIRE_PITCH,&DESIRE_YAW,&GYROX,&GYROY,&GYROZ);
	  
}


void wait_fly_time_over(void){
  		if(fly_time<0){  //到达飞行时间
				if(distance>10){ 
				  DESIRE_HEIGHT-=10;
					if(DESIRE_HEIGHT<=10){
					  DESIRE_HEIGHT=0;
					}
				}
				height_kp=25;
				if(distance<=20){
					while(1){
				  set_compare(1000,1000,1000,1000);
					}
				}
	  }	
}


