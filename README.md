# XY-Lang

## build

```shell script
git clone https://github.com/Explainaur/XL-Lang
cd XL-Lang && mkdir build && cd build
cmake ..
make -j8
./xycc a.xy
```

## EBNF of XY-Lang

```ebnf
trans_unit  ::= [trans_unit](declation | func_def)
declation   ::= var_decl
var_decl    ::= "var" var_def {',' var_def} ':' prime_type';'
var_def     ::= ident ['=' init_value]
init_value  ::= expr

func_def    ::= "def" ident'('[func_params]')' [func_type] block
func_type   ::= 'void' | prime_type
func_params ::= func_param {',' func_param}
func_param  ::= ident prime_type
block       ::= '{' block_items '}'
block_items ::= declation | stmt

stmt        ::= lval '=' expr ';' | [expr]';' | block 
              | if_stmt 
              | while_stmt

if_stmt     ::= 'if' cond stmt ['else' stmt] 
while_stmt  ::= 'while' cond stmt
expr        ::= add_expr
prime_type  ::= "int" | "string" | "char"
```


## Not finish yet
I am a little lazy. So the EBNF is not complete.

You can run the code below by `./xycc a.xy`:

```
def a(x int, y int) int {
  var a = 1 : int;
  while x + y > a {
    a = a + 1;
  }
  return a;
}

def main() int {

    return a(1, 2);
}
```

And you can get:
```llvm
define i32 @a(i32 %x, i32 %y) {
entry:
  %x.addr = alloca i32
  %y.addr = alloca i32
  store i32 %x, i32* %x.addr
  %retval = alloca i32
  store i32 %y, i32* %y.addr
  %0 = alloca i32
  store i32 1, i32* %0
  br label %while.cond0

while.cond0: ; preds: body, loop.body0
  %1 = load i32, i32* %x.addr
  %2 = load i32, i32* %y.addr
  %3 = add i32 %1, %2
  %4 = load i32, i32* %0
  %5 = icmp sgt i32 %3, %4
  br i1 %5, label %loop.body0, label %while.end0


loop.body0: ; preds: while.cond0
  %6 = load i32, i32* %0
  %7 = add i32 %6, 1
  store i32 %7, i32* %0
  br label %while.cond0

while.end0: ; preds: while.cond0
  %8 = load i32, i32* %0
  store i32 %8, i32* %retval
  %9 = load i32, i32* %retval
  ret i32 %9
}

define i32 @main() {
entry:
  %retval = alloca i32
  %0 = call i32 @a(i32 1, i32 2)
  store i32 %0, i32* %retval
  %1 = load i32, i32* %retval
  ret i32 %1
}
```