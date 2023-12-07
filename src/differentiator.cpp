#include "differentiator.h"

static Node* diffAdd (Node* curNode, FILE* outFile);

static Node* diffSub (Node* curNode, FILE* outFile);

static Node* diffMul (Node* curNode, FILE* outFile);

static Node* diffDiv (Node* curNode, FILE* outFile);

static Node* diffSin (Node* curNode, FILE* outFile);

static Node* diffCos (Node* curNode, FILE* outFile);

static Node* diffTg  (Node* curNode, FILE* outFile);

static Node* diffCtg (Node* curNode, FILE* outFile);

static Node* diffLn  (Node* curNode, FILE* outFile);

static Node* diffPow (Node* curNode, FILE* outFile);

static ErrorCode _dumpTreeTex(Node* node, FILE* outFile);

ErrorCode beginExpressionTex(FILE* outFile);

ErrorCode endExpressionTex(FILE* outFile);

ErrorCode dumpExpressionTex(Node* node, FILE* outFile);

ErrorCode dumpDiffExpressionTex(Node* node, FILE* outFile);

ErrorCode dumpDiffResExpressionTex(Node* node, FILE* outFile);

#define cN  copyNode (curNode)

#define cL  copyNode (curNode->left)
#define cR  copyNode (curNode->right)

#define dL  differentiateTree (cL, outFile)
#define dR  differentiateTree (cR, outFile)

#define ADD_(left, right)  createFuncNode (ADD, left, right)
#define MUL_(left, right)  createFuncNode (MUL, left, right)
#define SUB_(left, right)  createFuncNode (SUB, left, right)
#define DIV_(left, right)  createFuncNode (DIV, left, right)
#define POW_(left, right)  createFuncNode (POW, left, right)

#define SIN_(value)        createFuncNode (SIN, NULL, value)
#define COS_(value)        createFuncNode (COS, NULL, value)
#define CTG_(value)        createFuncNode (CTG, NULL, value)
#define  TG_(value)        createFuncNode (TG,  NULL, value)
#define  LN_(value)        createFuncNode (LN,  NULL, value)

#define NUM_(value)        createConstNode (value, NULL, NULL)

#define VAR_(value)        createVarNode(value, NULL, NULL)

#define COMP_FUNC(externalFunc, internalFunc)  MUL_(externalFunc, differentiateTree(internalFunc, outFile))  

Node* differentiateTree(Node* curNode, FILE* outFile)
{
    AssertSoft(curNode, NULL);

    switch (curNode->type)
    {
        case CONST:
            return createConstNode (0, NULL, NULL);

        case VAR:
            return createConstNode (1, NULL, NULL);

        case FUNC:
        {
            switch (curNode->data.func) 
            {
                case ADD:
                    return diffAdd(curNode, outFile);

                case SUB:
                    return diffSub(curNode, outFile);

                case MUL:
                    return diffMul(curNode, outFile);

                case DIV:
                    return diffDiv(curNode, outFile);

                case SIN:
                    return diffSin(curNode, outFile);

                case COS:
                    return diffCos(curNode, outFile);

                case TG:
                    return diffTg(curNode, outFile);

                case CTG:
                    return diffCtg(curNode, outFile);

                case LN:
                    return diffLn(curNode, outFile);

                case POW:
                    return diffPow(curNode, outFile);

                default:
                    printf("Unknown function: %d!\n", curNode->data.func);
                    return NULL;
            }
        }

        default:
            printf("Unknown type: %d!\n", curNode->type);
            return NULL;
    }
}

static Node* diffAdd(Node* curNode, FILE* outFile)
{
    AssertSoft(curNode, NULL);

    return  ADD_(dL, dR);
}

static Node* diffSub(Node* curNode, FILE* outFile)
{
    AssertSoft(curNode, NULL);

    dumpExpressionTex(curNode, outFile);

    return SUB_(dL, dR);
}

static Node* diffMul(Node* curNode, FILE* outFile)
{
    AssertSoft(curNode, NULL);

    dumpExpressionTex(curNode, NULL);

    Node* result = ADD_(MUL_(dL, cR), MUL_(cL, dR));

    return result;
}

