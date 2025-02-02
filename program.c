#include <stdio.h>
#include <math.h>
#include "program.h"

// 各个表
double sin_table[TABLE_SIZE];
double tan_table[TABLE_SIZE];

// 表初始化
void init_tables() {
    for (int i = 0; i < (TABLE_SIZE - 1); i++) {
        double radian =(i * RADIAN_STEP);
        sin_table[i] = sin(radian);
        tan_table[i] = tan(radian);
    }
    sin_table[TABLE_SIZE - 1] = 1.0;
    tan_table[TABLE_SIZE - 1] = HUGE_VAL;
}

//浮点型绝对值
double Abs_float(double a)
{
    if (a >= 0)return a;
    else return (a * -(1.0));
}

//比较浮点数是否相等
_Bool judge_float_equal(double value_1, double value_2)
{
    //阈值可以自行设置，这里认为两值相差小于1e-4即相等
    if (Abs_float(value_1 - value_2) < 1.0e-4)return 1;
    return 0;
}

//牛顿逼近法开方
double sqrt_newton(double a) {
    if (a < 0)return -1; // 返回错误值，表示输入无效 
    double x0 = a / 2.0; // 初始猜测值 
    double x1;
    double error = 1e-2; // 设定误差阈值 
    int max_iterations = 1000; // 最大迭代次数，防止死循环 

    for (int i = 0; i < max_iterations; ++i) {
        x1 = (x0 + a / x0) / 2.0; // 牛顿迭代公式 
        if (Abs_float(x1 - x0) < error) {
            break;
        }
        x0 = x1;
    }

    return x1;
}

//// 查表法实现sqrt
//double sqrt_lookup(double a) {
//    if (a < 0) return -1; // 返回错误值，表示输入无效
//
//    int index = find_closest_index(a, sqrt_table);
//    if (index < TABLE_SIZE - 1) {
//        // 线性插值
//        return sqrt_table[index] + ((a - index * RADIAN_STEP) / RADIAN_STEP) * (sqrt_table[index + 1] - sqrt_table[index]);
//    } else {
//        return sqrt_table[index];
//    }
//}

