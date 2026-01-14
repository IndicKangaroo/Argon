#include "token.h"

const char* token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_RETURN:       return "TOKEN_RETURN";
        case TOKEN_INT_LITERAL:  return "TOKEN_INT_LITERAL";
        case TOKEN_IDENTIFIER:   return "TOKEN_IDENTIFIER";
        case TOKEN_SEMICOLON:    return "TOKEN_SEMICOLON";
        case TOKEN_EOF:          return "TOKEN_EOF";
        case TOKEN_INVALID:      return "TOKEN_INVALID";
        default:                 return "TOKEN_UNKNOWN";
    }
}