#ifndef LEXER_H
#define LEXER_H

#include "token.h"

// Initialize the lexer with source code
void lexer_init(const char *source);

// Get the next token from the source
Token lexer_next(void);

// Peek at current position in source (for debugging)
char lexer_peek(void);

#endif
