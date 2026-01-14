#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Incorrect usage. Correct usage is:\n");
        fprintf(stderr, "arg <input.arg>\n");
        return 1;
    }

    //----reading file---------
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("arg");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    if (!buffer) {
        fprintf(stderr, "arg: error: out of memory\n");
        fclose(f);
        return 1;
    }

    size_t read = fread(buffer, 1, length, f);
    if (read != length) {
        fprintf(stderr, "arg: error: failed to read file\n");
        free(buffer);
        fclose(f);
        return 1;
    }

    buffer[length] = '\0';
    fclose(f);

    printf("Source:\n%s\n\n", buffer);

    //------tokenization call--------
    printf("Tokens:\n");
    lexer_init(buffer);
    for (;;) {
        Token tok = lexer_next();
        if (tok.type == TOKEN_EOF)
            break;

        printf("%-20s", token_type_name(tok.type));



        if (tok.type == TOKEN_INT_LITERAL) {
            printf(" value: %d", tok.int_value);
        }
        printf("\n");
    }

    free(buffer);
    return 0;
}