#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_RETURN,
    TOKEN_LET,
    TOKEN_IDENTIFIER,
    TOKEN_INT_LITERAL,

    TOKEN_SEMICOLON,
    TOKEN_ASSIGN,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,

    TOKEN_IF,
    TOKEN_ELSE,

    // arithmetic
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,

    // relational
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_LE,
    TOKEN_GE,
    TOKEN_EQ,
    TOKEN_NE,

    TOKEN_EOF,
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    union {
        int int_value;
        const char* lexeme;
    };
} Token;

const char* token_type_name(TokenType type);

#endif
