# RJIT

## EBNF of RJitLang

```ebnf
trans_unit ::= [trans_unit](declation | func_def)
declation  ::= var_decl
var_decl   ::= "var" var_def {',' var_def} ':' prime_type';'
var_def    ::= ident ['=' init_value]
init_value ::= expr

func_def    ::= "def" ident'('[func_params]')' [func_type] block
func_type   ::= 'void' | prime_type
func_params ::= func_param {',' func_param}
func_param  ::= ident prime_type
block       ::= '{' block_items '}'
block_items ::= declation | stmt

stmt ::= lval '=' expr ';' | [expr]';' | block 
       | if_stmt 
       | while_stmt

if_stmt    ::= 'if' cond stmt ['else' stmt] 
while_stmt ::= 'while' cond stmt
expr       ::= add_expr
prime_type ::= "int" | "string" | "char"
```