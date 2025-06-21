#include "globals.h"

void statement() {
    int *a, *b;

    if (token == If) {
        match(If);
        match('(');
        expression(Assign);
        match(')');


        *++text = JZ;
        b = ++text;

        statement();
        if (token == Else) {
            match(Else);


            *b = (int) (text + 3);
            *++text = JMP;
            b = ++text;

            statement();
        }

        *b = (int) (text + 1);
    } else if (token == While) {
        match(While);

        a = text + 1;

        match('(');
        expression(Assign);
        match(')');

        *++text = JZ;
        b = ++text;

        statement();

        *++text = JMP;
        *++text = (int) a;
        *b = (int) (text + 1);
    } else if (token == '{') {
        match('{');

        while (token != '}') {
            statement();
        }

        match('}');
    } else if (token == Return) {
        match(Return);

        if (token != ';') {
            expression(Assign);
        }

        match(';');


        *++text = LEV;
    } else if (token == ';') {
        match(';');
    } else {
        expression(Assign);
        match(';');
    }
}
