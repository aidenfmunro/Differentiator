#include "differentiator.h"

#include "latexdump.h"

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

#define cN  copyNode (curNode)

#define cL  copyNode (curNode->left)
#define cR  copyNode (curNode->right)

#define dL  differentiateTree (cL, outFile)
#define dR  differentiateTree (cR, outFile)

#include "dsl.h"

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

    Node* result = NULL;

    dumpExpressionTex(curNode, NULL);

    if (curNode->right->type == CONST)
    {
        Node* diffExternalFunc = POW_(cL, NUM_(cR->data.constVal - 1));

        diffExternalFunc       = MUL_(NUM_(cR->data.constVal), diffExternalFunc);

        result                 = COMP_FUNC(diffExternalFunc, cL);
    }
    else // f^g * (g' * ln(f) + g * f' * 1/f)
    {
        Node* leftSubPart  = MUL_(dR, LN_(cL));

        Node* rightSubPart = MUL_(cR, MUL_(dL, DIV_(NUM_(1), cL)));

        result             = MUL_(cN, ADD_(leftSubPart, rightSubPart)); 
    }

    dumpDiffResExpressionTex(result, outFile);

    return result;
}

#undef cN

#undef cR
#undef cL

#undef dR
#undef dL

#define DATA curNode->data

#define R_DATA curNode->right->data
#define L_DATA curNode->left->data

#define TYPE curNode->type

#define R_TYPE curNode->right->type
#define L_TYPE curNode->left->type

#define R_ curNode->right
#define L_ curNode->left

bool OptimizeMultiplication (Tree* tree, Node* curNode)
{
    AssertSoft(tree,    NULL);
    AssertSoft(curNode, NULL);

    if (TYPE == FUNC && (DATA.func == MUL || DATA.func == DIV))
    {
        if ((R_TYPE == CONST && R_DATA.constVal == 0) && (L_TYPE == CONST && L_DATA.constVal == 0))
        {
            TYPE = CONST;
            DATA.constVal = 0;

            deleteNode(L_);
            deleteNode(R_);

            return true;
        }
    }
}

bool OptimizeAddition (Tree* tree, Node* curNode)
{
    AssertSoft(tree,    NULL);
    AssertSoft(curNode, NULL);

    if (TYPE == CONST && (DATA.func == ADD || DATA.func == SUB))
    {
        if (countMaxDepth(curNode) == 1)
        {
            TYPE = CONST;

            if (DATA.func == ADD)
            {
                DATA.constVal = L_DATA.constVal + R_DATA.constVal;
            }
            else if (DATA.func == SUB)
            {
                DATA.constVal = L_DATA.constVal - R_DATA.constVal;
            }

            return true;
        }
    }
}