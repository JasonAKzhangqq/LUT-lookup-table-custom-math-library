#include <stdio.h>
#include <math.h>

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

// 各个表
double sin_table[TABLE_SIZE];
//double cos_table[TABLE_SIZE];
//double tan_table[TABLE_SIZE];
//double arcsin_table[TABLE_SIZE];
//double arccos_table[TABLE_SIZE];
double arctan_table[TABLE_SIZE];

// 表初始化
void init_tables() {
    for (int i = 0; i < (TABLE_SIZE - 1); i++) {
        double radian =((i * RADIAN_STEP));
        sin_table[i] = sin(radian);
        arctan_table[i] = atan(tan(radian));
    }
    sin_table[TABLE_SIZE - 1] = 1.00;
    arctan_table[TABLE_SIZE - 1] = M_PI_D2;
}
//int find_closest_index(double value, double* table) {
//    int closest_index = 0;
//    double closest_diff = fabs(value - table[0]);
//    for (int i = 1; i < TABLE_SIZE; i++) {
//        double diff = fabs(value - table[i]);
//        if (diff < closest_diff) {
//            closest_diff = diff;
//            closest_index = i;
//        }
//    }
//    return closest_index;
//}
// 
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


// sin查表
double sin_lookup(double radian) {
    //把表限制在-pi到pi
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
            return sin_table[(int)(((radian) / RADIAN_STEP) )];
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
    //转换公式
    return (sin_lookup(radian) / cos_lookup(radian));
}

// ʹ�ò��ұ����� arcsin
//double arcsin_lookup(double value) {
//    if (value < -1.0 || value > 1.0) {
//        printf("Error: Value out of range.\n");
//        return 0.0;
//    }
//    int index = find_closest_index(value, sin_table);
//    return arcsin_table[index];
//}
//
//// ʹ�ò��ұ����� arccos
//double arccos_lookup(double value) {
//    if (value < -1.0 || value > 1.0) {
//        printf("Error: Value out of range.\n");
//        return 0.0;
//    }
//    int index = find_closest_index(value, cos_table);
//    return arccos_table[index];
//}
//
//// ʹ�ò��ұ����� arctan
//double arctan_lookup(double value) {
//    int index = find_closest_index(value, tan_table);
//    return arctan_table[index];
//}
//
//// ʹ�ò��ұ����� atan2
//double atan2_lookup(double y, double x) {
//    double angle = atan2(y, x);
//    if (angle < RADIAN_MIN || angle > RADIAN_MAX) {
//        printf("Error: Angle out of range.\n");
//        return 0.0;
//    }
//    int index = (int)((angle - RADIAN_MIN) / RADIAN_STEP);
//    return arctan_table[index];
//}
//void create_table(void)
//{
//    
//        printf("const double sin_table[TABLE_SIZE]=\n");
//        printf("{\n");
//        for (int i = 0; i < TABLE_SIZE; i++)
//        {
//            printf("%.15f,\n", sin_table[i]);
//        }
//        printf("};\n");
//
//        printf("const double cos_table[TABLE_SIZE]=\n");
//        printf("{\n");
//        for (int i = 0; i < TABLE_SIZE; i++)
//        {
//            printf("%.15f,\n", cos_table[i]);
//        }
//        printf("};\n");
//
//        printf("const double tan_table[TABLE_SIZE]=\n");
//        printf("{\n");
//        for (int i = 0; i < TABLE_SIZE; i++)
//        {
//            printf("%.15f,\n", tan_table[i]);
//        }
//        printf("};\n");
//
//        printf("const double arcsin_table[TABLE_SIZE]=\n");
//        printf("{\n");
//        for (int i = 0; i < TABLE_SIZE; i++)
//        {
//            printf("%.15f,\n", arcsin_table[i]);
//        }
//        printf("};\n");
//
//        printf("const double arccos_table[TABLE_SIZE]=\n");
//        printf("{\n");
//        for (int i = 0; i < TABLE_SIZE; i++)
//        {
//            printf("%.15f,\n", arccos_table[i]);
//        }
//        printf("};\n");
//
//        printf("const double arctan_table[TABLE_SIZE]=\n");
//        printf("{\n");
//        for (int i = 0; i < TABLE_SIZE; i++)
//        {
//            printf("%.15f,\n", arctan_table[i]);
//        }
//        printf("};\n");
//    
//}

int main() {
    // 初始表
    init_tables();
    // 测试
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        printf("%.15f,\n", arctan_table[i]);
    }
    //printf("%.15f,\n", atan(0.815298612529843));

    //for (double i = 0; i <= M_PI_D2; i+= RADIAN_STEP)
    //{
    //    printf("%.5f\n", ((i ) * M_180_D_PI));
    //    printf("%.15f\n", tan_lookup(i));
    //    printf("%.15f\n", tan(i));
    //}

    //printf("%.15f\n", sin_lookup(((M_PI / 6) + 0)));
    //printf("%.15f\n", sin_lookup(((M_PI / 4) + 0)));
    //printf("%.15f\n", sin_lookup(((M_PI / 3) + 0)));
    //printf("%.15f\n", sin_lookup(((M_PI / 6) + M_PI_D2)));
    //printf("%.15f\n", sin_lookup(((M_PI / 4) + M_PI_D2)));
    //printf("%.15f\n", sin_lookup(((M_PI / 3) + M_PI_D2)));
    //printf("%.15f\n", sin((M_PI / 6) + M_PI_D2));
    //printf("%.15f\n", sin((M_PI / 4) + M_PI_D2));
    //printf("%.15f\n", sin((M_PI / 3) + M_PI_D2));

    printf("TABLE_SIZE=%d\n", TABLE_SIZE);

    return 0;
}
