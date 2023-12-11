#include "recursivedescent.h"

#include "dsl.h"

Node* GetE(const char* expression, size_t* curPos);

Node* GetT(const char* expression, size_t* curPos);

Node* GetP(const char* expression, size_t* curPos);

Node* GetId(const char* expression, size_t* curPos);

Node* GetN(const char* expression, size_t* curPos);

#define curSym expression[*curPos]

Node* GetG(const char* expression)
{
    AssertSoft(expression, NULL);

    size_t curPos = 0;

    Node* root = GetE(expression, &curPos);

    return root;
}

Node* GetE(const char* expression, size_t* curPos)
{
    Node* firstNode = GetT(expression, curPos);

    while (curSym == '+' || curSym == '-')
    {
        char func = curSym;

        (*curPos)++;

        Node* secondNode = GetT(expression, curPos);

        switch (func)
        {
            case '+':
                firstNode = ADD_(firstNode, secondNode);
                break;
            
            case '-':
                firstNode = SUB_(firstNode, secondNode);
                break;

            default:
                printf("Unknown function: %d! Position in expression: %d.\n", func, *curPos);
                break;
        }
    }

    return firstNode;
}

Node* GetT(const char* expression, size_t* curPos)
{
    Node* firstNode = GetP(expression, curPos);

    while (curSym == '*' || curSym == '/' || curSym == '^')
    {
        char func = curSym;

        (*curPos)++;

        Node* secondNode = GetP(expression, curPos);

        switch (func)
        {
            case '*':
                firstNode = MUL_(firstNode, secondNode);
                break;
            
            case '/':
                firstNode = DIV_(firstNode, secondNode);
                break;

            case '^':
                firstNode = POW_(firstNode, secondNode);
                break;
            
            default:
                printf("Unknown function: %d! Position in expression: %d.\n", func, curPos);
                break;
        }
    }

    return firstNode;
}

Node* GetP(const char* expression, size_t* curPos)
{
    Node* firstNode = NULL;

    if (curSym == '(')
    {
        (*curPos)++;

        firstNode = GetE(expression, curPos);

        AssertSoft(curSym == ')', NULL); // TODO: print error message for language

        (*curPos)++;

        return firstNode;
    }
    else if (isdigit(curSym))
    {
        return GetN(expression, curPos);
    }
    else
    {
        return GetId(expression, curPos);
    }
}

#define SIZEOF_STR(str) sizeof(str) - 1

Node* GetId(const char* expression, size_t* curPos)
{
    printf("curpos: %d\n", *curPos);

    printf("%d, %c\n", strncmp("sin", &curSym, 3), curSym);

    if      (strcmp("sin", &curSym) == 0 && expression[*curPos + SIZEOF_STR("sin")] == '(')
    {
        *curPos += SIZEOF_STR("sin");

        return SIN_(GetE(expression, curPos));
    }
    else if (strcmp("cos", &curSym) == 0 && expression[*curPos + SIZEOF_STR("cos")] == '(')
    {
        *curPos += SIZEOF_STR("cos");

        return COS_(GetE(expression, curPos));
    }
    else if (strcmp("tg", &curSym) == 0 && expression[*curPos + SIZEOF_STR("tg")] == '(')
    {
        *curPos += SIZEOF_STR("tg");

        return TG_(GetE(expression, curPos));
    }
    else if (strcmp("ctg", &curSym) == 0 && expression[*curPos + SIZEOF_STR("ctg")] == '(')
    {
        *curPos += SIZEOF_STR("ctg");

        return CTG_(GetE(expression, curPos));
    }
    else if (strcmp("ln", &curSym) == 0 && expression[*curPos + SIZEOF_STR("ln")] == '(') // check for '(' inside
    {
        *curPos += SIZEOF_STR("ln");

        return LN_(GetE(expression, curPos));
    }
    else if (isalpha(curSym))
    {
        *curPos += 1;

        return VAR_(expression[*curPos - 1]);
    }
    else
    {
        printf("Unrecognised ID: %c, position: %d\n", curSym, *curPos);
        return NULL;
    }
}

Node* GetN(const char* expression, size_t* curPos)
{
    char* endPtr = NULL;

    double value = strtof(&curSym, &endPtr);

    *curPos += endPtr - &curSym;

    return NUM_(value);
}

// TODO: don't forget other functions too :P

// TODO: поддержка числа pi и e