// 自定义实现的 pow 函数，只考虑正数次幂
double my_pow(double base, int exponent) {
    double result = 1.0;
    // 通过循环将 base 乘以自身 exponent 次
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

// 二分法，查找输入值和表相近值，返回相近值在表中的位置
int find_closest_index(double value, double* table) {
    int left = 0;
    int right = TABLE_SIZE - 1;
    int mid;
    do{
        mid = left + (right - left) / 2;
        if (value < table[mid])right = mid - 1;
        else if (value > table[mid]) left = mid + 1;
        else return mid;//防止输入值刚好和表值相等
    } while (left <= right);
    return mid;
}

double sin_lookup(double radian) {
    // 把输入值限制在 -pi 到 pi
    while (radian < -M_PI) {
        radian += M_PI_M2;
    }
    while (radian > M_PI) {
        radian -= M_PI_M2;
    }

    // 查表
    if (radian < 0) {
        if (radian < -M_PI_D2) {
            // -pi 到 -pi/2 诱导公式
            int index = (int)(((M_PI + radian) / RADIAN_STEP));
            if (index < TABLE_SIZE - 1) {
                // 线性插值
                return -sin_table[index] - ((M_PI + radian - index * RADIAN_STEP) / RADIAN_STEP) * (sin_table[index + 1] - sin_table[index]);
            } else {
                return -sin_table[index];
            }
        } else {
            // -pi/2 到 0 诱导公式
            int index = (int)(((-radian) / RADIAN_STEP));
            if (index < TABLE_SIZE - 1) {
                // 线性插值
                return -sin_table[index] - ((-radian - index * RADIAN_STEP) / RADIAN_STEP) * (sin_table[index + 1] - sin_table[index]);
            } else {
                return -sin_table[index];
            }
        }
    } else {
        if (radian < M_PI_D2) {
            // 0 到 pi/2 直接查表
            int index = (int)(((radian) / RADIAN_STEP));
            if (index < TABLE_SIZE - 1) {
                // 线性插值
                return sin_table[index] + ((radian - index * RADIAN_STEP) / RADIAN_STEP) * (sin_table[index + 1] - sin_table[index]);
            } else {
                return sin_table[index];
            }
        } else {
            // pi/2 到 pi 诱导公式
            int index = (int)(((M_PI - radian) / RADIAN_STEP));
            if (index < TABLE_SIZE - 1) {
                // 线性插值
                return sin_table[index] + ((M_PI - radian - index * RADIAN_STEP) / RADIAN_STEP) * (sin_table[index + 1] - sin_table[index]);
            } else {
                return sin_table[index];
            }
        }
    }
}

double cos_lookup(double radian)
{
    //诱导公式
    return sin_lookup(M_PI_D2 - radian);
}

double tan_lookup(double radian)
{
    //把输入值限制在-pi/2到pi/2
    while (radian < -M_PI_D2)
    {
        radian += M_PI;
    }
    while (radian > M_PI_D2)
    {
        radian -= M_PI;
    }
    // 查表
    if (radian > 0) {
        int index = (int)((radian) / RADIAN_STEP);
        if (index < TABLE_SIZE - 1) {
            // 线性插值
            return tan_table[index] + ((radian - index * RADIAN_STEP) / RADIAN_STEP) * (tan_table[index + 1] - tan_table[index]);
        } else {
            return tan_table[index];
        }
    } else {
        int index = (int)((-radian) / RADIAN_STEP);
        if (index < TABLE_SIZE - 1) {
            // 线性插值
            return -tan_table[index] - ((-radian - index * RADIAN_STEP) / RADIAN_STEP) * (tan_table[index + 1] - tan_table[index]);
        } else {
            return -tan_table[index];
        }
    }
}

// 查表法实现arcsin
double arcsin_lookup(double value) {
    if (value < -1.0 || value > 1.0) {
        // 输入值超出范围，返回错误值
        return NAN;
    }

    if (value > 0) {
        int index = find_closest_index(value, sin_table);
        if (index < TABLE_SIZE - 1) {
            // 线性插值
            return RADIAN_STEP*index + ((value - sin_table[index]) / (sin_table[index + 1] - sin_table[index])) * RADIAN_STEP;
        } else {
            return M_PI_D2;//最大值再插值数组就跨界访问了
        }
    } else {
        int index = find_closest_index(-value, sin_table);
        if (index < TABLE_SIZE - 1) {
            // 线性插值
            return -RADIAN_STEP*index - ((-value - sin_table[index]) / (sin_table[index + 1] - sin_table[index])) * RADIAN_STEP;
        } else {
            return -M_PI_D2;//最大值再插值数组就跨界访问了
        }
    }
}

// 利用 asin_lookup 函数计算 arccos
double arccos_lookup(double value) {
    //等效公式
    return (M_PI_D2 - arcsin_lookup(value));
}

// 查表法实现atan
double arctan_lookup(double value) {
        if (value > 0) {
        int index = find_closest_index(value, tan_table);
        if (index < TABLE_SIZE - 1) {
            // 线性插值
            return RADIAN_STEP * index + ((value - tan_table[index]) / (tan_table[index + 1] - tan_table[index])) * RADIAN_STEP;
        }
        else {
            return M_PI_D2;//最大值再插值数组就跨界访问了
        }
    } else {
        int index = find_closest_index(-value, tan_table);
        if (index < TABLE_SIZE - 1) {
            // 线性插值
            return -RADIAN_STEP * index - ((-value - tan_table[index]) / (tan_table[index + 1] - tan_table[index])) * RADIAN_STEP;
        }
        else {
            return -M_PI_D2;//最大值再插值数组就跨界访问了
        }
    }
}

// 查表法实现atan2
double atan2_lookup(double y, double x) {
    if (judge_float_equal(x,0.0)) {
        if (y > 0) {
            return M_PI_D2;
        }
        else if (y < 0) {
            return -M_PI_D2;
        }
        else {
            return 0;
        }
    }
    if (x > 0) {
        return arctan_lookup(y / x);
    }
    else if (y >= 0) {
        return arctan_lookup(y / x) + M_PI;
    }
    else {
        return arctan_lookup(y / x) - M_PI;
    }


}

//打印表
void print_table(void)
{
    
        printf("const double sin_table[TABLE_SIZE]=\n");
        printf("{\n");
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            printf("%.15f,\n", sin_table[i]);
        }
        printf("};\n");

        printf("const double tan_table[TABLE_SIZE]=\n");
        printf("{\n");
        for (int i = 0; i < (TABLE_SIZE - 1); i++)
        {
            printf("%.15f,\n", tan_table[i]);
        }
        printf("%.1f,\n",1e15);//最后一个值是无限大
        printf("};\n");    
}

