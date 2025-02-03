# my_math

#### Links to original open source libraries
https://gitee.com/jasonakzhang/my_math

#### Introduction
Table lookup method implements common functions in mathematics library. It contains sin, cos, tan, asin, acos, atan, atan2, sqrt, pow. (sqrt, pow is not a table lookup method)
Achieve moderate precision with moderate efficiency under moderate table, such as generated table step size is 0.0001, table file size is 615KB, the calculation accuracy is the worst 7 decimal places. Under the loose test, the calculation time is slightly longer than math.h, and the table lookup method is 0.02 seconds longer in (1<<30) random value calculations.

#### Software Architecture
Written in c language, no structure, beginners can understand. This implementation method is by no means optimal. The following is a description of the program implementation.
The square root takes into account the fact that sometimes the data will be large, so it does not consider using the table lookup method for the time being, and directly uses the Newton approximation method. Taking into account the fact that there will be no negative power, there is no function of doing negative power. Binary table lookup is used for inverse trigonometric function lookup. All tables have only 0 to pi/2, and other domains can be evaluated by symmetry and induction formulas. Cosine can be converted to sin by induction formula, which can save space for cos_table, and the same is true for asin and acos. The tan table is different from the sin table, although it can be converted into sin and cos expressions by induction formulas, but atan needs tan tables, and atan2 is used more, so a separate tan table is created to ensure efficiency.

#### installation tutorial
1. Place my_math.c and my_math.h into the project.
2. Use create_math_table to generate lookup table my_math_table.h and put the table file into the project. After my_math_create.exe is executed, a step value is entered. The smaller the value, the higher the precision, but the larger the space it occupies. The exe file is pre-compiled for Windows and can be run directly, or if you are on another platform, you can run it with create_math_table.c. Keep the entered step value the same as TABLE_STEP in my_math.h.

#### Usage instructions
1. Include my_math.h in your project, and you can use the functions declared therein in the same way as math.h. For example, if you want to calculate sin(value), you can use sin_lookup(value).

#### Contribute
1. Me
2. Each AI
3. Open source projects

#### Special effects
1. No stunts

#### Other, are not related to the program
The development background of this project:
I am a stinking play intelligent car, cross-country, with Infineon TC chip, found in the optimization of the program if on the double-precision floating-point fpu can not be used, that is, the mathematical function has failed to calculate, but double precision is more important for us, after all, single-precision floating-point even longitude and latitude can not be stable expression, Under the advice of the senior brothers and various groups of friends, it is still convenient to write a math library of their own. At that time, it was the eve of the New Year holiday, just write a math library without a single chip computer to stay around, afraid of the New Year holiday idle boredom, the task of writing math library in the New Year holiday. Before to see what git is a programmer's standard configuration, learn a little, learn to have something to practice ah, alas just combined with this small project, both skilled in the use of git, but also to in-depth understanding of the project, but also to promote the progress of the car.
