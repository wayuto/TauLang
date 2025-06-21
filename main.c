#include "globals.h"

int debug;
int assembly;
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

void program() {
    next();
    while (token > 0) {
        global_declaration();
    }
}

int main(int argc, char **argv) {
    int i, fd;
    int *tmp;

    argc--;
    argv++;


    if (argc > 0 && **argv == '-' && (*argv)[1] == 's') {
        assembly = 1;
        --argc;
        ++argv;
    }
    if (argc > 0 && **argv == '-' && (*argv)[1] == 'd') {
        debug = 1;
        --argc;
        ++argv;
    }
    if (argc < 1) {
        printf("usage: xc [-s] [-d] file ...\n");
    }

    if ((fd = open(*argv, 0)) < 0) {
        printf("could not open(%s)\n", *argv);
        return -1;
    }

    pool_size = 256 * 1024;
    line = 1;


    if (!(text = malloc(pool_size))) {
        printf("could not malloc(%d) for text area\n", pool_size);
        return -1;
    }
    if (!(data = malloc(pool_size))) {
        printf("could not malloc(%d) for data area\n", pool_size);
        return -1;
    }
    if (!(stack = malloc(pool_size))) {
        printf("could not malloc(%d) for stack area\n", pool_size);
        return -1;
    }
    if (!(symbols = malloc(pool_size))) {
        printf("could not malloc(%d) for symbol table\n", pool_size);
        return -1;
    }

    memset(text, 0, pool_size);
    memset(data, 0, pool_size);
    memset(stack, 0, pool_size);
    memset(symbols, 0, pool_size);

    old_text = text;

    src = "char else enum if int return sizeof while "
            "open read close printf malloc memset memcmp exit void main";


    i = Char;
    while (i <= While) {
        next();
        current_id[Token] = i++;
    }


    i = OPEN;
    while (i <= EXIT) {
        next();
        current_id[Class] = Sys;
        current_id[Type] = INT;
        current_id[Value] = i++;
    }

    next();
    current_id[Token] = Char;
    next();
    idmain = current_id;

    if (!(src = old_src = malloc(pool_size))) {
        printf("could not malloc(%d) for source area\n", pool_size);
        return -1;
    }

    if ((i = read(fd, src, pool_size - 1)) <= 0) {
        printf("read() returned %d\n", i);
        return -1;
    }
    src[i] = 0;
    close(fd);

    program();

    if (!(pc = (int *) idmain[Value])) {
        printf("main() not defined\n");
        return -1;
    }


    if (assembly) {
        return 0;
    }


    sp = (int *) ((int) stack + pool_size);
    *--sp = EXIT;
    *--sp = PUSH;
    tmp = sp;
    *--sp = argc;
    *--sp = (int) argv;
    *--sp = (int) tmp;

    return eval();
}
