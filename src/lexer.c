#include <ctype.h>
#include <string.h>
#include "lexer.h"

#include <stdlib.h>

#include "token.h"

static const char* src;
static size_t pos;

void lexer_init(const char* s) {
    src = s;
    pos = 0;
}

static int is_ident_start(char c) {
    return isalpha(c) || c == '_';
}

static int is_ident(char c) {
    return isalnum(c) || c == '_';
}

Token lexer_next(void) {
    while (src[pos] == ' ' || src[pos] == '\t' || src[pos] == '\n')
        pos++;

    char c = src[pos];
    if (c == '\0')
        return (Token){ TOKEN_EOF };

    // keywords
    if (strncmp(&src[pos], "return", 6) == 0 && !is_ident(src[pos+6])) {
        pos += 6;
        return (Token){TOKEN_RETURN};
    }
    if (strncmp(&src[pos], "let", 3) == 0 && !is_ident(src[pos+3])) {
        pos += 3;
        return (Token){TOKEN_LET};
    }
    if (strncmp(&src[pos], "if", 2) == 0 && !is_ident(src[pos+2])) {
        pos += 2;
        return (Token){TOKEN_IF};
    }
    if (strncmp(&src[pos], "else", 4) == 0 && !is_ident(src[pos+4])) {
        pos += 4;
        return (Token){TOKEN_ELSE};
    }

    // identifiers
    if (is_ident_start(c)) {
        size_t start = pos;
        while (is_ident(src[pos])) pos++;
        Token t = { TOKEN_IDENTIFIER };
        size_t len = pos - start;
        char* s = malloc(len+1);
        memcpy(s, &src[start], len);
        s[len]=0;
        t.lexeme = s;
        return t;
    }

    // numbers
    if (isdigit(c)) {
        int v = 0;
        while (isdigit(src[pos])) {
            v = v*10 + (src[pos]-'0');
            pos++;
        }
        Token t = {TOKEN_INT_LITERAL};
        t.int_value = v;
        return t;
    }

    // relational (longest first)
    if (c=='<' && src[pos+1]=='=') { pos+=2; return (Token){TOKEN_LE}; }
    if (c=='>' && src[pos+1]=='=') { pos+=2; return (Token){TOKEN_GE}; }
    if (c=='=' && src[pos+1]=='=') { pos+=2; return (Token){TOKEN_EQ}; }
    if (c=='!' && src[pos+1]=='=') { pos+=2; return (Token){TOKEN_NE}; }

    if (c=='<') { pos++; return (Token){TOKEN_LT}; }
    if (c=='>') { pos++; return (Token){TOKEN_GT}; }

    // arithmetic
    if (c=='+') { pos++; return (Token){TOKEN_PLUS}; }
    if (c=='-') { pos++; return (Token){TOKEN_MINUS}; }
    if (c=='*') { pos++; return (Token){TOKEN_STAR}; }
    if (c=='/') { pos++; return (Token){TOKEN_SLASH}; }

    // punctuation
    if (c=='=') { pos++; return (Token){TOKEN_ASSIGN}; }
    if (c==';') { pos++; return (Token){TOKEN_SEMICOLON}; }
    if (c=='(') { pos++; return (Token){TOKEN_LPAREN}; }
    if (c==')') { pos++; return (Token){TOKEN_RPAREN}; }
    if (c=='{') { pos++; return (Token){TOKEN_LBRACE}; }
    if (c=='}') { pos++; return (Token){TOKEN_RBRACE}; }

    pos++;
    return (Token){TOKEN_INVALID};
}
