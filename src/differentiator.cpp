

#include "differentiator.h"


static Node* diffAdd (Node* curNode);

static Node* diffSub (Node* curNode);

static Node* diffMul (Node* curNode);

static Node* diffDiv (Node* curNode);

static Node* diffSin (Node* curNode);

static Node* diffCos (Node* curNode);

static Node* diffTg  (Node* curNode);

static Node* diffCtg (Node* curNode);

static Node* diffLn  (Node* curNode);

static Node* diffPow (Node* curNode);

#define cN  copyNode (curNode)

#define cL  copyNode (curNode->left)
#define cR  copyNode (curNode->right)

#define dL  differentiateTree (cL)
#define dR  differentiateTree (cR)

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

#define COMP_FUNC(externalFunc, internalFunc)  MUL_(externalFunc, differentiateTree(internalFunc))  

Node* differentiateTree(Node* curNode)
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
                    return diffAdd(curNode);

                case SUB:
                    return diffSub(curNode);

                case MUL:
                    return diffMul(curNode);

                case DIV:
                    return diffDiv(curNode);

                case SIN:
                    return diffSin(curNode);

                case COS:
                    return diffCos(curNode);

                case TG:
                    return diffTg(curNode);

                case CTG:
                    return diffCtg(curNode);

                case LN:
                    return diffLn(curNode);

                case POW:
                    return diffPow(curNode);

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

static Node* diffAdd(Node* curNode)
{
    return ADD_(dL, dR);
}

static Node* diffSub(Node* curNode)
{
    return SUB_(dL, dR);
}

static Node* diffMul(Node* curNode)
{
    return ADD_(MUL_(dL, cR), MUL_(cL, dR));
}

static Node* diffDiv(Node* curNode)
{
    return DIV_(SUB_(MUL_(dL, cR), MUL_(cL, dR)), POW_(cR, NUM_(2)));
}

static Node* diffSin(Node* curNode)
{
    return COMP_FUNC(COS_(cR), cR);
}

static Node* diffCos(Node* curNode)
{
    return COMP_FUNC(MUL_(NUM_(-1), SIN_(cR)), cR);
}

static Node* diffTg(Node* curNode)
{
    return COMP_FUNC(DIV_(NUM_(1),  POW_(COS_(cR), NUM_(2))), cR);
}

static Node* diffCtg(Node* curNode)
{
    return COMP_FUNC(DIV_(NUM_(-1), POW_(SIN_(cR), NUM_(2))), cR);
}

static Node* diffLn(Node* curNode)
{
    return COMP_FUNC(DIV_(NUM_(1), cR), cR);
}

static Node* diffPow(Node* curNode)
{
    // return COMP_FUNC(MUL_(cN, ADD_(MUL_())));
}

#define dumpTex(...) fprintf(outFile, __VA_ARGS__);

ErrorCode DumpTreeTex(Tree* tree, const char* filename)
{
    myOpen(filename, "w", outFile);

    

    myClose(outFile);
}

#define DUMP_L_TREE(node) _dumpTreeTex(node->left,  outFile)
#define DUMP_R_TREE(node) _dumpTreeTex(node->right, outFile)


ErrorCode _dumpTreeTex(Node* node, FILE* outFile)
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
                dumpTex("(");

                DUMP_L_TREE(node);

                dumpTex(" + ");

                DUMP_R_TREE(node);

                dumpTex(")");

                break;
            }
            case DIV:
            {
                dumpTex("(");

                dumpTex("\\frac{");

                DUMP_L_TREE(node);

                dumpTex("}{");

                DUMP_R_TREE(node);

                dumpTex("}");

                dumpTex(")");

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
        
        default:
        {
            printf("Unknown type: %d!\n", node->type);
            return NULL;
        }
        }
    }
}










// TODO: don't forget other functions too :P

// TODO: написать рекурсивный спуск

// TODO: поддержка числа pi и e