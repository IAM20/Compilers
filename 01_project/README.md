# Project 1 Scanner

## Using-lex scanner example
<p align=center>
<img src="https://user-images.githubusercontent.com/35682872/46332622-2e3d6f80-c658-11e8-8203-bfaae92a4fda.gif"/>
</p>

********************************

## Tiny-Compiler modified scanner example
<p align=center>
<img src="https://user-images.githubusercontent.com/35682872/46332622-2e3d6f80-c658-11e8-8203-bfaae92a4fda.gif"/>
</p>

<p align=center>
<img src="https://img.shields.io/badge/gcc-5.xx-green.svg"/>
<img src="https://img.shields.io/badge/dependencies-make-green.svg"/>
<img src="https://img.shields.io/badge/dependencies-lex-green.svg"/>
</p>

**************************

### How to build
You can build both scanner with `make`.

```bash
$ make
```

### Build status

|program | Ubuntu 18.04 | Windows |
|----  |   ----   | ---- |
|tiny-compiler<br>modified|  <img src="https://img.shields.io/badge/build-passing-green.svg"/> | <img src="https://img.shields.io/badge/build-failure-red.svg"/> |
| using-lex | <img src="https://img.shields.io/badge/build-passing-green.svg"/> | <img src="https://img.shields.io/badge/build-failure-red.svg"/> |



### How to generate test
Both scanner has test c-minus code. <br> If you call `test.sh` you can test scanner.
```bash
$ ./tiny-compiler-modified/test.sh
$ ./using-lex/test.sh
```

## How it works?
### Tiny-compiler-modified
Tiny-compiler has own dfa. This compiler does not using lex.<br>
So, I changed source codes. 
`globals.h, scan.c, util.c, main.c`<br>
I just add some new tokens `int, while, etc...` <br>
And modified, comment case in scan.c. <br>

*****************************
### Using-lex
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

