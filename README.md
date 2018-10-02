# Compiler Assignment
> Professor : Yong-jun park<br> Student number : 2013008264 <br> Student name : Young-dae Lee

*******************************
<p align=center>
<img src="https://img.shields.io/badge/gcc-5.xx-green.svg"/>
<img src="https://img.shields.io/badge/dependencies-make-green.svg"/>
<img src="https://img.shields.io/badge/dependencies-lex-green.svg"/>
</p>

## Project #1 : Scanner
> Implementation of C-Scanner using lex.
 * [globals.h](https://hconnect.hanyang.ac.kr/2018_ELE4029_10734/2018_ELE4029_2013008264/blob/master/01_project/src/globals.h)
 * [main.c](https://hconnect.hanyang.ac.kr/2018_ELE4029_10734/2018_ELE4029_2013008264/blob/master/01_project/src/main.c)
 * [scan.h](https://hconnect.hanyang.ac.kr/2018_ELE4029_10734/2018_ELE4029_2013008264/blob/master/01_project/src/scan.h)
 * [util.c](https://hconnect.hanyang.ac.kr/2018_ELE4029_10734/2018_ELE4029_2013008264/blob/master/01_project/src/util.c)
 * [util.h](https://hconnect.hanyang.ac.kr/2018_ELE4029_10734/2018_ELE4029_2013008264/blob/master/01_project/src/util.h)

### Build dependencies
> GCC 5.xx or more. <br>
> Make <br>
> lex

### Build status

|branch | Ubuntu 18.04 |
|----  |   ----   |
|master|  <img src="https://img.shields.io/badge/build-passing-green.svg"/> |

### How to build
> in 01_project directory
```bash
$ make
```
> If you wanna generate test, (also in 01_project directory)
```bash
$ test.sh
```

scan.h uses `lex.yy.c` and c-- 's lex file is [cminus.l](https://hconnect.hanyang.ac.kr/2018_ELE4029_10734/2018_ELE4029_2013008264/blob/master/01_project/src/cminus.l)<br>
I use lex program's outputs as much as possible. <br>
So, below's function and variables have been used.
```cpp
int yylex();
extern FILE *yyin;
extern char *yytext;
```

### Tests
Test code will be updated soon!

