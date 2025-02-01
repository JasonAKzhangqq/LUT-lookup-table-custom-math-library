//#ifndef _PROGRAM_H_		//用于嵌入式编程
#define _PROGRAM_H_

// 常用的值，以及相关配置
#define M_PI       3.1415926535897932
#define M_PI_M2    6.2831853071795862        //pi*2
#define M_PI_D2    1.5707963267948965        //pi/2 
#define M_180_D_PI  57.29577951308232        //180/pi
#define M_PI_D180  0.017453292519943        //pi/180
#define RADIAN_MIN -M_PI_D2
#define RADIAN_MAX M_PI_D2
#define RADIAN_STEP 0.001
#define TABLE_SIZE ((int)( M_PI_D2 / RADIAN_STEP ) + 1 )//ֻ只记录0到pi/2的表

