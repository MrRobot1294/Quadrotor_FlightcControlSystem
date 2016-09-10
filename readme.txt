关于四轴硬件引脚的使用情况：
四路PWM的输出：
//PWM0 PB6
//PWM1 PB7
//PWM2 PB4
//PWM3 PB5

MPU6050的引脚暂定于：
vcc  vcc
gnd  gnd
scl  PB2
sda  PB3
int  PB0


HMC5883L的引脚设置：
vcc    VCC
gnd    GND
scl    PA6
sda    PA7
DRDY   PA4


LCD占用的口：
PD0|-->SCK  //串行时钟
PD3|-->SDA	//串行输出
PB1|-->CS   //低电平片选
PC6|-->CD   //指令数据寄存器
PE5|-->RST	//低电平复位

PC7|<--Button1 
PD6|<--Button2
PD7|<--Button3
PE0|<--ADC    //滚轮ADC采样

超声波
vcc   vcc
trig  pc4
echo  pc5
gnd   gnd


超声波：
vcc     vcc
trig    PC4
echo    pc5
gnd     GND


继电器：
vcc  vcc
gnd  vcc
pin  PD1








系统时钟设置在50MHZ

代码编写记录：
6.30......程序的基本框架已经完成，未知电调的驱动方式