#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "token.h"


//custom malloc with error exit
static void* xmalloc(size_t n) {
    void* p = malloc(n);
    if (!p) { perror("malloc"); exit(1); }
    return p;
}

static Token* peek(Parser* p, int off) {
    if (p->index + off >= p->count) return NULL;
    return &p->tokens[p->index + off];
}

static Token* consume(Parser* p) {
    if (p->index >= p->count) return NULL;
    return &p->tokens[p->index++];
}

//move ahead
static Token* try_consume(Parser* p, TokenType t) {
    Token* tok = peek(p, 0);
    if (tok && tok->type == t) {
        p->index++;
        return tok;
    }
    return NULL;
}

static int prec(TokenType t) {
    switch (t) {
        case TOKEN_STAR:
        case TOKEN_SLASH:
            return 20;
        case TOKEN_PLUS:
        case TOKEN_MINUS:
            return 10;
        case TOKEN_LT:
        case TOKEN_GT:
        case TOKEN_LE:
        case TOKEN_GE:
            return 6;
        case TOKEN_EQ:
        case TOKEN_NE:
            return 5;
        default:
            return 0;
    }
}

static NodeExpr* parse_expr_prec(Parser* p, int min);


//pratt parsing + normal
static NodeExpr* parse_prefix(Parser* p) {
    Token* tok;

    if ((tok = try_consume(p, TOKEN_PLUS)) || (tok = try_consume(p, TOKEN_MINUS))) {
        NodeUnaryExpr* un = xmalloc(sizeof(NodeUnaryExpr));
        un->op = tok->type;
        un->expr = parse_prefix(p);

        NodeExpr* e = xmalloc(sizeof(NodeExpr));
        e->type = EXPR_UNARY;
        e->data.unary = un;
        return e;
    }

    if ((tok = try_consume(p, TOKEN_INT_LITERAL))) {
        NodeTermInt* t = xmalloc(sizeof(NodeTermInt));
        t->value = tok->int_value;

        NodeTerm* nt = xmalloc(sizeof(NodeTerm));
        nt->type = TERM_INT;
        nt->data.int_lit = t;

        NodeExpr* e = xmalloc(sizeof(NodeExpr));
        e->type = EXPR_TERM;
        e->data.term = nt;
        return e;
    }

    if ((tok = try_consume(p, TOKEN_IDENTIFIER))) {
        NodeTermIdent* id = xmalloc(sizeof(NodeTermIdent));
        id->name = tok->lexeme;

        NodeTerm* nt = xmalloc(sizeof(NodeTerm));
        nt->type = TERM_IDENT;
        nt->data.ident = id;

        NodeExpr* e = xmalloc(sizeof(NodeExpr));
        e->type = EXPR_TERM;
        e->data.term = nt;
        return e;
    }

    if (try_consume(p, TOKEN_LPAREN)) {
        NodeExpr* e = parse_expr_prec(p, 0);
        if (!try_consume(p, TOKEN_RPAREN)) {
            fprintf(stderr, "missing ')'\n");
            exit(1);
        }
        return e;
    }

    fprintf(stderr, "unexpected token in expression\n");
    exit(1);
}


//precedence
static NodeExpr* parse_expr_prec(Parser* p, int min) {
    NodeExpr* left = parse_prefix(p);

    for (;;) {
        Token* tok = peek(p, 0);
        if (!tok) break;
        int pr = prec(tok->type);
        if (pr <= min) break;

        consume(p);
        NodeExpr* right = parse_expr_prec(p, pr);

        NodeBinExpr* bin = xmalloc(sizeof(NodeBinExpr));
        bin->op = tok->type;
        bin->left = left;
        bin->right = right;

        NodeExpr* e = xmalloc(sizeof(NodeExpr));
        e->type = EXPR_BIN;
        e->data.bin = bin;
        left = e;
    }
    return left;
}


//expression
static NodeExpr* parse_expr(Parser* p) {
    return parse_expr_prec(p, 0);
}

static NodeStmt** parse_block(Parser* p, size_t* out_count);


