#include <stdio.h>
#include <math.h>

// ����Ƕȵķ�Χ�;��ȣ��Ի���Ϊ��λ��
#define M_PI       3.1415926535897932
#define M_PI_M2    6.2831853071795862        //pi*2
#define M_PI_D2    1.5707963267948965        //pi/2 
#define M_180_D_PI  57.29577951308232        //180/pi
#define M_PI_D180  0.017453292519943        //pi/180
#define RADIAN_MIN -M_PI_D2
#define RADIAN_MAX M_PI_D2
#define RADIAN_STEP 0.001
#define TABLE_SIZE ((int)( M_PI_D2 / RADIAN_STEP ) + 1 )//ֻ����0��PI/2��ֵ

// ������ұ�
double sin_table[TABLE_SIZE];
//double cos_table[TABLE_SIZE];
//double tan_table[TABLE_SIZE];
//double arcsin_table[TABLE_SIZE];
//double arccos_table[TABLE_SIZE];
double arctan_table[TABLE_SIZE];

// ��ʼ�����ұ�
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
// ���ұ�����ӽ��ĽǶ�����
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
            return mid; // �ҵ���ȷƥ��
        }
    }

    return closest_index;
}


// �����Ǻ���ֱ�Ӳ����ͨ������Ե�����Եͣ��м䲿���ڿ�紦��һ�������Ȳ���
double sin_lookup(double radian) {
    //������Ƕ��޶���-pi��pi
    while (radian < -M_PI)
    {
        radian += M_PI_M2;
    }
    while (radian > M_PI)
    {
        radian -= M_PI_M2;
    }

    //��������ӳ�䵽���ұ�,
    if (radian < 0)
    {
        if (radian < -M_PI_D2)
        {
            //-pi��-pi/2���յ���ʽ
            return -sin_table[(int)(((M_PI +radian) / RADIAN_STEP))];
        }
        else
        {
            //-pi/2��0���յ���ʽ
            return -sin_table[(int)(((-radian) / RADIAN_STEP))];
        }
    }
    else
    {
        if (radian < M_PI_D2)
        {
            //0��pi/2��ֱ��ӳ��
            return sin_table[(int)(((radian) / RADIAN_STEP) )];
        }
        else
        {
            //pi/2��pi���յ���ʽ
            return sin_table[(int)(((M_PI-radian) / RADIAN_STEP)+0)];
        }
    }
}

double cos_lookup(double radian)
{
    //�յ���ʽ
    return sin_lookup(M_PI_D2 - radian);
}

double tan_lookup(double radian)
{
    //��Ч��ʽ
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
    // ��ʼ�����ұ�
    init_tables();
    //�������
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
