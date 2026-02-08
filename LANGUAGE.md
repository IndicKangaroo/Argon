# The Argon Programming Language

Argon is a minimal, statically compiled, C-like programming language designed for learning compiler construction and low-level systems programming.

It prioritizes simplicity, explicit control, and direct translation to machine code.

---

## Design Goals

- Minimal syntax
- Predictable compilation
- No hidden runtime
- Easy-to-analyze grammar
- Direct x86-64 code generation
- Educational clarity over convenience

Argon is intended as a learning language, not a production language.

---

## Source Files

Argon programs use the `.arg` file extension.

Example:

```c
int add(int a, int b) {
    return a + b;
}

int main() {
    return add(2, 3);
}
```

---

## Data Types

Argon supports two built-in types:

| Type | Description |
|------|-------------|
| int  | 64-bit signed integer |
| void | No value |

There are currently no pointers, arrays, or structures.

---

## Keywords

```
int
void
return
if
```

---

## Operators

### Arithmetic

```
+  -  *  /
```

### Comparison

```
==  !=  <  >  <=  >=
```

### Assignment

```
=
```

---

## Delimiters

```
(  )  {  }  ,  ;
```

---

## Lexical Elements

### Identifiers

Identifiers consist of letters, digits, and underscores, and must not begin with a digit.

Example:

```
count
_sum
value42
```

---

### Integer Literals

Integer literals are base-10 signed integers.

Example:

```
0
42
-100
```

---

## Grammar (EBNF)

### Program

```ebnf
program ::= function*
```

---

### Functions

```ebnf
function ::= type IDENT "(" param_list ")" block
```

---

### Parameters

```ebnf
param_list ::= (param ("," param)*)?

param ::= type IDENT
```

---

### Types

```ebnf
type ::= "int" | "void"
```

---

### Blocks

```ebnf
block ::= "{" statement* "}"
```

---

### Statements

```ebnf
statement ::= return_stmt
            | var_decl
            | assignment
            | if_stmt
```

---

### Return Statement

```ebnf
return_stmt ::= "return" expression ";"
```

---

### Variable Declaration

```ebnf
var_decl ::= "int" IDENT "=" expression ";"
```

---

### Assignment

```ebnf
assignment ::= IDENT "=" expression ";"
```

---

### If Statement

```ebnf
if_stmt ::= "if" "(" expression ")" block
```

---

## Expressions

Operator precedence is encoded in the grammar.

```ebnf
expression ::= equality
```

```ebnf
equality ::= relational (("==" | "!=") relational)*

relational ::= additive (("<" | ">" | "<=" | ">=") additive)*

additive ::= term (("+" | "-") term)*

term ::= factor (("*" | "/") factor)*

factor ::= INT_LITERAL
         | IDENT
         | "(" expression ")"
```

---

## Semantics

### Variable Scope

- Variables are block-scoped
- No global variables
- No shadowing (current implementation)

### Functions

- Call-by-value parameters
- Stack-based calling convention
- Single return value

### Control Flow

- `if` statements do not support `else` (yet)
- No loops in current version

---

## Compilation Pipeline

Argon programs are compiled using the following stages:

1. Lexical Analysis
2. Parsing
3. Semantic Analysis
4. x86-64 Assembly Generation
5. Assembly (GAS)
6. Linking (GCC/LD)

The final output is a native Linux executable.

---

## Error Handling

The compiler reports:

- Syntax errors
- Type errors
- Undefined identifiers
- Invalid return types

Error messages include line numbers where possible.

---

## Limitations

Current limitations include:

- No loops
- No arrays
- No pointers
- No structs
- No standard library
- Linux x86-64 only

