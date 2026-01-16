# Argon Compiler

A simple compiler for the Argon programming language that compiles to x86-64 assembly.

## Features

- **Arithmetic expressions**: `+`, `-`, `*`, `/`
- **Comparison operators**: `<`, `>`, `<=`, `>=`, `==`, `!=`
- **Variables**: Declare and use variables with `let`
- **Control flow**: `if`/`else` statements
- **Return statements**: Return values from your program

## Prerequisites

- GCC (GNU Compiler Collection)
- NASM (Netwide Assembler)
- Linux x86-64 system

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get install build-essential nasm
```

**Fedora/RHEL:**
```bash
sudo dnf install gcc nasm
```

**Arch Linux:**
```bash
sudo pacman -S gcc nasm
```

## Building the Compiler

Clone the repository and build:

```bash
git clone https://github.com/IndicKangaroo/Argon
cd Argon
mkdir build
cd build
cmake ..
make
```

This will create the `arg` executable.

## Usage

### Writing an Argon Program

Create a file with the `.arg` extension. Here's a simple example:

**example.arg:**
```
let x = 10;
let y = 20;
let result = x + y * 2;
return result;
```

### Compiling and Running

```bash
./argon./out example.arg
./out
echo $?
```

The program's return value will be printed (in this case: 50).

## Language Syntax

### Variables

```
let variable_name = expression;
```

### Arithmetic Expressions

```
let sum = 5 + 3;
let product = 4 * 7;
let complex = (10 + 5) * 2 - 3;
```

### Unary Operators

```
let negative = -42;
let positive = +15;
```

### Comparison Operators

```
let less = 5 < 10;        // returns 1 (true) or 0 (false)
let greater = 10 > 5;
let equal = 5 == 5;
let not_equal = 5 != 3;
let less_equal = 5 <= 5;
let greater_equal = 10 >= 5;
```

### Control Flow

```
if (x > 10) {
    let result = x * 2;
    return result;
} else {
    return x;
}
```

### Return Statements

```
return expression;
```

## Example Programs

### Example 1: Basic Arithmetic

**arithmetic.arg:**
```
let a = 15;
let b = 7;
let sum = a + b;
let diff = a - b;
let prod = a * b;
return prod;
```

### Example 2: Conditional Logic

**conditional.arg:**
```
let x = 25;
if (x > 20) {
    return 1;
} else {
    return 0;
}
```

### Example 3: Complex Expression

**complex.arg:**
```
let x = 10;
let y = 5;
let z = 3;
let result = (x + y) * z - y / z;
return result;
```

## Running Examples

```bash
# Compile the compiler
make

# Run example 1
./arg arithmetic.arg
./out
echo $?  # Should print 105

# Run example 2
./arg conditional.arg
./out
echo $?  # Should print 1

# Run example 3
./arg complex.arg
./out
echo $?  # Should print 44
```

## Project Structure

```
argon/
├── src/
│   ├── main.c       # Entry point and driver
│   ├── lexer.c      # Tokenization
│   ├── lexer.h
│   ├── parser.c     # AST generation
│   ├── parser.h
│   ├── token.c      # Token definitions
│   ├── token.h
│   ├── codegen.c    # x86-64 code generation
│   └── codegen.h
├── Makefile
└── README.md
```

## Compilation Pipeline

1. **Lexical Analysis**: Source code → Tokens
2. **Parsing**: Tokens → Abstract Syntax Tree (AST)
3. **Code Generation**: AST → x86-64 Assembly
4. **Assembly**: Assembly → Object file (via NASM)
5. **Linking**: Object file → Executable (via GCC)

## Cleaning Up

```bash
make clean
```

This removes all build artifacts including object files, assembly files, and executables.

## Limitations

- Only integer types supported
- Variables are stored at fixed stack offsets (limited variable support)
- No function definitions (only main implicit function)
- No loops (while/for)
- No boolean type (uses integers: 0 = false, non-zero = true)




## Acknowledgments

Built as a learning project to understand compiler construction fundamentals.
