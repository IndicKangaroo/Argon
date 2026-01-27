#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "codegen.h"

------------ print helpers ------------

static void indent(int n) { while (n--) printf("  "); }

static void print_expr(NodeExpr* e, int d);

static void print_term(NodeTerm* t, int d) {
    indent(d);
    switch (t->type) {
        case TERM_INT:
            printf("INT(%d)\n", t->data.int_lit->value);
            break;
        case TERM_IDENT:
            printf("IDENT(%s)\n", t->data.ident->name);
            break;
    }
}

static void print_expr(NodeExpr* e, int d) {
    indent(d);
    switch (e->type) {
        case EXPR_TERM:
            printf("TERM:\n");
            print_term(e->data.term, d+1);
            break;

        case EXPR_UNARY:
            printf("UNARY(%s):\n", token_type_name(e->data.unary->op));
            print_expr(e->data.unary->expr, d+1);
            break;

        case EXPR_BIN:
            printf("BIN(%s):\n", token_type_name(e->data.bin->op));
            print_expr(e->data.bin->left, d+1);
            print_expr(e->data.bin->right, d+1);
            break;
    }
}

static void print_stmt(NodeStmt* s, int d) {
    indent(d);
    switch (s->type) {

        case STMT_LET:
            printf("LET %s:\n", s->data.let->name);
            print_expr(s->data.let->expr, d+1);
            break;

        case STMT_EXPR:
            printf("EXPR_STMT:\n");
            print_expr(s->data.expr->expr, d+1);
            break;

        case STMT_RETURN:
            printf("RETURN:\n");
            print_expr(s->data.ret->expr, d+1);
            break;

        case STMT_IF:
            printf("IF:\n");
            indent(d+1);
            printf("COND:\n");
            print_expr(s->data.ifs->cond, d+2);

            indent(d+1);
            printf("THEN BLOCK:\n");
            for (size_t i = 0; i < s->data.ifs->then_count; i++)
                print_stmt(s->data.ifs->then_stmts[i], d+2);

            if (s->data.ifs->else_stmts) {
                indent(d+1);
                printf("ELSE BLOCK:\n");
                for (size_t i = 0; i < s->data.ifs->else_count; i++)
                    print_stmt(s->data.ifs->else_stmts[i], d+2);
            }
            break;
    }
}

static void print_prog(NodeProg* prog) {
    printf("\n==== AST ====\n");
    for (size_t i = 0; i < prog->count; i++)
        print_stmt(prog->stmts[i], 1);
    printf("=============\n\n");
}

// ------------ main ------------

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: arg <file>\n");
        return 1;
    }

    FILE* f = fopen(argv[1], "rb");
    if (!f) { perror("open"); return 1; }

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buf = malloc(len+1);
    fread(buf,1,len,f);
    fclose(f);
    buf[len]=0;

    printf("SOURCE:\n%s\n", buf);

    lexer_init(buf);

    // printf("\n==== TOKENS ====\n");
    Token toks[4096];
    size_t n=0;
    for (;;) {
        toks[n] = lexer_next();
        // printf("%s", token_type_name(toks[n].type));
        if (toks[n].type==TOKEN_INT_LITERAL)
            // printf("(%d)", toks[n].int_value);
        if (toks[n].type==TOKEN_IDENTIFIER)
            // printf("(%s)", toks[n].lexeme);
        // printf("\n");
        if (toks[n].type==TOKEN_EOF) break;
        n++;
    }
    // printf("==============\n");

    Parser* p = parser_create(toks, n+1);
    NodeProg* prog = parse_prog(p);
    // print_prog(prog);
    //
    // printf("=== EVALUATION ===\n");
    // eval_prog(prog);

    // output assembly file
    const char* asm_file = "out.asm";
    FILE* out = fopen(asm_file, "w");
    if (!out) { perror("fopen"); exit(1); }

    codegen(prog, out);
    fclose(out);

    // assemble & link
    system("nasm -felf64 out.asm -o out.o");
    system("gcc out.o -o out");

    printf("[build complete] run ./out and inspect exit code via echo $?\n");
}