//if
static NodeStmt* parse_if_stmt(Parser* p) {
    try_consume(p, TOKEN_IF);

    if (!try_consume(p, TOKEN_LPAREN)) {
        fprintf(stderr, "expected '('\n");
        exit(1);
    }

    NodeExpr* cond = parse_expr(p);

    if (!try_consume(p, TOKEN_RPAREN)) {
        fprintf(stderr, "expected ')'\n");
        exit(1);
    }

    size_t then_count = 0;
    NodeStmt** then_stmts = parse_block(p, &then_count);

    size_t else_count = 0;
    NodeStmt** else_stmts = NULL;

    if (try_consume(p, TOKEN_ELSE)) {
        else_stmts = parse_block(p, &else_count);
    }

    NodeStmtIf* is = xmalloc(sizeof(NodeStmtIf));
    is->cond = cond;
    is->then_stmts = then_stmts;
    is->then_count = then_count;
    is->else_stmts = else_stmts;
    is->else_count = else_count;

    NodeStmt* s = xmalloc(sizeof(NodeStmt));
    s->type = STMT_IF;
    s->data.ifs = is;
    return s;
}


//statement
static NodeStmt* parse_stmt(Parser* p) {
    Token* tok;
    if (peek(p,0)->type == TOKEN_IF)
        return parse_if_stmt(p);

    if ((tok = try_consume(p, TOKEN_LET))) {
        Token* id = try_consume(p, TOKEN_IDENTIFIER);
        if (!id) { fprintf(stderr, "let missing ident\n"); exit(1); }

        if (!try_consume(p, TOKEN_ASSIGN)) {
            fprintf(stderr, "let missing '='\n");
            exit(1);
        }

        NodeExpr* e = parse_expr(p);
        if (!try_consume(p, TOKEN_SEMICOLON)) {
            fprintf(stderr, "missing ';'\n");
            exit(1);
        }

        NodeStmtLet* ls = xmalloc(sizeof(NodeStmtLet));
        ls->name = id->lexeme;
        ls->expr = e;

        NodeStmt* s = xmalloc(sizeof(NodeStmt));
        s->type = STMT_LET;
        s->data.let = ls;
        return s;
    }

    if ((tok = try_consume(p, TOKEN_RETURN))) {
        NodeExpr* e = parse_expr(p);
        if (!try_consume(p, TOKEN_SEMICOLON)) {
            fprintf(stderr, "return missing ';'\n");
            exit(1);
        }

        NodeStmtReturn* rs = xmalloc(sizeof(NodeStmtReturn));
        rs->expr = e;

        NodeStmt* s = xmalloc(sizeof(NodeStmt));
        s->type = STMT_RETURN;
        s->data.ret = rs;
        return s;
    }

    NodeExpr* e = parse_expr(p);
    if (!try_consume(p, TOKEN_SEMICOLON)) {
        fprintf(stderr, "expr missing ';'\n");
        exit(1);
    }

    NodeStmtExpr* es = xmalloc(sizeof(NodeStmtExpr));
    es->expr = e;

    NodeStmt* s = xmalloc(sizeof(NodeStmt));
    s->type = STMT_EXPR;
    s->data.expr = es;
    return s;
}

//scope
static NodeStmt** parse_block(Parser* p, size_t* out_count) {
    if (!try_consume(p, TOKEN_LBRACE)) {
        fprintf(stderr, "expected '{'\n");
        exit(1);
    }

    NodeStmt** list = NULL;
    size_t count = 0, cap = 0;

    while (!try_consume(p, TOKEN_RBRACE)) {
        NodeStmt* st = parse_stmt(p);
        if (!st) { fprintf(stderr, "invalid stmt in block\n"); exit(1); }

        if (count >= cap) {
            size_t nc = cap == 0 ? 4 : cap*2;
            list = realloc(list, nc*sizeof(NodeStmt*));
            cap = nc;
        }
        list[count++] = st;
    }

    *out_count = count;
    return list;
}

//main
NodeProg* parse_prog(Parser* p) {
    NodeProg* prog = xmalloc(sizeof(NodeProg));
    prog->stmts = NULL;
    prog->count = 0;
    prog->capacity = 0;

    while (p->index < p->count && p->tokens[p->index].type != TOKEN_EOF) {
        NodeStmt* st = parse_stmt(p);
        if (prog->count >= prog->capacity) {
            size_t nc = prog->capacity==0 ? 8 : prog->capacity*2;
            prog->stmts = realloc(prog->stmts, nc*sizeof(NodeStmt*));
            prog->capacity = nc;
        }
        prog->stmts[prog->count++] = st;
    }
    return prog;
}

//create tree
Parser* parser_create(Token* toks, size_t count) {
    Parser* p = xmalloc(sizeof(Parser));
    p->tokens = toks;
    p->count = count;
    p->index = 0;
    return p;
}

void parser_destroy(Parser* p) {
    free(p);
}
