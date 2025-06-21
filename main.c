#include "globals.h"

int index_of_bp;
int basetype;
int expr_type;
int *idmain;
int token_val;
int *current_id, *symbols;
int token;
int pool_size;
int line;
int *text, *old_text, *stack;
int *pc, *bp, *sp, ax, cycle;
char *data;
char *src, *old_src;

void error(const char *fmt, ...)
{
    va_list args;
    fprintf(stderr, "\033[1;31m[Error][Line %d]: ", line);
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\033[0m\n");
    exit(-1);
}

void program()
{
    next();
    while (token > 0)
    {
        global_declaration();
    }
}

int main(int argc, char **argv)
{
    int i, fd;
    int *tmp;

    argc--;
    argv++;

    pool_size = 256 * 1024;
    line = 1;

    if ((fd = open(*argv, 0)) < 0)
    {
        error("Cannot open file: %s", *argv);
        return -1;
    }
    if (!(text = old_text = malloc(pool_size)))
    {
        error("Memory allocation failed for text area (%d bytes)", pool_size);
        return -1;
    }
    if (!(data = malloc(pool_size)))
    {
        error("Memory allocation failed for data area (%d bytes)", pool_size);
        return -1;
    }
    if (!(stack = malloc(pool_size)))
    {
        error("Memory allocation failed for stack area (%d bytes)", pool_size);
        return -1;
    }
    if (!(symbols = malloc(pool_size)))
    {
        error("Memory allocation failed for symbol table (%d bytes)", pool_size);
        return -1;
    }

    memset(text, 0, pool_size);
    memset(data, 0, pool_size);
    memset(stack, 0, pool_size);
    memset(symbols, 0, pool_size);
    bp = sp = (int *)((int)stack + pool_size);
    ax = 0;

    src = "char else enum if int return sizeof while "
          "open read close printf malloc memset memcmp exit void main";

    i = Char;
    while (i <= While)
    {
        next();
        current_id[Token] = i++;
    }

    i = OPEN;
    while (i <= EXIT)
    {
        next();
        current_id[Class] = Sys;
        current_id[Type] = INT;
        current_id[Value] = i++;
    }

    next();
    current_id[Token] = Char;
    next();
    idmain = current_id;

    if ((fd = open(*argv, 0)) < 0)
    {
        error("Cannot open file: %s", *argv);
        return -1;
    }

    if (!(src = old_src = malloc(pool_size)))
    {
        error("Memory allocation failed for source area (%d bytes)", pool_size);
        return -1;
    }

    if ((i = read(fd, src, pool_size - 1)) <= 0)
    {
        printf("read() returned %d\n", i);
        return -1;
    }
    src[i] = 0;
    close(fd);

    program();

    if (!(pc = (int *)idmain[Value]))
    {
        printf("main() not defined\n");
        return -1;
    }

    sp = (int *)((int)stack + pool_size);
    *--sp = EXIT;
    *--sp = PUSH;
    tmp = sp;
    *--sp = argc;
    *--sp = (int)argv;
    *--sp = (int)tmp;

    return eval();
}
