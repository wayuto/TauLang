#include "globals.h"

void statement()
{
    int *a, *b;

    switch (token)
    {
    case If:
        match(If);
        match('(');
        expression(Assign);
        match(')');

        *++text = JZ;
        b = ++text;

        statement();
        if (token == Else)
        {
            match(Else);

            *b = (int)(text + 3);
            *++text = JMP;
            b = ++text;

            statement();
        }

        *b = (int)(text + 1);
        break;
    case While:
        match(While);

        a = text + 1;

        match('(');
        expression(Assign);
        match(')');

        *++text = JZ;
        b = ++text;

        statement();

        *++text = JMP;
        *++text = (int)a;
        *b = (int)(text + 1);
        break;
    case '{':
        match('{');

        while (token != '}')
        {
            statement();
        }

        match('}');
        break;
    case Return:
        match(Return);

        if (token != ';')
        {
            expression(Assign);
        }

        match(';');

        *++text = LEV;
        break;
    case ';':
        match(';');
        break;
    default:
        expression(Assign);
        match(';');
        break;
    }
}