static Node* diffDiv(Node* curNode, FILE* outFile)
{
    AssertSoft(curNode, NULL);

    Node* result = DIV_(SUB_(MUL_(dL, cR), MUL_(cL, dR)), POW_(cR, NUM_(2)));

    dumpDiffResExpressionTex(curNode, outFile);

    return result;
}

static Node* diffSin(Node* curNode, FILE* outFile)
{
    AssertSoft(curNode, NULL);

    dumpDiffExpressionTex(curNode, outFile);

    Node* result = COMP_FUNC(COS_(cR), cR);

    dumpDiffResExpressionTex(result, outFile);

    return result;
}

static Node* diffCos(Node* curNode, FILE* outFile)
{
    AssertSoft(curNode, NULL);

    dumpDiffExpressionTex(curNode, outFile);

    Node* result = COMP_FUNC(MUL_(NUM_(-1), SIN_(cR)), cR);

    dumpDiffResExpressionTex(result, outFile);

    return result;
}

static Node* diffTg(Node* curNode, FILE* outFile)
{
    AssertSoft(curNode, NULL);

    dumpDiffExpressionTex(curNode, outFile);

    // Node* externalFunc = ... <--- dump this

    Node* result = COMP_FUNC(DIV_(NUM_(1),  POW_(COS_(cR), NUM_(2))), cR);

    dumpDiffResExpressionTex(result, outFile);

    return result;
}

static Node* diffCtg(Node* curNode, FILE* outFile)
{
    AssertSoft(curNode, NULL);

    dumpDiffExpressionTex(curNode, outFile);

    Node* result = COMP_FUNC(DIV_(NUM_(-1), POW_(SIN_(cR), NUM_(2))), cR);

    dumpDiffResExpressionTex(result, outFile);

    return result;
}

static Node* diffLn(Node* curNode, FILE* outFile)
{
    AssertSoft(curNode, NULL);

    dumpDiffExpressionTex(curNode, outFile);

    Node* result = COMP_FUNC(DIV_(NUM_(1), cR), cR);

    dumpDiffResExpressionTex(result, outFile);

    return result;
}

static Node* diffPow(Node* curNode, FILE* outFile)
{
    AssertSoft(curNode, NULL);

    dumpExpressionTex(curNode, NULL);

    // return COMP_FUNC(MUL_(cN, ADD_(MUL_())));
}

#define dumpTex(...) fprintf(outFile, __VA_ARGS__);

const int MAX_FILENAME_LENGTH = 256;

const int MAX_COMMAND_LENGTH  = 256;

ErrorCode DumpTreeTex(Tree* tree)
{
    static int TEX_DUMP_NUM = 0;

    char filename[MAX_FILENAME_LENGTH] = {};

    sprintf(filename, "result_%d", TEX_DUMP_NUM);

    char command0[MAX_COMMAND_LENGTH] = {};

    sprintf(command0, "log/tex/%s.tex", filename);

    myOpen(command0, "w", outFile);

    dumpTex(BeginTexDocument);

    dumpExpressionTex(tree->root, outFile);

    tree->root = differentiateTree(tree->root, outFile);

    dumpTex(EndTexDocument);

    myClose(outFile);

    char command1[MAX_COMMAND_LENGTH] = {};

    sprintf(command1, "pdflatex --output-directory=log/pdf log/tex/%s.tex", filename);

    system(command1);

    char command2[MAX_COMMAND_LENGTH] = {};

    sprintf(command2, "open log/pdf/%s.pdf", filename);

    system(command2);

    DestroyTree(tree);

    return OK;
}

#define DUMP_L_TREE(node) _dumpTreeTex(node->left,  outFile)
#define DUMP_R_TREE(node) _dumpTreeTex(node->right, outFile)

