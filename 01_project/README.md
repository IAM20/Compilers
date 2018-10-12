# Project 1 Scanner

## Using-lex scanner build & sample test
<p align=center>
<img src="https://user-images.githubusercontent.com/35682872/46450970-0c1a2d80-c7cf-11e8-8f8c-153d5c7acb11.gif"/>
</p>

********************************

## Tiny-Compiler-modified scanner build & sample test
<p align=center>
<img src="https://user-images.githubusercontent.com/35682872/46450968-0b819700-c7cf-11e8-9f0a-b014f6afc345.gif"/>
</p>

<p align=center>
<img src="https://img.shields.io/badge/gcc-5.xx-green.svg"/>
<img src="https://img.shields.io/badge/dependencies-make-green.svg"/>
<img src="https://img.shields.io/badge/dependencies-flex-green.svg"/>
</p>

**************************

### How to build
You can build both scanner with `make`.

```bash
$ make
```

### Build status

|program | Ubuntu 18.04 | Windows | X OS |
|----  |   ----   | ---- | ---- |
|tiny-compiler<br>modified|  <img src="https://img.shields.io/badge/build-passing-green.svg"/> | <img src="https://img.shields.io/badge/build-failure-red.svg"/> | <img src="https://img.shields.io/badge/build-failure-red.svg"/>  |
| using-lex | <img src="https://img.shields.io/badge/build-passing-green.svg"/> | <img src="https://img.shields.io/badge/build-failure-red.svg"/> | <img src="https://img.shields.io/badge/build-passing-green.svg"/>  |

### How to generate test
Both scanner has test c-minus code. <br> If you call `test.sh` you can test scanner.
```bash
$ ./tiny-compiler-modified/test.sh
$ ./using-lex/test.sh
```

## How it works?
### Tiny-compiler-modified
Tiny-compiler has own dfa. This compiler does not using flex.<br>
So, I changed source codes. 
`globals.h, scan.c, util.c, main.c`<br>
I just add some new tokens `int, while, etc...` <br>
And modified, comment case in scan.c. <br>

#### This compiler cannot be built in OS X. (10/9/2018) <br>
Error message.
```shell
clang: error: unable to execute command: Segmentation fault: 11
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Apple LLVM version 10.0.0 (clang-1000.10.44.2)
Target: x86_64-apple-darwin18.0.0
Thread model: posix
InstalledDir: /Library/Developer/CommandLineTools/usr/bin
clang: note: diagnostic msg: PLEASE submit a bug report to http://developer.apple.com/bugreporter/ and include the crash backtrace, preprocessed source, and associated run script.
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /var/folders/d8/8qsxwt_14bqgvf4kh6r4qpph0000gn/T/parse-bbf491.c
clang: note: diagnostic msg: /var/folders/d8/8qsxwt_14bqgvf4kh6r4qpph0000gn/T/parse-bbf491.sh
clang: note: diagnostic msg: Crash backtrace is located in
clang: note: diagnostic msg: /Users/ydlee/Library/Logs/DiagnosticReports/clang_<YYYY-MM-DD-HHMMSS>_<hostname>.crash
clang: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang: note: diagnostic msg: 

********************
make: *** [src/parse.o] Error 254
```

*****************************
### Using-lex
I wanna use lex program's outputs as much as possible<br>
So, `scan.h` has `lex.yy.c`'s resource. (which comes from flex) <br>
```cpp
int yylex();
extern File *yyin;
extern char *yytext;
```
And C--'s lex file is cminus.l <br>
So, everytime building scanner, Makefile calls `lex` and then, compiles it.<br>
This means if you don't have `flex` scanner cannot be built.<br>

********************************************
# Korean ver.
### Tiny-compiler modified
Tiny compiler 의 소스코드를 조금 바꿔서 만든 Scanner 입니다. Tiny-compiler가 기존에, 자기 자신의 Scanner를 가지고 있기 땨문에, flex 를 사용하지 않습니다., 덕분에 flex 가 없어도 빌드가 가능합니다. 그래서 그냥 소스 코드를 바꿔서 사용했으며, 바꾼 소스는 
`globals.h, scan.c, util.c, main.c` 가 있습니다. 그리고, c-minus 의 spec에 따라, 몇가지의 토큰이 추가되었습니다. `int, while etc...`<br> 

***2018년 10월 9일 기준으로 OS X에서 빌드 되지 않습니다.***
<br>error message
```shell
clang: error: unable to execute command: Segmentation fault: 11
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Apple LLVM version 10.0.0 (clang-1000.10.44.2)
Target: x86_64-apple-darwin18.0.0
Thread model: posix
InstalledDir: /Library/Developer/CommandLineTools/usr/bin
clang: note: diagnostic msg: PLEASE submit a bug report to http://developer.apple.com/bugreporter/ and include the crash backtrace, preprocessed source, and associated run script.
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /var/folders/d8/8qsxwt_14bqgvf4kh6r4qpph0000gn/T/parse-bbf491.c
clang: note: diagnostic msg: /var/folders/d8/8qsxwt_14bqgvf4kh6r4qpph0000gn/T/parse-bbf491.sh
clang: note: diagnostic msg: Crash backtrace is located in
clang: note: diagnostic msg: /Users/ydlee/Library/Logs/DiagnosticReports/clang_<YYYY-MM-DD-HHMMSS>_<hostname>.crash
clang: note: diagnostic msg: (choose the .crash file that corresponds to your crash)
clang: note: diagnostic msg: 

********************
make: *** [src/parse.o] Error 254
```

***********************************
### Using-lex
flex 를 사용한 Scanner 입니다. flex 를 최대한 효율적이고, 또, flex의 결과물을 최대한 편안하게 활용하기 위해서, scan.h 파일에 `lex.yy.c`의 함수와, 변수가 선언되어있습니다.
```cpp
int yylex()
extern FILE *yyin;
extern char *yytext;
```
이 세 개가 그것이며, 이 세 개는 util.c 에서 아주 유용하게 사용됩니다. c-- 의 scanner spec 은 cminus.l 에 정의 되어있으며, makefile 은 cminus.l 을 사용합니다. 그리고, flex 프로그램도 항상 부르기 때문에, flex 가 없으면, 빌드되지 않습니다.

***OS X에서 빌드가 됩니다.***