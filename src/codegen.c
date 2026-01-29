#include "parser.h"
#include <stdio.h>
#include <string.h>

static FILE* out_file;
static int label_count = 0;

// Variable tracking
#define MAX_VARS 256
static struct {
    const char* name;
    int offset;
} vars[MAX_VARS];
static int var_count = 0;
static int stack_offset = 0;

static void gen_expr(NodeExpr* e);
static void gen_stmt(NodeStmt* s);

static int find_var(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            return vars[i].offset;
        }
    }
    return 0; // Not found
}

static int add_var(const char* name) {
    if (var_count >= MAX_VARS) {
        fprintf(stderr, "Error: Too many variables\n");
        return 0;
    }
    stack_offset -= 4; // Allocate 4 bytes (int)
    vars[var_count].name = name;
    vars[var_count].offset = stack_offset;
    var_count++;
    return stack_offset;
}

static void gen_term(NodeTerm* t) {
    switch (t->type) {
        case TERM_INT:
            fprintf(out_file, "    movl $%d, %%eax\n", t->data.int_lit->value);
            break;
        case TERM_IDENT: {
            int offset = find_var(t->data.ident->name);
            if (offset == 0) {
                fprintf(stderr, "Error: Undefined variable '%s'\n", t->data.ident->name);
            }
            fprintf(out_file, "    movl %d(%%rbp), %%eax\n", offset);
            break;
        }
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
                fprintf(out_file, "    negl %%eax\n");
        break;

        case EXPR_BIN: {
            // left
            gen_expr(e->data.bin->left);
            fprintf(out_file, "    pushq %%rax\n");

            // right
            gen_expr(e->data.bin->right);
            fprintf(out_file, "    movl %%eax, %%ecx\n");
            fprintf(out_file, "    popq %%rax\n");

            switch (e->data.bin->op) {
                case TOKEN_PLUS:  fprintf(out_file, "    addl %%ecx, %%eax\n"); break;
                case TOKEN_MINUS: fprintf(out_file, "    subl %%ecx, %%eax\n"); break;
                case TOKEN_STAR:  fprintf(out_file, "    imull %%ecx, %%eax\n"); break;
                case TOKEN_SLASH:
                    fprintf(out_file, "    cltd\n");
                    fprintf(out_file, "    idivl %%ecx\n");
                    break;

                    // Comparison operators
                case TOKEN_LT:
                    fprintf(out_file, "    cmpl %%ecx, %%eax\n");
                    fprintf(out_file, "    setl %%al\n");
                    fprintf(out_file, "    movzbl %%al, %%eax\n");
                    break;
                case TOKEN_GT:
                    fprintf(out_file, "    cmpl %%ecx, %%eax\n");
                    fprintf(out_file, "    setg %%al\n");
                    fprintf(out_file, "    movzbl %%al, %%eax\n");
                    break;
                case TOKEN_LE:
                    fprintf(out_file, "    cmpl %%ecx, %%eax\n");
                    fprintf(out_file, "    setle %%al\n");
                    fprintf(out_file, "    movzbl %%al, %%eax\n");
                    break;
                case TOKEN_GE:
                    fprintf(out_file, "    cmpl %%ecx, %%eax\n");
                    fprintf(out_file, "    setge %%al\n");
                    fprintf(out_file, "    movzbl %%al, %%eax\n");
                    break;
                case TOKEN_EQ:
                    fprintf(out_file, "    cmpl %%ecx, %%eax\n");
                    fprintf(out_file, "    sete %%al\n");
                    fprintf(out_file, "    movzbl %%al, %%eax\n");
                    break;
                case TOKEN_NE:
                    fprintf(out_file, "    cmpl %%ecx, %%eax\n");
                    fprintf(out_file, "    setne %%al\n");
                    fprintf(out_file, "    movzbl %%al, %%eax\n");
                    break;
            }
            break;
        }
    }
}

static void gen_stmt(NodeStmt* s) {
    switch (s->type) {
        case STMT_LET: {
            int offset = add_var(s->data.let->name);
            gen_expr(s->data.let->expr);
            fprintf(out_file, "    movl %%eax, %d(%%rbp)\n", offset);
            break;
        }

        case STMT_RETURN:
            gen_expr(s->data.ret->expr);
            fprintf(out_file, "    movq %%rbp, %%rsp\n");
            fprintf(out_file, "    popq %%rbp\n");
            fprintf(out_file, "    ret\n");
            break;

        case STMT_EXPR:
            gen_expr(s->data.expr->expr);
            break;

        case STMT_IF: {
            int label_else = label_count++;
            int label_end = label_count++;

            // Generate condition
            gen_expr(s->data.ifs->cond);
            fprintf(out_file, "    cmpl $0, %%eax\n");
            fprintf(out_file, "    je .L%d\n", label_else);

            // Generate then block
            for (size_t i = 0; i < s->data.ifs->then_count; i++) {
                gen_stmt(s->data.ifs->then_stmts[i]);
            }
            fprintf(out_file, "    jmp .L%d\n", label_end);

            // Generate else block
            fprintf(out_file, ".L%d:\n", label_else);
            if (s->data.ifs->else_stmts) {
                for (size_t i = 0; i < s->data.ifs->else_count; i++) {
                    gen_stmt(s->data.ifs->else_stmts[i]);
                }
            }

            fprintf(out_file, ".L%d:\n", label_end);
            break;
        }
    }
}

void codegen(NodeProg* prog, FILE* out) {
    out_file = out;
    label_count = 0;  // Reset label counter
    var_count = 0;    // Reset variable counter
    stack_offset = 0; // Reset stack offset

    fprintf(out_file, ".global main\n");
    fprintf(out_file, ".text\n");
    fprintf(out_file, "main:\n");
    fprintf(out_file, "    pushq %%rbp\n");
    fprintf(out_file, "    movq %%rsp, %%rbp\n");
    fprintf(out_file, "    subq $256, %%rsp\n");  // Allocate stack space

    for (size_t i = 0; i < prog->count; i++) {
        gen_stmt(prog->stmts[i]);
    }

    fprintf(out_file, "    movq %%rbp, %%rsp\n");
    fprintf(out_file, "    popq %%rbp\n");
    fprintf(out_file, "    ret\n");
}