static ErrorCode _dumpTreeTex(Node* node, FILE* outFile)
{
    AssertSoft(node, NULL_PTR);

    switch (node->type)
    {
        case CONST:
        {
            dumpTex("%lg", node->data.constVal);
            break;
        }   
        case VAR:
        {
            dumpTex("%c", node->data.var);
            break;
        }
        case FUNC:
        {
            switch (node->data.func)
            {
            case ADD:
            {
                dumpTex("(");

                DUMP_L_TREE(node);

                dumpTex(" + ");

                DUMP_R_TREE(node);

                dumpTex(")");

                break;
            }

            case SUB:
            {
                dumpTex("(");

                DUMP_L_TREE(node);

                dumpTex(" - ");

                DUMP_R_TREE(node);

                dumpTex(")");

                break;
            }

            case MUL:
            {
                DUMP_L_TREE(node);

                dumpTex(" \\cdot ");

                DUMP_R_TREE(node);

                break;
            }
            case DIV:
            {
                dumpTex(" \\frac{");

                DUMP_L_TREE(node);

                dumpTex(" }{ ");

                DUMP_R_TREE(node);

                dumpTex(" } ");

                break;
            }
            case SIN:
            {
               dumpTex("\\sin");

               dumpTex("(");

               DUMP_R_TREE(node);

               dumpTex(")")

               break;
            }
            case COS:
            {
                dumpTex("\\cos");

                dumpTex("(");

                DUMP_R_TREE(node);

                dumpTex(")");

                break;
            }
            case TG:
            {
                dumpTex("\\tan");

                dumpTex("(");

                DUMP_R_TREE(node);

                dumpTex(")");

                break;
            }
            case CTG:
            {
                dumpTex("\\cot");

                dumpTex("(");

                DUMP_R_TREE(node);

                dumpTex(")");

                break;
            }

            case LN:
            {
                dumpTex("\\ln");

                dumpTex("(");

                DUMP_R_TREE(node);

                dumpTex(")");

                break;
            }
            case POW:
            {
                dumpTex("(");

                DUMP_L_TREE(node);

                dumpTex("^{");

                DUMP_R_TREE(node);

                dumpTex("}");

                break;
            }

            default:
            {
                printf("Unknown function: %d!\n", node->data.func);
                return NULL;
            }
            }

            break;
        }
        default:
        {
            printf("Unknown type: %d!\n", node->type);
            return NULL;
        }
        
    }

    return OK;
}

#undef DUMP_L_TREE
#undef DUMP_R_TREE

ErrorCode beginExpressionTex(FILE* outFile)
{
    AssertSoft(outFile, UNABLE_TO_OPEN_FILE);

    dumpTex("\n\n"
            "\\begin{center}\n"
            "\\begin{equation}\n");

    return OK;
}

ErrorCode endExpressionTex(FILE* outFile)
{
    AssertSoft(outFile, UNABLE_TO_OPEN_FILE);

    dumpTex("\n\\end{equation}\n"
            "\\end{center}"
            "\n\n");

    return OK;
}

ErrorCode dumpExpressionTex(Node* node, FILE* outFile)
{
    AssertSoft(node, NULL_PTR);
    AssertSoft(outFile, UNABLE_TO_OPEN_FILE);

    beginExpressionTex(outFile);

    _dumpTreeTex(node, outFile);

    endExpressionTex(outFile);

    return OK;
}

ErrorCode dumpDiffResExpressionTex(Node* node, FILE* outFile)
{
    AssertSoft(node, NULL_PTR);
    AssertSoft(outFile, UNABLE_TO_OPEN_FILE);

    beginExpressionTex(outFile);

    _dumpTreeTex(node, outFile);

    endExpressionTex(outFile);

    return OK;   
}

ErrorCode dumpDiffExpressionTex(Node* node, FILE* outFile)
{
    AssertSoft(node, NULL_PTR);
    AssertSoft(outFile, UNABLE_TO_OPEN_FILE);

    beginExpressionTex(outFile);

    dumpTex("(");

    _dumpTreeTex(node, outFile);

    dumpTex(")'");

    endExpressionTex(outFile);

    return OK;
}

Node* GetE();

Node* GetT();

Node* GetP();

Node* GetId();

Node* GetN();

const char* expression = "x+(x*x)-8+2";

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

        // assert for ')'

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
    else /* (isalpha(expression[curPos])) */
    {
        curPos += 1;

        return VAR_(expression[curPos - 1]);
    }
}

Node* GetN()
{
    char* endPtr = NULL;

    double value = strtof(&expression[curPos], &endPtr);

    curPos += endPtr - &expression[curPos];

    return NUM_(value);
}

// TODO: pow function diff

// TODO: don't forget other functions too :P

// TODO: написать рекурсивный спуск

// TODO: поддержка числа pi и e