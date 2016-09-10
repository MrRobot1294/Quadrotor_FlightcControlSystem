#include "includes.h"
#include "fliter.h"
//外部全局姿态变量的数据声明
extern float ROLL,PITCH,YAW;
extern short GYROX,GYROY,GYROZ;

#define  FLITER_LEVER   15

//声明滑动滤波的数据
float ROLL_array[FLITER_LEVER]={0};;
float PITCH_array[FLITER_LEVER]={0};
float YAW_array[FLITER_LEVER]={0};

//short GYROX_array[FLITER_LEVER]={0};
//short GYROY_array[FLITER_LEVER]={0};
//short GYROZ_array[FLITER_LEVER]={0};

u8 rollquanvalue[FLITER_LEVER]={2,4,6,8,10,12,14,16,18,20,22,24,26,28,30};//权重放大了240
u8 pitchquanvalue[FLITER_LEVER]={2,4,6,8,10,12,14,16,18,20,22,24,26,28,30};//权重放大了240倍
u8 yawquanvalue[FLITER_LEVER]={2,4,6,8,10,12,14,16,18,20,22,24,26,28,30};//权重放大了240倍

//填满滑动滤波数据
void fill_array(void){
  u8 i;
	for(i=0;i<FLITER_LEVER;i++){
		yaw_roll_pitch_gyro_data(&ROLL,&PITCH,&YAW,&GYROX,&GYROY,&GYROZ);
		ROLL_array[i]=ROLL;
		PITCH_array[i]=PITCH;
		YAW_array[i]=YAW;
//		GYROX_array[i]=GYROX;
//		GYROY_array[i]=GYROY;
//		GYROZ_array[i]=GYROZ;
		delay_ms(200);
	}
}

//滑动滤波代码,
//带入参数是全局的姿态变量
//返回的参数也是全局的姿态变量.通过全局变量进行参数的返回
void moving_fliter(float oularoll,float oulapitch,float oulayaw,short gyrox,short gyroy,short gyroz){
	u8 i;
	u8 m;
	static u8 j=0;
	float ROLL_SUM=0,PITCH_SUM=0,YAW_SUM=0;
	
	float ROLL_max=0,ROLL_min=0;
	float PITCH_max=0,PITCH_min=0;
	float YAW_max=0,YAW_min=0;
	
//	short GYROX_max=0,GYROX_min=0;
//	short GYROY_max=0,GYROY_min=0;
//	short GYROZ_max=0,GYROZ_min=0;
	
  if(j==FLITER_LEVER){
	  j=0;
	}	
	ROLL_array[j]=oularoll;
	PITCH_array[j]=oulapitch;
	YAW_array[j]=oulayaw;
	
//	GYROX_array[j]=gyrox;
//	GYROY_array[j]=gyroy;
//	GYROZ_array[j]=gyroz;
	j++;
	
	ROLL_max=ROLL_min=ROLL_array[0];  //求取roll的最大最小值
	PITCH_max=PITCH_min=PITCH_array[0];
	YAW_max=YAW_min=YAW_array[0];
//	GYROX_max=GYROX_min=GYROX_array[0];
//	GYROY_max=GYROY_min=GYROY_array[0];
//	GYROZ_max=GYROZ_min=GYROX_array[0];
	
  for(i=1;i<FLITER_LEVER;i++){
		//寻找roll的最大最小值
	  if(ROLL_array[i]>ROLL_max){
		  ROLL_max=ROLL_array[i];
		}
		if(ROLL_array[i]<ROLL_min){
			ROLL_min=ROLL_array[i];
		}
		//寻找pitch的最大最小值
		if(PITCH_array[i]>PITCH_max){
		  PITCH_max=PITCH_array[i];
		}
		if(PITCH_array[i]<PITCH_min){
			PITCH_min=PITCH_array[i];
		}
		//寻找YAW的最大最小值
		if(YAW_array[i]>YAW_max){
		  YAW_max=YAW_array[i];
		}
		if(YAW_array[i]<YAW_min){
			YAW_min=YAW_array[i];
		}
	}
			for(m=0;m<FLITER_LEVER;m++){
		  ROLL_SUM+=ROLL_array[m];
			PITCH_SUM+=PITCH_array[m];
		  YAW_SUM+=YAW_array[m];
		}
		ROLL=(ROLL_SUM-ROLL_max-ROLL_min)/(FLITER_LEVER-2);
		PITCH=(PITCH_SUM-PITCH_max-PITCH_min)/(FLITER_LEVER-2);
		YAW=(YAW_SUM-YAW_max-YAW_min)/(FLITER_LEVER-2);

}