int main() {
    // 初始表
    init_tables();

    //print_table();
    // 测试
    //for (int i = 0; i < TABLE_SIZE; i++)
    //{
    //    printf("%.15f,\n", arcsin_table[i]);
    //    printf("%.15f,\n", arc_table[i]);
    //}
    //printf("%.15f,\n", atan(0.815298612529843));

    //for (double i = 0; i <= M_PI_D2; i+= RADIAN_STEP)
    //{
    //    //printf("%.5f\n", ((i ) * M_180_D_PI));
    //    printf("%.15f\n", tan_lookup(i));
    //    //printf("%.15f\n", tan(i));
    //}
    //    for (double i = -M_PI_M2; i <= M_PI_M2; i += 0.1)
    //{
    //    printf("%.3f\n", (i* M_180_D_PI));
    //    printf("%.15f\n", cos_lookup(i));
    //    printf("%.15f\n", cos(i));
    //}
        //printf("%.15f\n", sin_lookup(M_PI_D2));
        //printf("%.15f\n", sin(M_PI_D2));
        //printf("%.15f\n", sin_lookup(-M_PI_D2));
        //printf("%.15f\n", sin(-M_PI_D2));

    //for (double i = -M_PI_M2; i <= M_PI_M2; i += 0.1)
    //{
    //    printf("%.15f\n", tan_lookup(i));
    //    printf("%.15f\n", tan(i));
    //}
    //printf("%.15f\n", tan_lookup(0));
    //printf("%.15f\n", tan(0));
    //printf("%.15f\n", tan_lookup(M_PI_D2));
    //printf("%.15f\n", tan(M_PI_D2));
    //printf("%.15f\n", tan_lookup(-M_PI_D2));
    //printf("%.15f\n", tan(-M_PI_D2));

    //for (int i = 0; i < TABLE_SIZE; i++)
    //{
    //    printf("%.15f,\n", arc_table[i]);
    //}
    //for (double i = -1; i <= 1; i += 0.1)
    //{
    //    printf("%.15f\n", arcsin_lookup(i));
    //    printf("%.15f\n", asin(i));
    //}
    //atan2_lookup(0.0000, -3);
    //for (double i = -3; i <= 3; i += 1)
    //    for (double j = -3; j <= 3; j += 1)
    //    {
    //        printf("i=%f j=%f\n", i, j);
    //        printf("%.15f\n", atan2_lookup(i,j));
    //        printf("%.15f\n", atan2(i,j));
    //    }
    //printf("%.15f\n", atan2_lookup(100, 1));
    //printf("%.15f\n", atan2(100, 1));
    //printf("%.15f\n", atan2_lookup(100, -1));
    //printf("%.15f\n", atan2(100, -1));
    //printf("%.15f\n", atan2_lookup(-100, 1));
    //printf("%.15f\n", atan2(-100, 1));
    //printf("%.15f\n", atan2_lookup(-100, -1));
    //printf("%.15f\n", atan2(-100, -1));
    //printf("%.15f\n", atan2_lookup(1, 100));
    //printf("%.15f\n", atan2(1, 100));
    //printf("%.15f\n", atan2_lookup(1, -100));
    //printf("%.15f\n", atan2(1, -100));
    //printf("%.15f\n", atan2_lookup(-1, 100));
    //printf("%.15f\n", atan2(-1, 100));
    //printf("%.15f\n", atan2_lookup(-1, -100));
    //printf("%.15f\n", atan2(-1, -100));

    //printf("%.15f\n", sin_lookup(((M_PI / 6) + 0)));
    //printf("%.15f\n", sin_lookup(((M_PI / 4) + 0)));
    //printf("%.15f\n", sin_lookup(((M_PI / 3) + 0)));
    //printf("%.15f\n", sin_lookup(((M_PI / 6) + M_PI_D2)));
    //printf("%.15f\n", sin_lookup(((M_PI / 4) + M_PI_D2)));
    //printf("%.15f\n", sin_lookup(((M_PI / 3) + M_PI_D2)));
    //printf("%.15f\n", sin((M_PI / 6) + M_PI_D2));
    //printf("%.15f\n", sin((M_PI / 4) + M_PI_D2));
    //printf("%.15f\n", sin((M_PI / 3) + M_PI_D2));

    //printf("TABLE_SIZE=%d\n", TABLE_SIZE);
    //for (int i = 0; i < 10; i++)
    //{
    //    printf("%.3f\n", my_pow(i*1.7,3));
    //    printf("%.3f\n", pow(i * 1.7, 3));
    //}

    

    return 0;
}
