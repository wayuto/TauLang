#include "globals.h"

void match(int tk)
{
    if (token == tk)
    {
        next();
    }
    else
    {
        error("Expected token: %c", tk);
    }
}

void expression(int level)
{
    int *id;
    int tmp;
    int *addr;
    {
        if (!token)
        {
            error("Unexpected token EOF of expression");
        }
        switch (token)
        {
        case Num:
            match(Num);
            *++text = IMM;
            *++text = token_val;
            expr_type = INT;
            break;
        case '"':
            *++text = IMM;
            *++text = token_val;
            match('"');
            while (token == '"')
            {
                match('"');
            }
            data = (char *)(((int)data + sizeof(int)) & (-sizeof(int)));
            expr_type = PTR;
            break;
        case Sizeof:
            match(Sizeof);
            match('(');
            expr_type = INT;
            if (token == Int)
            {
                match(Int);
            }
            else if (token == Char)
            {
                match(Char);
                expr_type = CHAR;
            }
            while (token == Mul)
            {
                match(Mul);
                expr_type = expr_type + PTR;
            }
            match(')');
            *++text = IMM;
            *++text = (expr_type == CHAR) ? sizeof(char) : sizeof(int);
            expr_type = INT;
            break;
        case Id:
            match(Id);
            id = current_id;
            if (token == '(')
            {
                match('(');
                tmp = 0;
                while (token != ')')
                {
                    expression(Assign);
                    *++text = PUSH;
                    tmp++;
                    if (token == ',')
                    {
                        match(',');
                    }
                }
                match(')');
                if (id[Class] == Sys)
                {
                    *++text = id[Value];
                }
                else if (id[Class] == Fun)
                {
                    *++text = CALL;
                    *++text = id[Value];
                }
                else
                {
                    printf("%d: bad function call\n", line);
                    exit(-1);
                }
                if (tmp > 0)
                {
                    *++text = ADJ;
                    *++text = tmp;
                }
                expr_type = id[Type];
            }
            else if (id[Class] == Num)
            {
                *++text = IMM;
                *++text = id[Value];
                expr_type = INT;
            }
            else
            {
                if (id[Class] == Loc)
                {
                    *++text = LEA;
                    *++text = index_of_bp - id[Value];
                }
                else if (id[Class] == Glo)
                {
                    *++text = IMM;
                    *++text = id[Value];
                }
                else
                {
                    printf("%d: undefined variable\n", line);
                    exit(-1);
                }
                expr_type = id[Type];
                *++text = (expr_type == Char) ? LC : LI;
            }
            break;
        case '(':
        {
            match('(');
            if (token == Int || token == Char)
            {
                tmp = (token == Char) ? CHAR : INT;
                match(token);
                while (token == Mul)
                {
                    match(Mul);
                    tmp = tmp + PTR;
                }
                match(')');
                expression(Inc);
                expr_type = tmp;
            }
            else
            {
                expression(Assign);
                match(')');
            }
            break;
        }
        case Mul:
            match(Mul);
            expression(Inc);
            if (expr_type >= PTR)
            {
                expr_type = expr_type - PTR;
            }
            else
            {
                printf("%d: bad dereference\n", line);
                exit(-1);
            }
            *++text = (expr_type == CHAR) ? LC : LI;
            break;
        case And:
            match(And);
            expression(Inc);
            if (*text == LC || *text == LI)
            {
                text--;
            }
            else
            {
                printf("%d: bad address of\n", line);
                exit(-1);
            }
            expr_type = expr_type + PTR;
            break;
        case '!':
            match('!');
            expression(Inc);
            *++text = PUSH;
            *++text = IMM;
            *++text = 0;
            *++text = EQ;
            expr_type = INT;
            break;
        case '~':
            match('~');
            expression(Inc);
            *++text = PUSH;
            *++text = IMM;
            *++text = -1;
            *++text = XOR;
            expr_type = INT;
            break;
        case Add:
            match(Add);
            expression(Inc);
            expr_type = INT;
            break;
        case Sub:
            match(Sub);
            if (token == Num)
            {
                *++text = IMM;
                *++text = -token_val;
                match(Num);
            }
            else
            {
                *++text = IMM;
                *++text = -1;
                *++text = PUSH;
                expression(Inc);
                *++text = MUL;
            }
            expr_type = INT;
            break;
        case Inc:
        case Dec:
            tmp = token;
            match(token);
            expression(Inc);
            if (*text == LC)
            {
                *text = PUSH;
                *++text = LC;
            }
            else if (*text == LI)
            {
                *text = PUSH;
                *++text = LI;
            }
            else
            {
                printf("%d: bad lvalue of pre-increment\n", line);
                exit(-1);
            }
            *++text = PUSH;
            *++text = IMM;
            *++text = (expr_type > PTR) ? sizeof(int) : sizeof(char);
            *++text = (tmp == Inc) ? ADD : SUB;
            *++text = (expr_type == CHAR) ? SC : SI;
            break;
        default:
            printf("%d: bad expression\n", line);
            exit(-1);
        }
    }
    {
        while (token >= level)
        {
            tmp = expr_type;
            if (token == Assign)
            {
                match(Assign);
                if (*text == LC || *text == LI)
                {
                    *text = PUSH;
                }
                else
                {
                    printf("%d: bad lvalue in assignment\n", line);
                    exit(-1);
                }
                expression(Assign);

                expr_type = tmp;
                *++text = (expr_type == CHAR) ? SC : SI;
            }
            else if (token == Cond)
            {
                match(Cond);
                *++text = JZ;
                addr = ++text;
                expression(Assign);
                if (token == ':')
                {
                    match(':');
                }
                else
                {
                    printf("%d: missing colon in conditional\n", line);
                    exit(-1);
                }
                *addr = (int)(text + 3);
                *++text = JMP;
                addr = ++text;
                expression(Cond);
                *addr = (int)(text + 1);
            }
            else if (token == Lor)
            {
                match(Lor);
                *++text = JNZ;
                addr = ++text;
                expression(Lan);
                *addr = (int)(text + 1);
                expr_type = INT;
            }
            else if (token == Lan)
            {
                match(Lan);
                *++text = JZ;
                addr = ++text;
                expression(Or);
                *addr = (int)(text + 1);
                expr_type = INT;
            }
            else if (token == Or)
            {
                match(Or);
                *++text = PUSH;
                expression(Xor);
                *++text = OR;
                expr_type = INT;
            }
            else if (token == Xor)
            {
                match(Xor);
                *++text = PUSH;
                expression(And);
                *++text = XOR;
                expr_type = INT;
            }
            else if (token == And)
            {
                match(And);
                *++text = PUSH;
                expression(Eq);
                *++text = AND;
                expr_type = INT;
            }
            else if (token == Eq)
            {
                match(Eq);
                *++text = PUSH;
                expression(Ne);
                *++text = EQ;
                expr_type = INT;
            }
            else if (token == Ne)
            {
                match(Ne);
                *++text = PUSH;
                expression(Lt);
                *++text = NE;
                expr_type = INT;
            }
            else if (token == Lt)
            {
                match(Lt);
                *++text = PUSH;
                expression(Shl);
                *++text = LT;
                expr_type = INT;
            }
            else if (token == Gt)
            {
                match(Gt);
                *++text = PUSH;
                expression(Shl);
                *++text = GT;
                expr_type = INT;
            }
            else if (token == Le)
            {
                match(Le);
                *++text = PUSH;
                expression(Shl);
                *++text = LE;
                expr_type = INT;
            }
            else if (token == Ge)
            {
                match(Ge);
                *++text = PUSH;
                expression(Shl);
                *++text = GE;
                expr_type = INT;
            }
            else if (token == Shl)
            {
                match(Shl);
                *++text = PUSH;
                expression(Add);
                *++text = SHL;
                expr_type = INT;
            }
            else if (token == Shr)
            {
                match(Shr);
                *++text = PUSH;
                expression(Add);
                *++text = SHR;
                expr_type = INT;
            }
            else if (token == Add)
            {
                match(Add);
                *++text = PUSH;
                expression(Mul);

                expr_type = tmp;
                if (expr_type > PTR)
                {
                    *++text = PUSH;
                    *++text = IMM;
                    *++text = sizeof(int);
                    *++text = MUL;
                }
                *++text = ADD;
            }
            else if (token == Sub)
            {
                match(Sub);
                *++text = PUSH;
                expression(Mul);
                if (tmp > PTR && tmp == expr_type)
                {
                    *++text = SUB;
                    *++text = PUSH;
                    *++text = IMM;
                    *++text = sizeof(int);
                    *++text = DIV;
                    expr_type = INT;
                }
                else if (tmp > PTR)
                {
                    *++text = PUSH;
                    *++text = IMM;
                    *++text = sizeof(int);
                    *++text = MUL;
                    *++text = SUB;
                    expr_type = tmp;
                }
                else
                {
                    *++text = SUB;
                    expr_type = tmp;
                }
            }
            else if (token == Mul)
            {
                match(Mul);
                *++text = PUSH;
                expression(Inc);
                *++text = MUL;
                expr_type = tmp;
            }
            else if (token == Div)
            {
                match(Div);
                *++text = PUSH;
                expression(Inc);
                *++text = DIV;
                expr_type = tmp;
            }
            else if (token == Mod)
            {
                match(Mod);
                *++text = PUSH;
                expression(Inc);
                *++text = MOD;
                expr_type = tmp;
            }
            else if (token == Inc || token == Dec)
            {
                if (*text == LI)
                {
                    *text = PUSH;
                    *++text = LI;
                }
                else if (*text == LC)
                {
                    *text = PUSH;
                    *++text = LC;
                }
                else
                {
                    printf("%d: bad value in increment\n", line);
                    exit(-1);
                }

                *++text = PUSH;
                *++text = IMM;
                *++text = (expr_type > PTR) ? sizeof(int) : sizeof(char);
                *++text = (token == Inc) ? ADD : SUB;
                *++text = (expr_type == CHAR) ? SC : SI;
                *++text = PUSH;
                *++text = IMM;
                *++text = (expr_type > PTR) ? sizeof(int) : sizeof(char);
                *++text = (token == Inc) ? SUB : ADD;
                match(token);
            }
            else if (token == Brak)
            {
                match(Brak);
                *++text = PUSH;
                expression(Assign);
                match(']');

                if (tmp > PTR)
                {
                    *++text = PUSH;
                    *++text = IMM;
                    *++text = sizeof(int);
                    *++text = MUL;
                }
                else if (tmp < PTR)
                {
                    printf("%d: pointer type expected\n", line);
                    exit(-1);
                }
                expr_type = tmp - PTR;
                *++text = ADD;
                *++text = (expr_type == CHAR) ? LC : LI;
            }
            else
            {
                printf("%d: compiler error, token = %d\n", line, token);
                exit(-1);
            }
        }
    }
}