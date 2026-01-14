#include <ctype.h>
#include <string.h>
#include "lexer.h"

static const char *src;
static size_t pos;

void lexer_init(const char *source) {
    src = source;
    pos = 0;
}

Token lexer_next(void) {
    while (src[pos] == ' ' || src[pos] == '\n' || src[pos] == '\t')
        pos++;

    if (src[pos] == '\0')
        return (Token){ TOKEN_EOF };

    if (strncmp(&src[pos], "return", 6) == 0 &&
        !isalnum(src[pos + 6])) {
        pos += 6;
        return (Token){ TOKEN_RETURN };
        }

    if (isdigit(src[pos])) {
        int val = 0;
        while (isdigit(src[pos])) {
            val = val * 10 + (src[pos] - '0');
            pos++;
        }
        return (Token){ TOKEN_INT_LITERAL, .int_value = val };
    }

    if (src[pos] == ';') {
        pos++;
        return (Token){ TOKEN_SEMICOLON };
    }

    pos++;
    return (Token){ TOKEN_INVALID };
}
