# Argon — A Simple Compiler Toolchain

Argon is a custom compiler written in C. It takes `.arg` source files and translates them into executable programs using the GNU toolchain (gcc + gas + ld).

This project is built as a modular compiler with separate stages for lexing, parsing, and code generation.

---

## Features

- Written in C
- Modular design (lexer, parser, codegen, tokens)
- Uses GNU assembler (gas) via gcc
- Incremental builds with Make
- Automatic header dependency tracking
- Debug mode support

---

## Project Structure

```
argon/
├── Makefile
├── src/
│   ├── main.c
│   ├── lexer.c / lexer.h
│   ├── parser.c / parser.h
│   ├── token.c / token.h
│   ├── codegen.c / codegen.h
│   └── ...
├── build/        (generated)
├── tests/
│   └── test.arg
└── README.md
```

---

## Requirements

You need:

- GCC
- GNU Make

### Arch Linux
```bash
sudo pacman -S gcc make
```

### Debian/Ubuntu
```bash
sudo apt install gcc make
```

---

## Building

```bash
make
```

Creates:

```
./argon
```

---

## Running

```bash
make run
```

Or manually:

```bash
./argon tests/test.arg
```

---

## Getting the Return Value

After Argon generates the output executable (`out`), you can run it and check its exit status using:

```bash
./out
echo $?
```

`echo $?` prints the return value of the last executed program.

---

## Debug Build

```bash
make debug
```

---

## Cleaning

```bash
make clean
```

---

## Development Notes

- Source files are auto-detected
- Headers are tracked automatically
- Incremental builds enabled
- Object files stored in `build/`

---

## Usage Example

```bash
make
./argon program.arg
./out
echo $?
```

---

## Roadmap

- Optimizer
- IR
- Standard Library
- Self-hosting

---

## License

MIT License
