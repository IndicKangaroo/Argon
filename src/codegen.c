#include "parser.h"
#include <stdio.h>

static FILE* out_file;

static void gen_expr(NodeExpr* e);

static void gen_term(NodeTerm* t) {
    switch (t->type) {
        case TERM_INT:
            fprintf(out_file, "    mov eax, %d\n", t->data.int_lit->value);
            break;
        case TERM_IDENT:
            // assume offset stored somewhere later
            fprintf(out_file, "    mov eax, [rbp - 4]\n"); // placeholder
            break;
    }
}

static void gen_expr(NodeExpr* e) {
    switch (e->type) {
        case EXPR_TERM:
            gen_term(e->data.term);
            break;

        case EXPR_UNARY:
            gen_expr(e->data.unary->expr);
            if (e->data.unary->op == TOKEN_MINUS)
                fprintf(out_file, "    neg eax\n");
            break;

        case EXPR_BIN: {
            // left
            gen_expr(e->data.bin->left);
            fprintf(out_file, "    push rax\n");

            // right
            gen_expr(e->data.bin->right);
            fprintf(out_file, "    mov ecx, eax\n");
            fprintf(out_file, "    pop rax\n");

            switch (e->data.bin->op) {
                case TOKEN_PLUS:  fprintf(out_file, "    add eax, ecx\n"); break;
                case TOKEN_MINUS: fprintf(out_file, "    sub eax, ecx\n"); break;
                case TOKEN_STAR:  fprintf(out_file, "    imul eax, ecx\n"); break;
                case TOKEN_SLASH: 
                    fprintf(out_file, "    cdq\n");
                    fprintf(out_file, "    idiv ecx\n");
                    break;
            }
            break;
        }
    }
}

void codegen(NodeProg* prog, FILE* out) {
    out_file = out;
    
    fprintf(out_file, "global main\n");
    fprintf(out_file, "section .text\n");
    fprintf(out_file, "main:\n");
    fprintf(out_file, "    push rbp\n");
    fprintf(out_file, "    mov rbp, rsp\n");

    for (size_t i = 0; i < prog->count; i++) {
        NodeStmt* s = prog->stmts[i];
        switch (s->type) {
            case STMT_LET:
                gen_expr(s->data.let->expr);
                fprintf(out_file, "    mov [rbp - 4], eax\n");
                break;

            case STMT_RETURN:
                gen_expr(s->data.ret->expr);
                fprintf(out_file, "    mov rsp, rbp\n");
                fprintf(out_file, "    pop rbp\n");
                fprintf(out_file, "    ret\n");
                return;

            case STMT_EXPR:
                gen_expr(s->data.expr->expr);
                break;
        }
    }

    fprintf(out_file, "    mov rsp, rbp\n");
    fprintf(out_file, "    pop rbp\n");
    fprintf(out_file, "    ret\n");
}
