#ifndef __MY6050DATA_H
#define __MY6050DATA_H
void mpu6050_init(void);
void yaw_roll_pitch_gyro_data(float* Roll,float* Pitch,float* Yaw,short* Gyro_x,short* Gyro_y,short* Gyro_z);


#endif

