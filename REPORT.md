# Compiler 과제 최종 보고서
## Test & Build

<p align=center>
<img src="https://user-images.githubusercontent.com/35682872/48176062-e3e39880-e351-11e8-8926-5f1928254a09.gif"/>
<br>
<img src="https://img.shields.io/badge/gcc-5.xx-green.svg"/>
<img src="https://img.shields.io/badge/dependencies-make-green.svg"/>
<img src="https://img.shields.io/badge/dependencies-flex-green.svg"/>
<img src="https://img.shields.io/badge/dependencies-bison-green.svg">
<br>
해당 프로젝트는 마지막 프로젝트인 Semantic analysis 입니다.
</p>

## Build environment
| Compiler | OS | dependencies |
|---|---|---|
| GCC | macOS, Ubuntu | flex, bison | 

Cygwin 과 같은 윈도우 GCC, Make 를 받아서 빌드하면, 빌드는 가능합니다만, 추천하지 않습니다.<br>

## Implementation
Semantic analysis 의 구현은 쉽지 않았습니다. 구현하면서, AST 를 만들 때 "아! 이런 부분이 필요하구나.." 하고 새로 만든 기억만 여러가지입니다.<br>
Semantic analysis 를 하기 위해서는 먼저 필요한것이 AST 입니다. 이 AST를 만들기 위해서 우리는 Parser 를 만들었고 Parser 를 만들기 위해서 Scanner 를 만들었습니다.<br>
Scanner 와 Parser 는 이미 문서화를 마쳤고, 해당 소스코드는 거의 전부 재사용되었습니다. <br>
> [Scanner document](https://github.com/IAM20/Compilers/tree/master/01_project) <br>
> [Parser document](https://github.com/IAM20/Compilers/tree/master/02_project)
<br>
이제, Semantic analysis 의 세부적인 구현을 설명하려고 합니다.

### More details
Semantic analysis 는 AST 를 분석합니다. 이 AST 를 분석해서 Scope 라는 것을 만들어내고 Scope 안의 정보들을 분석합니다. 때문에 우리는 지금까지 없던 Scope 라는 structure 를 만들 필요가 있습니다.
```cpp
/* author : IAM20
 * in symtab.h
 */
typedef struct scope {
  char * name;
  Bucket bucket[221]; // 221 is just prime number for hash.
  struct scope * parent;
} * Scope;
```
이렇게 scope 라는 것을 만들고 이 scope 를 만드는 함수를 만듭니다.<br>
scope 는 정확히 Stack 의 행동패턴과 같기 때문에, stack으로 구현합니다. <br>

```cpp
/* author : IAM20
 * in symtab.h
 */
Scope currScope();
Scope newScope(char *);
void popScope();
void pushScope(Scope scope);
```
<br>
이제 scope 라는 것을 AST 를 분석하여 만들 수 있습니다. <br>
이제 scope 의 속을 채워넣어야 합니다. 아까 설명하지 않고 넘어갔던 Bucket 에 대해서 설명하겠습니다.

```cpp
/* author : IAM20
 * in symtab.h
 */
typedef struct bucket {
  char * name;
  ExpectedType expectedType;
  LineList lines;
  int memloc;
  int paramNum;
  struct bucket * next;
} * Bucket;
```

이것은 해당 id에 대한 정보를 넣어놓는 Bucket 입니다. <br>
이 id 는 type 과 자신이 함수인지 아닌지의 정보를 가지고 있습니다. <br>
자신이 몇 번째 줄에서 사용되는지의 정보도 가지고 있습니다. <br>
우리는 이 Bucket 을 Scope 에 넣어서 확인할 것입니다. <br>
이렇게 만들어진 것을 Symbol table 이라고 할것입니다. <br>
<br>

이 Symbol table 을 만드는 과정에 대해서 알아보겠습니다. <br>
이 symbol table 은 `semanticAnalysis` 라는 함수에서 생성되고 소비됩니다. <br>

```cpp
/* author : IAM20
 * in symtab.h
 */

void 
semanticAnalysis (TreeNode * syntaxTree) {
  if(syntaxTree == NULL) {
    printf("Invalid AST\n");
    return;
  }
  scopeStackTop = scopeArraySize = 0;
  Scope globalScope = newScope("global");
  pushScope(globalScope);
  stInsert("global", "input", Integer, 0, globalCounter++, 0);
  stInsert("global", "output", Void, 0, globalCounter++, 1);
  

  if(traverse(syntaxTree, insertNode, popAfterInsertProc)) {
    printf("There is semantic error!\n");
  }
  popScope();
}
```

이것이 전체적인 semantic analysis 의 flow 입니다. 맨 처음 symbol_table 에 집어넣는 작업인 stInsert 가 input, output 이라는 내장함수를 집어넣는 모습을 볼 수 있습니다.
그 이후 traverse라는 함수가 보이는데 이것이 사실 semantic analysis 를 하게됩니다. <br>
tree traverse 를 하면서 Symbol table 을 만들고, C-minus 의 특성상 declaration 이 먼저 나올 수 밖에 없기 때문에 동시에 type checking 도 가능합니다. <br>
<br>
이 때 조금 귀찮은 것이 있는데, Type checking 을 할 때, 해당 id의 type이 AST 에서는 알 수 없다는 점 때문입니다. <br>
때문에, Semantic analyzer 가 id 의 type을 찾고, 해당 type으로 비교해야합니다.<br> 
id가 여러번 나와도 매번 비교해야 하니 조금 귀찮은 작업이 되어서, 함수를 따로 빼서 사용했습니다. <br>

```cpp
/* author : IAM20
 * in symtab.h
 */
ExpectedType
getTypeOfID (TreeNode * t) {
  Bucket bucket;
  bucket = stLookupWithScope(currScope(), t->attr.name);
  if(bucket == NULL) {
    printf("Line %d : identifier %s is not declared\n", t->lineno, t->attr.name);
    printf("Terminate the compilation, please check line number %d first\n", t->lineno);
    exit(1);
  }
  return bucket->expectedType;
}
```

이렇게 하여, AST 에서 type checking 이 필요한 부분이 나오면, type check 를 하게됩니다.

최종적인 main.c 의 모습은 아래와 같습니다.
```cpp
/* author : IAM20
 * in main function.
 */

  AST = parse();
  semanticAnalysis(AST);
  printSymTab();
```
parse 함수는 scanner 를 사용하고, AST 를 생성합니다.<br>
semanticAnalysis 함수는 AST 를 사용해서 semantic error 를 찾아냅니다.<br>
모든 분석이 끝난 후, 생성된 symbol table 을 print 합니다.<br>
<br>
이를 flow 로 표현하면 아래와 같습니다.

![flow](https://user-images.githubusercontent.com/35682872/49925361-0130f700-fefc-11e8-8337-ab63a3a91678.png)


이로써, 이번학기 project1, project2, project3 이 모두 종료되었습니다.

### Results
Catch 할 수 있는 error 는 아래와 같습니다.
> index type error <br>
> assign type error <br>
> function call parameter error <br>
> void type id error <br>
> function call error <br>
> return type error <br>
> Syntax error<br>
> Lexical error<br>

Syntax spec 은 `cminus.y` 에 정의되어 있습니다. <br>
Lexical spec 은 `cminus.l` 에 정의되어 있습니다.<br>