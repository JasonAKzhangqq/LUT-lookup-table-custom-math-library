//#ifndef _PROGRAM_H_		/*用于嵌入式编程*/
#define _PROGRAM_H_

// 常用的值，以及相关配置
#define M_PI       3.1415926535897932
#define M_PI_M2    6.2831853071795862        //pi*2
#define M_PI_D2    1.5707963267948965        //pi/2 
#define M_180_D_PI  57.29577951308232        //180/pi
#define M_PI_D180  0.017453292519943        //pi/180
#define RADIAN_MIN -M_PI_D2
#define RADIAN_MAX M_PI_D2
#define RADIAN_STEP 0.0001
#define TABLE_SIZE ((int)( M_PI_D2 / RADIAN_STEP ) + 1 )//ֻ只记录0到pi/2的表

//#endif	/*用于嵌入式编程*/

/*
说明：
本实现方法绝非最优，但适用于本人的项目
绝对值考虑浮点型数据用整型转换会丢失小数
开方考虑到实际有时数据会很大，故暂不考虑用查表法，直接用牛顿逼近法
次方考虑到实际不会有负次幂，故没有做负次幂的功能
二分法查表是用于反三角函数查表使用
所有表都只有0到pi/2，通过对称性和诱导公式就可以求其它定义域的值
cos用诱导公式转为sin，可以节省cos_table的空间，asin和acos同理
tan的表和sin的表不同，虽然能用诱导公式转换为sin和cos表示，但atan需要tan表，故单独开一个tan表
arc的表都相同，由步长决定，只是在查表时查找了对应的正三角函数表而已

？反三角函数表可以不要，直接用步长代替（见asin内的注释代码
*/

