#include <stdio.h>
#include "differentiator.h"
#include "utils.h"

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

#define NUM_(value)        createConstNode(value, NULL, NULL)

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
    return COMP_FUNC(DIV_(NUM_(1), POW_(NUM_(2), COS_(cR))), cR);
}

static Node* diffCtg(Node* curNode)
{

}

static Node* diffLn(Node* curNode)
{

}

static Node* diffPow(Node* curNode)
{

}

// TODO: don't forget other functions too :P

// TODO: написать рекурсивный спуск