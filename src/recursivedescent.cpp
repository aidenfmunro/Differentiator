#include "recursivedescent.h"

#include "dsl.h"

Node* GetE();

Node* GetT();

Node* GetP();

Node* GetId();

Node* GetN();

const char* expression = "(x*tg(x)+sin(x))^(x+sin(x))";

int curPos = 0;

Node* GetG()
{
    AssertSoft(expression, NULL);

    // expression = expr;

    Node* root = GetE();

    return root;
}

Node* GetE()
{
    Node* firstNode = GetT();

    while (expression[curPos] == '+' || expression[curPos] == '-')
    {
        char func = expression[curPos];

        curPos++;

        Node* secondNode = GetT();

        switch (func)
        {
            case '+':
                firstNode = ADD_(firstNode, secondNode);
                break;
            
            case '-':
                firstNode = SUB_(firstNode, secondNode);
                break;

            default:
                printf("Unknown function: %d! Position in expression: %d.\n", func, curPos);
                break;
        }
    }

    return firstNode;
}

Node* GetT()
{
    Node* firstNode = GetP();

    while (expression[curPos] == '*' || expression[curPos] == '/' || expression[curPos] == '^')
    {
        char func = expression[curPos];

        curPos++;

        Node* secondNode = GetP();

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

Node* GetP()
{
    Node* firstNode = NULL;

    if (expression[curPos] == '(')
    {
        curPos++;

        firstNode = GetE();

        AssertSoft(expression[curPos] == ')', NULL); // TODO: print error message for language

        curPos++;

        return firstNode;
    }
    else if (isdigit(expression[curPos]))
    {
        return GetN();
    }
    else
    {
        return GetId();
    }
}

Node* GetId()
{
    printf("curpos: %d\n", curPos);

    printf("%d, %c\n", strncmp("sin", &expression[curPos], 3), expression[curPos]);

    if (strncmp("sin", &expression[curPos], 3) == 0 && expression[curPos + 3] == '(')
    {
        curPos += 3;

        return SIN_(GetE());
    }
    else if (strncmp("cos", &expression[curPos], 3) == 0 && expression[curPos + 3] == '(')
    {
        curPos += 3;

        return COS_(GetE());
    } // TODO: more functions
    else if (strncmp("tg", &expression[curPos], 2) == 0 && expression[curPos + 2] == '(')
    {
        curPos += 2;

        return TG_(GetE());
    }
    else if (strncmp("ctg", &expression[curPos], sizeof("ctg") - 1) == 0 && expression[curPos + sizeof("ctg") - 1] == '(')
    {
        curPos += 3;

        return CTG_(GetE());
    }
    else if (strncmp("ln", &expression[curPos], 2) == 0 && expression[curPos + 2] == '(') // check for '(' inside
    {
        curPos += 2;

        return LN_(GetE());
    }
    else if (isalpha(expression[curPos]))
    {
        curPos += 1;

        return VAR_(expression[curPos - 1]);
    }
    else
    {
        printf("Unrecognised ID: %c, position: %d\n", expression[curPos], curPos);
        return NULL;
    }
}

Node* GetN()
{
    char* endPtr = NULL;

    double value = strtof(&expression[curPos], &endPtr);

    curPos += endPtr - &expression[curPos];

    return NUM_(value);
}

// TODO: don't forget other functions too :P

// TODO: написать рекурсивный спуск

// TODO: поддержка числа pi и e