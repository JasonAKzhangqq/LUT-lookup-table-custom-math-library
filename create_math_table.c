#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
//#include <stdlib.h>
//#include <time.h>
//#include "my_math.h"

//生成表文件
_Bool create_table_file(double radian_step)
{
    int table_size = ((int)(1.5707963267948965 / radian_step) + 1);//ֻ只记录0到pi/2的表;
    FILE* fp;
    fp = fopen("my_math_table.h", "w");
    if (fp == NULL) {
        return 0;
    }
    fprintf(fp, "#define _MY_MATH_TABLE_H_\n");
    fprintf(fp, "const double sin_table[%d]=\n", table_size);
    fprintf(fp, "{\n");
    for (int i = 0; i < (table_size - 1); i++)
    {
        fprintf(fp, "%.15f,\n", sin((i * radian_step)));
    }
    fprintf(fp, "%.15f,\n", (1.0));
    fprintf(fp, "};\n");

    fprintf(fp, "const double tan_table[%d]=\n", table_size);
    fprintf(fp, "{\n");
    for (int i = 0; i < (table_size - 1); i++)
    {
        fprintf(fp, "%.15f,\n", tan((i * radian_step)));
    }
    fprintf(fp, "%.1f,\n",1e15);//最后一个值是无限大
    fprintf(fp, "};\n");
    fclose(fp);
    return 1;
}

int main() {
    double input_radian_step;
    //double random_value;
    printf("generate lookup table\n");
    printf("you can enter the step size you want now\n");
    printf("the smaller the higher the accuracy, but it also takes up more space\n");
    printf("0.0001 is recommended\n");
    printf("input:");
    scanf("%lf", &input_radian_step);
    printf("---------------------------\n");
    if (create_table_file(input_radian_step))
    {
        printf("Created Successfully\n");
        printf("please keep the input value equal to the RADIAN_STEP in my_math.h\n");
    }
    else
    {
        printf("Creation failure");
        exit(0);
    }

    //测试生成表后，查表法计算有没有问题
    //printf("---------------------------");
    //printf("random test\n");
    //printf("check that the values of each two rows are equal please");
    //srand((unsigned int)time(NULL));
    

    
    printf("---------------------------\n");
    printf("PROGRAM END\n");
    printf("---------------------------\n");
    return 0;
}
