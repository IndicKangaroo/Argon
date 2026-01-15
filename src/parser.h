#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include "token.h"

// ==== Forward ====
typedef struct NodeExpr NodeExpr;
typedef struct NodeStmt NodeStmt;

// ==== Terms ====
typedef enum {
    TERM_INT,
    TERM_IDENT
} TermType;

typedef struct {
    int value;
} NodeTermInt;

typedef struct {
    const char* name;
} NodeTermIdent;

typedef struct {
    TermType type;
    union {
        NodeTermInt* int_lit;
        NodeTermIdent* ident;
    } data;
} NodeTerm;

// ==== Expressions ====
typedef enum {
    EXPR_TERM,
    EXPR_UNARY,
    EXPR_BIN
} ExprType;

typedef struct {
    TokenType op;
    NodeExpr* expr;
} NodeUnaryExpr;

typedef struct {
    TokenType op;
    NodeExpr* left;
    NodeExpr* right;
} NodeBinExpr;

struct NodeExpr {
    ExprType type;
    union {
        NodeTerm* term;
        NodeUnaryExpr* unary;
        NodeBinExpr* bin;
    } data;
};

// ==== Statements ====
typedef enum {
    STMT_LET,
    STMT_RETURN,
    STMT_EXPR,
    STMT_IF
} StmtType;

typedef struct {
    const char* name;
    NodeExpr* expr;
} NodeStmtLet;

typedef struct {
    NodeExpr* expr;
} NodeStmtReturn;

typedef struct {
    NodeExpr* expr;
} NodeStmtExpr;

typedef struct {
    NodeExpr* cond;
    NodeStmt** then_stmts;
    size_t then_count;
    NodeStmt** else_stmts; // may be NULL
    size_t else_count;
} NodeStmtIf;

struct NodeStmt {
    StmtType type;
    union {
        NodeStmtLet* let;
        NodeStmtReturn* ret;
        NodeStmtExpr* expr;
        NodeStmtIf* ifs;
    } data;
};

// ==== Program ====
typedef struct {
    NodeStmt** stmts;
    size_t count;
    size_t capacity;
} NodeProg;

// ==== Parser ====
typedef struct {
    Token* tokens;
    size_t count;
    size_t index;
} Parser;

Parser* parser_create(Token* toks, size_t count);
NodeProg* parse_prog(Parser* p);
void parser_destroy(Parser* p);

#endif
