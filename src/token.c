#include "token.h"

const char* token_type_name(TokenType t) {
    switch (t) {
        case TOKEN_RETURN: return "TOKEN_RETURN";
        case TOKEN_LET: return "TOKEN_LET";
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_INT_LITERAL: return "TOKEN_INT_LITERAL";
        case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
        case TOKEN_ASSIGN: return "TOKEN_ASSIGN";
        case TOKEN_LPAREN: return "TOKEN_LPAREN";
        case TOKEN_RPAREN: return "TOKEN_RPAREN";
        case TOKEN_LBRACE: return "TOKEN_LBRACE";
        case TOKEN_RBRACE: return "TOKEN_RBRACE";
        case TOKEN_IF: return "TOKEN_IF";
        case TOKEN_ELSE: return "TOKEN_ELSE";
        case TOKEN_PLUS: return "TOKEN_PLUS";
        case TOKEN_MINUS: return "TOKEN_MINUS";
        case TOKEN_STAR: return "TOKEN_STAR";
        case TOKEN_SLASH: return "TOKEN_SLASH";
        case TOKEN_LT: return "TOKEN_LT";
        case TOKEN_GT: return "TOKEN_GT";
        case TOKEN_LE: return "TOKEN_LE";
        case TOKEN_GE: return "TOKEN_GE";
        case TOKEN_EQ: return "TOKEN_EQ";
        case TOKEN_NE: return "TOKEN_NE";
        case TOKEN_EOF: return "TOKEN_EOF";
        case TOKEN_INVALID: return "TOKEN_INVALID";
        default: return "TOKEN_UNKNOWN";
    }
}
