#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_RETURN,
    TOKEN_INT_LITERAL,
    TOKEN_IDENTIFIER,
    TOKEN_SEMICOLON,
    TOKEN_EOF,
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    union {
        int int_value;        //int
        const char *lexeme;   //every thing else
    };
} Token;

// Function to get token name as string
const char* token_type_name(TokenType type);

#endif