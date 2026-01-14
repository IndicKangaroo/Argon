# Argon

*A small hobby compiler written in C*

---

## Overview

Argon is a **minimal statically typed language** I built to learn more about compiler design and low-level concepts.

It supports:

* Variables and functions
* `return` statements
* `if` control flow (no loops)
* Integer arithmetic with correct operator precedence

Only two data types exist:

* `int`
* `void`

---

## Example

```text
def add (int:a int:b){
    return a + b;
}

def (){
    return 0;
}
```

---

## Tokens

### Keywords

```
def, return, if, int, void
```

### Operators

```
+  -  *  /
=  ==  !=
<  >  <=  >=
```

### Delimiters

```
(  )  {  }  :  ;
```

### Other

* `IDENT` — identifier
* `INT_LITERAL` — integer constant

---

## Grammar (EBNF)

### Program

```ebnf
program       ::= function*
```

---

### Functions

```ebnf
function      ::= "def" IDENT? "(" param_list ")" block
```

Function names are optional.

---

### Parameters

```ebnf
param_list    ::= (param)*

param         ::= type ":" IDENT
```

Parameters are space-separated (no commas).

---

### Types

```ebnf
type ::= "int" | "void"
```

---

### Blocks and Statements

```ebnf
block         ::= "{" statement* "}"

statement     ::= return_stmt
                | var_decl
                | assignment
                | if_stmt
```

---

### Return

```ebnf
return_stmt ::= "return" expression ";"
```

---

### Variables

```ebnf
var_decl   ::= "int" IDENT "=" expression ";"
assignment ::= IDENT "=" expression ";"
```

---

### If Statement

```ebnf
if_stmt ::= "if" "(" expression ")" block
```

---

## Expressions

Operator precedence is handled directly in the grammar.

```ebnf
expression ::= equality
```

```ebnf
equality   ::= relational (("==" | "!=") relational)*

relational ::= additive (("<" | ">" | "<=" | ">=") additive)*

additive   ::= term (("+" | "-") term)*

term       ::= factor (("*" | "/") factor)*

factor     ::= INT_LITERAL
             | IDENT
             | "(" expression ")"
```

---

