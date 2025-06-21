#include "globals.h"


void enum_declaration() {
    int i;
    i = 0;
    while (token != '}') {
        if (token != Id) {
            printf("%d: bad enum identifier %d\n", line, token);
            exit(-1);
        }
        next();
        if (token == Assign) {
            next();
            if (token != Num) {
                printf("%d: bad enum initializer\n", line);
                exit(-1);
            }
            i = token_val;
            next();
        }

        current_id[Class] = Num;
        current_id[Type] = INT;
        current_id[Value] = i++;

        if (token == ',') {
            next();
        }
    }
}

void global_declaration() {
    int type;
    int i;

    basetype = INT;

    if (token == Enum) {
        match(Enum);
        if (token != '{') {
            match(Id);
        }
        if (token == '{') {
            match('{');
            enum_declaration();
            match('}');
        }

        match(';');
        return;
    }


    if (token == Int) {
        match(Int);
    } else if (token == Char) {
        match(Char);
        basetype = CHAR;
    }


    while (token != ';' && token != '}') {
        type = basetype;

        while (token == Mul) {
            match(Mul);
            type = type + PTR;
        }

        if (token != Id) {
            printf("%d: bad global declaration\n", line);
            exit(-1);
        }
        if (current_id[Class]) {
            printf("%d: duplicate global declaration\n", line);
            exit(-1);
        }
        match(Id);
        current_id[Type] = type;

        if (token == '(') {
            current_id[Class] = Fun;
            current_id[Value] = (int) (text + 1);
            function_declaration();
        } else {
            current_id[Class] = Glo;
            current_id[Value] = (int) data;
            data = data + sizeof(int);
        }

        if (token == ',') {
            match(',');
        }
    }
    next();
}

void function_declaration() {
    match('(');
    function_parameter();
    match(')');
    match('{');
    function_body();


    current_id = symbols;
    while (current_id[Token]) {
        if (current_id[Class] == Loc) {
            current_id[Class] = current_id[BClass];
            current_id[Type] = current_id[BType];
            current_id[Value] = current_id[BValue];
        }

        current_id = current_id + IdSize;
    }
}
