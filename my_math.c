#include <stdio.h>
#include "my_math.h"
#include "my_math_table.h"

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

//pow只考虑正整数次幂
double pow_custom(double base, int exponent) 
{
    double result = 1.0;
    // 通过循环将 base 乘以自身 exponent 次
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

//牛顿逼近法开方
double sqrt_newton(double a) 
{
    if (a < 0)return -1; // 返回错误值，表示输入无效 
    double x0 = a / 3.0; // 初始猜测值，当求的值大，除的数越大，迭代次数越少，但同时对小数值的迭代次数会增加，自行设定
    double x1;
    double error = 1e-4; // 设定误差阈值 
    int max_iterations = 1000; // 最大迭代次数，防止死循环 

    for (int i = 0; i < max_iterations; ++i) {
        x1 = (x0 + a / x0) / 2.0; // 牛顿迭代公式 
        if (Abs_float(x1 - x0) < error) {
            printf("iterations time=%d\n", i);
            break;
        }
        x0 = x1;
    }

    return x1;
}

// 二分法，查找输入值和表相近值，返回相近值在表中的位置
int find_closest_index(double value, const double* table) 
{
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

double sin_lookup(double radian) 
{
    // 把输入值限制在 -pi 到 pi
    while (radian < -M_PI) {
        radian += M_2_PI;
    }
    while (radian > M_PI) {
        radian -= M_2_PI;
    }

    // 查表
    if (radian < 0) {
        if (radian < -M_PI_2) {
            // -pi 到 -pi/2 诱导公式
            int index = (int)(((M_PI + radian) / RADIAN_STEP));
            if (index < TABLE_SIZE - 1) {
                return -sin_table[index] - ((M_PI + radian - index * RADIAN_STEP) / RADIAN_STEP) * (sin_table[index + 1] - sin_table[index]);
            } else {
                return -sin_table[index];
            }
        } else {
            // -pi/2 到 0 诱导公式
            int index = (int)(((-radian) / RADIAN_STEP));
            if (index < TABLE_SIZE - 1) {
                return -sin_table[index] - ((-radian - index * RADIAN_STEP) / RADIAN_STEP) * (sin_table[index + 1] - sin_table[index]);
            } else {
                return -sin_table[index];
            }
        }
    } else {
        if (radian < M_PI_2) {
            // 0 到 pi/2 直接查表
            int index = (int)(((radian) / RADIAN_STEP));
            if (index < TABLE_SIZE - 1) {
                return sin_table[index] + ((radian - index * RADIAN_STEP) / RADIAN_STEP) * (sin_table[index + 1] - sin_table[index]);
            } else {
                return sin_table[index];
            }
        } else {
            // pi/2 到 pi 诱导公式
            int index = (int)(((M_PI - radian) / RADIAN_STEP));
            if (index < TABLE_SIZE - 1) {
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
    return sin_lookup(M_PI_2 - radian);
}

double tan_lookup(double radian)
{
    //把输入值限制在-pi/2到pi/2
    while (radian < -M_PI_2)
    {
        radian += M_PI;
    }
    while (radian > M_PI_2)
    {
        radian -= M_PI;
    }
    // 查表
    if (radian > 0) {
        int index = (int)((radian) / RADIAN_STEP);
        if (index < TABLE_SIZE - 1) {
            return tan_table[index] + ((radian - index * RADIAN_STEP) / RADIAN_STEP) * (tan_table[index + 1] - tan_table[index]);
        } else {
            return tan_table[index];
        }
    } else {
        int index = (int)((-radian) / RADIAN_STEP);
        if (index < TABLE_SIZE - 1) {
            return -tan_table[index] - ((-radian - index * RADIAN_STEP) / RADIAN_STEP) * (tan_table[index + 1] - tan_table[index]);
        } else {
            return -tan_table[index];
        }
    }
}

double asin_lookup(double value) 
{
    if (value < -1.0 || value > 1.0) {
        // 输入值超出范围，返回错误值，原本是NAN，这里取0
        return 0;
    }
    if (value > 0) {
        int index = find_closest_index(value, sin_table);
        if (index < TABLE_SIZE - 1) {
            return RADIAN_STEP*index + ((value - sin_table[index]) / (sin_table[index + 1] - sin_table[index])) * RADIAN_STEP;
        } else {
            return M_PI_2;
        }
    } else {
        int index = find_closest_index(-value, sin_table);
        if (index < TABLE_SIZE - 1) {
            return -RADIAN_STEP*index - ((-value - sin_table[index]) / (sin_table[index + 1] - sin_table[index])) * RADIAN_STEP;
        } else {
            return -M_PI_2;
        }
    }
}

double acos_lookup(double value) 
{
    //等效公式
    return (M_PI_2 - asin_lookup(value));
}

double atan_lookup(double value) {
        if (value > 0) {
        int index = find_closest_index(value, tan_table);
        if (index < TABLE_SIZE - 1) {
            return RADIAN_STEP * index + ((value - tan_table[index]) / (tan_table[index + 1] - tan_table[index])) * RADIAN_STEP;
        }
        else {
            return M_PI_2;
        }
    } else {
        int index = find_closest_index(-value, tan_table);
        if (index < TABLE_SIZE - 1) {
            return -RADIAN_STEP * index - ((-value - tan_table[index]) / (tan_table[index + 1] - tan_table[index])) * RADIAN_STEP;
        }
        else {
            return -M_PI_2;
        }
    }
}

double atan2_lookup(double y, double x) 
{
    if (judge_float_equal(x,0)) {
        if (y > 0) {
            return M_PI_2;
        }
        else if (y < 0) {
            return -M_PI_2;
        }
        else {
            return 0;
        }
    }
    if (x > 0) {
        return atan_lookup(y / x);
    }
    else if (y >= 0) {
        return atan_lookup(y / x) + M_PI;
    }
    else {
        return atan_lookup(y / x) - M_PI;
    }
}

int main() {


    return 0;
}
