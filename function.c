#include "globals.h"

void function_parameter() {
    int type;
    int params;
    params = 0;
    while (token != ')') {
        type = INT;
        if (token == Int) {
            match(Int);
        } else if (token == Char) {
            type = CHAR;
            match(Char);
        }


        while (token == Mul) {
            match(Mul);
            type = type + PTR;
        }


        if (token != Id) {
            printf("%d: bad parameter declaration\n", line);
            exit(-1);
        }
        if (current_id[Class] == Loc) {
            printf("%d: duplicate parameter declaration\n", line);
            exit(-1);
        }

        match(Id);

        current_id[BClass] = current_id[Class];
        current_id[Class] = Loc;
        current_id[BType] = current_id[Type];
        current_id[Type] = type;
        current_id[BValue] = current_id[Value];
        current_id[Value] = params++;

        if (token == ',') {
            match(',');
        }
    }
    index_of_bp = params + 1;
}

void function_body() {
    int pos_local;
    int type;
    pos_local = index_of_bp;

    while (token == Int || token == Char) {
        basetype = (token == Int) ? INT : CHAR;
        match(token);

        while (token != ';') {
            type = basetype;
            while (token == Mul) {
                match(Mul);
                type = type + PTR;
            }

            if (token != Id) {
                printf("%d: bad local declaration\n", line);
                exit(-1);
            }
            if (current_id[Class] == Loc) {
                printf("%d: duplicate local declaration\n", line);
                exit(-1);
            }
            match(Id);


            current_id[BClass] = current_id[Class];
            current_id[Class] = Loc;
            current_id[BType] = current_id[Type];
            current_id[Type] = type;
            current_id[BValue] = current_id[Value];
            current_id[Value] = ++pos_local;

            if (token == ',') {
                match(',');
            }
        }
        match(';');
    }


    *++text = ENT;
    *++text = pos_local - index_of_bp;


    while (token != '}') {
        statement();
    }


    *++text = LEV;
}
