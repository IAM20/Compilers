# Project 1 Scanner
<p align=center>
<img src="https://user-images.githubusercontent.com/35682872/46332622-2e3d6f80-c658-11e8-8203-bfaae92a4fda.gif"/>
</p>

<p align=center>
<img src="https://img.shields.io/badge/gcc-5.xx-green.svg"/>
<img src="https://img.shields.io/badge/dependencies-make-green.svg"/>
<img src="https://img.shields.io/badge/dependencies-lex-green.svg"/>
</p>


### How to build
```bash
$ make
```
### Build status

|branch | Ubuntu 18.04 |
|----  |   ----   |
|master|  <img src="https://img.shields.io/badge/build-passing-green.svg"/> |



### How to generate test
```bash
$ ./test.sh
```

### How it works?
I wanna use lex program's outpus as much as possible<br>
So, `scan.h` has `lex.yy.c`'s resource. (which comes from lex) <br>
```cpp
int yylex();
extern File *yyin;
extern char *yytext;
```
And C--'s lex file is cminus.l <br>
So, everytime building scanner, Makefile calls `lex` and then, compiles it.<br>
This means if you don't have `lex` scanner cannot be built.<br>