//滑动加权滤波

void moving_height_fliter(float oularoll,float oulapitch,float oulayaw,short gyrox,short gyroy,short gyroz){
	u8 i,n;
	static u8 j=0;
//	float ROLL_SUM=0,PITCH_SUM=0,YAW_SUM=0;
	float ROLL_max=0,ROLL_min=0;
	float PITCH_max=0,PITCH_min=0;
	float YAW_max=0,YAW_min=0;
	//最大与最小数据的下标
	u8 ROLLMAX_pin,ROLLMIN_pin;
	u8 PITCHMAX_pin,PITCHMIN_pin;
	u8 YAWMAX_pin,YAWMIN_pin;
	
  if(j==FLITER_LEVER){
	  j=0;
	}	
	ROLL_array[j]=oularoll;
	PITCH_array[j]=oulapitch;
	YAW_array[j]=oulayaw;
	j++;
	
	ROLL_max=ROLL_min=ROLL_array[0];  //求取roll的最大最小值
	PITCH_max=PITCH_min=PITCH_array[0];
	YAW_max=YAW_min=YAW_array[0];
	
  for(i=1;i<FLITER_LEVER;i++){
		//寻找roll的最大最小值
	  if(ROLL_array[i]>ROLL_max){
		  ROLL_max=ROLL_array[i];
			ROLLMAX_pin=i;
		}
		if(ROLL_array[i]<ROLL_min){
			ROLL_min=ROLL_array[i];
			ROLLMIN_pin=i;
		}
		//寻找pitch的最大最小值
		if(PITCH_array[i]>PITCH_max){
		  PITCH_max=PITCH_array[i];
			PITCHMAX_pin=i;
		}
		if(PITCH_array[i]<PITCH_min){
			PITCH_min=PITCH_array[i];
			PITCHMIN_pin=i;
		}
		//寻找YAW的最大最小值
		if(YAW_array[i]>YAW_max){
		  YAW_max=YAW_array[i];
			YAWMAX_pin=i;
		}
		if(YAW_array[i]<YAW_min){
			YAW_min=YAW_array[i];
			YAWMIN_pin=i;
		}
	}
	//分配权值，分配规则参考函数
	quanvalue_allocate(ROLLMAX_pin,ROLLMIN_pin,rollquanvalue);
	quanvalue_allocate(PITCHMAX_pin,PITCHMIN_pin,pitchquanvalue);
	quanvalue_allocate(YAWMAX_pin,YAWMIN_pin,yawquanvalue);
	
	for(n=0;n<15;n++){
	  ROLL+=rollquanvalue[i]*ROLL_array[i]/320;
		PITCH+=pitchquanvalue[i]*PITCH_array[i]/320;
		YAW+=yawquanvalue[i]*YAW_array[i]/320;
	}
}


void quanvalue_allocate(u8 max_pin,u8 min_pin,u8* quanvalue_array){
	//最大权值的分配函数////////////////////////////////////////////////////////
  if(max_pin==14){
	  quanvalue_array[13]=(u8)(quanvalue_array[max_pin]/2);
		quanvalue_array[0]= (u8)(quanvalue_array[max_pin]/2);
		quanvalue_array[max_pin]=0;
	}
	if(max_pin==0){
	  quanvalue_array[14]=(u8)(quanvalue_array[max_pin]/2);
		quanvalue_array[1]= (u8)(quanvalue_array[max_pin]/2);
		quanvalue_array[max_pin]=0;
	}
	quanvalue_array[14]=(u8)(quanvalue_array[max_pin]/2);
	quanvalue_array[0]= (u8)(quanvalue_array[max_pin]/2);
	quanvalue_array[max_pin]=0;
	
	//最小权值分配///////////////////////////////////////////////////////////////
	if(min_pin==14){
	  quanvalue_array[13]=(u8)(quanvalue_array[min_pin]/2);
		quanvalue_array[0]= (u8)(quanvalue_array[min_pin]/2);
		quanvalue_array[min_pin]=0;
	}
	if(min_pin==0){
	  quanvalue_array[14]=(u8)(quanvalue_array[min_pin]/2);
		quanvalue_array[1]= (u8)(quanvalue_array[min_pin]/2);
		quanvalue_array[min_pin]=0;
	}
	quanvalue_array[14]=(u8)(quanvalue_array[min_pin]/2);
	quanvalue_array[0]= (u8)(quanvalue_array[min_pin]/2);
	quanvalue_array[min_pin]=0;
	
}

	


