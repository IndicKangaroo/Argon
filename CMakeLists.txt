# Argon Compiler Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRCDIR = src
OBJDIR = obj
BINDIR = .

# Source files
SOURCES = $(SRCDIR)/main.c \
          $(SRCDIR)/lexer.c \
          $(SRCDIR)/parser.c \
          $(SRCDIR)/token.c \
          $(SRCDIR)/codegen.c

# Object files
OBJECTS = $(OBJDIR)/main.o \
          $(OBJDIR)/lexer.o \
          $(OBJDIR)/parser.o \
          $(OBJDIR)/token.o \
          $(OBJDIR)/codegen.o

# Target executable
TARGET = $(BINDIR)/arg

# Default target
all: $(TARGET)

# Create obj directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Build complete! Run './arg <file.arg>' to compile a program."

# Compile source files to object files
$(OBJDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/lexer.h $(SRCDIR)/parser.h $(SRCDIR)/token.h $(SRCDIR)/codegen.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/lexer.o: $(SRCDIR)/lexer.c $(SRCDIR)/lexer.h $(SRCDIR)/token.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/parser.o: $(SRCDIR)/parser.c $(SRCDIR)/parser.h $(SRCDIR)/token.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/token.o: $(SRCDIR)/token.c $(SRCDIR)/token.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/codegen.o: $(SRCDIR)/codegen.c $(SRCDIR)/codegen.h $(SRCDIR)/parser.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR)
	rm -f $(TARGET)
	rm -f out.asm out.o out
	@echo "Clean complete!"

# Clean and rebuild
rebuild: clean all

# Run tests (you can add test files here)
test: $(TARGET)
	@echo "Running tests..."
	@if [ -f tests/test1.arg ]; then \
		./arg tests/test1.arg && ./out; \
		echo "Test 1 exit code: $$?"; \
	fi

# Install (copy to /usr/local/bin or similar)
install: $(TARGET)
	@echo "Installing to /usr/local/bin (requires sudo)..."
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation complete! You can now run 'arg' from anywhere."

# Uninstall
uninstall:
	@echo "Removing from /usr/local/bin..."
	sudo rm -f /usr/local/bin/arg
	@echo "Uninstall complete!"

# Show help
help:
	@echo "Argon Compiler Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Build the compiler"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make rebuild  - Clean and rebuild"
	@echo "  make test     - Run test suite"
	@echo "  make install  - Install to /usr/local/bin"
	@echo "  make uninstall- Remove from /usr/local/bin"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Usage:"
	@echo "  ./arg <file.arg>  - Compile an Argon source file"
	@echo "  ./out             - Run the compiled program"
	@echo "  echo $$?          - View the exit code"

.PHONY: all clean rebuild test install uninstall help
