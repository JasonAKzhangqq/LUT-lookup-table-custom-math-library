#include <stdio.h>
#include <math.h>
#include "program.h"


// 各个表
double sin_table[TABLE_SIZE];
double tan_table[TABLE_SIZE];
double arc_table[TABLE_SIZE];

// 表初始化
void init_tables() {
    for (int i = 0; i < (TABLE_SIZE - 1); i++) {
        double radian =((i * RADIAN_STEP));
        sin_table[i] = sin(radian);
        tan_table[i] = tan(radian);
        arc_table[i] = RADIAN_STEP*i;
    }
    sin_table[TABLE_SIZE - 1] = 1.0;
    tan_table[TABLE_SIZE - 1] = HUGE_VAL;
    arc_table[TABLE_SIZE - 1] = M_PI_D2;
}

//浮点型绝对值
double Abs_float(double a)
{
    if (a >= 0)
    {
        return a;
    }
    else
        return (a * -(1.0));
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
    if (a < 0) {
        return -1; // 返回错误值，表示输入无效 
    }
    //if (a == 0) {
    //    return 0;
    //}

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

// 自定义实现的 pow 函数，只考虑正数次幂
double my_pow(double base, int exponent) {
    double result = 1.0;
    // 通过循环将 base 乘以自身 exponent 次
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

// 二分法查表
int find_closest_index(double value, double* table) {
    int left = 0;
    int right = TABLE_SIZE - 1;
    int closest_index = 0;
    double closest_diff = fabs(value - table[0]);

    while (left <= right) {
        int mid = left + (right - left) / 2;
        double diff = fabs(value - table[mid]);

        if (diff < closest_diff) {
            closest_diff = diff;
            closest_index = mid;
        }

        if (value < table[mid]) {
            right = mid - 1;
        }
        else if (value > table[mid]) {
            left = mid + 1;
        }
        else {
            return mid; 
        }
    }

    return closest_index;
}

// 正三角函数查表
double sin_lookup(double radian) {
    //把输入值限制在-pi到pi
    while (radian < -M_PI)
    {
        radian += M_PI_M2;
    }
    while (radian > M_PI)
    {
        radian -= M_PI_M2;
    }

    //查表
    if (radian < 0)
    {
        if (radian < -M_PI_D2)
        {
            //-pi到-pi/2诱导公式
            return -sin_table[(int)(((M_PI +radian) / RADIAN_STEP))];
        }
        else
        {
            //-pi/2到0诱导公式
            return -sin_table[(int)(((-radian) / RADIAN_STEP))];
        }
    }
    else
    {
        if (radian < M_PI_D2)
        {
            //0到pi/2直接查表
            // return sin_table[(int)(((radian) / RADIAN_STEP) )];
            int index = (int)(((radian) / RADIAN_STEP));
            if (index < TABLE_SIZE - 1) {
                // 线性插值
                double fraction = (radian - index * RADIAN_STEP) / RADIAN_STEP;
                return sin_table[index] + fraction * (sin_table[index + 1] - sin_table[index]);
            } else {
                return sin_table[index];
            }
        }
        else
        {
            //pi/2到pi诱导公式
            return sin_table[(int)(((M_PI-radian) / RADIAN_STEP)+0)];
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

    // //查表
    // if (radian > 0)
    // {
    //     return tan_table[(int)((radian) / RADIAN_STEP)];
    // }
    // else
    // {
    //     return -tan_table[(int)((-radian) / RADIAN_STEP)];
    // }
        // 查表
    if (radian > 0) {
        int index = (int)((radian) / RADIAN_STEP);
        if (index < TABLE_SIZE - 1) {
            // 线性插值
            double fraction = (radian - index * RADIAN_STEP) / RADIAN_STEP;
            return tan_table[index] + fraction * (tan_table[index + 1] - tan_table[index]);
        } else {
            return tan_table[index];
        }
    } else {
        int index = (int)((-radian) / RADIAN_STEP);
        if (index < TABLE_SIZE - 1) {
            // 线性插值
            double fraction = (-radian - index * RADIAN_STEP) / RADIAN_STEP;
            return -tan_table[index] - fraction * (tan_table[index + 1] - tan_table[index]);
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
            double fraction = (value - sin_table[index]) / (sin_table[index + 1] - sin_table[index]);
            return arc_table[index] + fraction * (arc_table[index + 1] - arc_table[index]);
        } else {
            return arc_table[index];
        }
    } else {
        int index = find_closest_index(-value, sin_table);
        if (index < TABLE_SIZE - 1) {
            // 线性插值
            double fraction = (-value - sin_table[index]) / (sin_table[index + 1] - sin_table[index]);
            return -arc_table[index] - fraction * (arc_table[index + 1] - arc_table[index]);
        } else {
            return -arc_table[index];
        }
    }

    // if (value > 0)
    // {
    //     return arc_table[find_closest_index(value, sin_table)];
    // }
    // else
    // {
    //     return -arc_table[find_closest_index((-value), sin_table)];
    // }


    //if (value > 0)
    //{
    //    return (RADIAN_STEP*find_closest_index(value, sin_table));
    //}
    //else
    //{
    //    return ((-RADIAN_STEP) * find_closest_index((-value), sin_table));
    //}
}

// 利用 asin_lookup 函数计算 arccos
double arccos_lookup(double value) {
    return M_PI_D2 - arcsin_lookup(value);
}

// 查表法实现atan
double arctan_lookup(double value) {
    // if (value > 0)
    // {
    //     return arc_table[find_closest_index(value, tan_table)];
    // }
    // else
    // {
    //     return -arc_table[find_closest_index((-value), tan_table)];
    // }
        if (value > 0) {
        int index = find_closest_index(value, tan_table);
        if (index < TABLE_SIZE - 1) {
            // 线性插值
            double fraction = (value - tan_table[index]) / (tan_table[index + 1] - tan_table[index]);
            return arc_table[index] + fraction * (arc_table[index + 1] - arc_table[index]);
        } else {
            return arc_table[index];
        }
    } else {
        int index = find_closest_index(-value, tan_table);
        if (index < TABLE_SIZE - 1) {
            // 线性插值
            double fraction = (-value - tan_table[index]) / (tan_table[index + 1] - tan_table[index]);
            return -arc_table[index] - fraction * (arc_table[index + 1] - arc_table[index]);
        } else {
            return -arc_table[index];
        }
    }
}

// 查表法实现atan2
double atan2_lookup(double y, double x) {
    double value = ((y / x));
    double atan_value = 0;
    if (judge_float_equal(x,0)) {
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

    atan_value = arctan_lookup(value);

    if (x > 0) {
        return atan_value;
    }
    else if (y >= 0) {
        return atan_value + M_PI; 
    }
    else {
        return atan_value - M_PI; 
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

        printf("const double arc_table[TABLE_SIZE]=\n");
        printf("{\n");
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            printf("%.15f,\n", arc_table[i]);
        }
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

    //for (double i = -M_PI_M2; i <= M_PI_M2; i += 0.1)
    //{
    //    printf("%.15f\n", tan_lookup(i));
    //    printf("%.15f\n", tan(i));
    //}
    //for (int i = 0; i < TABLE_SIZE; i++)
    //{
    //    printf("%.15f,\n", arc_table[i]);
    //}
    //for (double i = -1; i <= 1; i += 0.1)
    //{
    //    printf("%.15f\n", arcsin_lookup(i));
    //    printf("%.15f\n", asin(i));
    //}

    for (double i = -3; i <= 3; i += 1)
        for (double j = -3; j <= 3; j += 1)
        {
            printf("%.15f\n", atan2_lookup(i,j));
            printf("%.15f\n", atan2(i,j));
        }
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
