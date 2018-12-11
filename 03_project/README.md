# Project 3 - Semantic analysis for C - Minus
## **Build & Sample test**

<p align=center>
<img src="https://user-images.githubusercontent.com/35682872/48176062-e3e39880-e351-11e8-8926-5f1928254a09.gif"/>
</p>

<p align=center>
<img src="https://img.shields.io/badge/gcc-5.xx-green.svg"/>
<img src="https://img.shields.io/badge/dependencies-make-green.svg"/>
<img src="https://img.shields.io/badge/dependencies-flex-green.svg"/>
<img src="https://img.shields.io/badge/dependencies-bison-green.svg">
</p>

****************************
### **How to build**
You can build this parse with `make`
```bash
$ make
```

### **Build status**
|program | Ubuntu 18.04 | Windows | X OS |
|----  |   ----   | ---- | ---- |
|Analyzer|  <img src="https://img.shields.io/badge/build-passing-green.svg"/> | <img src="https://img.shields.io/badge/build-failure-red.svg"/> | <img src="https://img.shields.io/badge/build-passing-green.svg"/>  |

### **How to generate test**
Test this parser with shell script.
```bash
$ ./test.sh
```
## **How it works?**
This is upgrade version of project 2. We use AST which is made in project 2. The compiler will analyze the AST, and report the errors.

## **Reports**
I write the reports about these whole projects in Korean.