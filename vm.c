#include "globals.h"

int eval()
{
    int op, *tmp;
    while (1)
    {
        op = *pc++;
        switch (op)
        {
        case IMM:
            ax = *pc++;
            break;
        case LC:
            ax = *(char *)ax;
            break;
        case LI:
            ax = *(int *)ax;
            break;
        case SC:
            ax = *(char *)*sp++ = ax;
            break;
        case SI:
            *(int *)*sp++ = ax;
            break;
        case PUSH:
            *--sp = ax;
            break;
        case JMP:
            pc = (int *)*pc;
            break;
        case JZ:
            pc = ax ? pc + 1 : (int *)*pc;
            break;
        case JNZ:
            pc = ax ? (int *)*pc : pc + 1;
            break;
        case CALL:
            *--sp = (int)(sp + 1);
            pc = (int *)*pc;
            break;
        case ENT:
            *--sp = (int)bp;
            bp = sp;
            sp = sp - *pc++;
            break;
        case ADJ:
            sp = sp + *pc++;
            break;
        case LEV:
            sp = bp;
            bp = (int *)*sp++;
            pc = (int *)*sp++;
            break;
        case LEA:
            ax = (int)(bp + *pc++);
            break;
        case OR:
            ax = *sp++ | ax;
            break;
        case XOR:
            ax = *sp++ ^ ax;
            break;
        case AND:
            ax = *sp++ & ax;
            break;
        case EQ:
            ax = *sp++ == ax;
            break;
        case NE:
            ax = *sp++ != ax;
            break;
        case LT:
            ax = *sp++ < ax;
            break;
        case GT:
            ax = *sp++ > ax;
            break;
        case LE:
            ax = *sp++ <= ax;
            break;
        case GE:
            ax = *sp++ >= ax;
            break;
        case SHL:
            ax = *sp++ << ax;
            break;
        case SHR:
            ax = *sp++ >> ax;
            break;
        case ADD:
            ax = *sp++ + ax;
            break;
        case SUB:
            ax = *sp++ - ax;
            break;
        case MUL:
            ax = *sp++ * ax;
            break;
        case DIV:
            ax = *sp++ / ax;
            break;
        case MOD:
            ax = *sp++ % ax;
            break;
        case EXIT:
            return *sp;
        case OPEN:
            ax = open((char *)sp[1], sp[0]);
            break;
        case CLOS:
            ax = close(*sp);
            break;
        case READ:
            ax = read(sp[2], (char *)sp[1], *sp);
            break;
        case PRTF:
            tmp = sp + pc[1];
            ax = printf((char *)tmp[-1], tmp[-2], tmp[-3], tmp[-4], tmp[-5], tmp[-6]);
            break;
        case MALC:
            ax = (int)malloc(*sp);
            break;
        case MSET:
            ax = (int)memset((char *)sp[2], sp[1], *sp);
            break;
        case MCMP:
            ax = memcmp((char *)sp[2], (char *)sp[1], *sp);
            break;
        default:
            printf("Unknown instruction: %d\n", op);
            return -1;
        }
    }
}
