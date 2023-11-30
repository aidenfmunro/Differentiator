#include <stdio.h>
#include "tree.h"
#include "utils.h"

#define cL copyNode(curNode->left)
#define cR copyNode(curNode->right)

#define dL differentiateTree(curNode->left)
#define dR differentiateTree(curNode->right)


Node* differentiateTree(Node* curNode)
{
    AssertSoft(curNode, NULL);

    switch (curNode->type)
    {
        case CONST:

        case VAR:

        case FUNC:
        {
            switch (curNode->data.func)
            {
                case ADD:

                case SUB:

                case MUL:

                case DIV:

                case SIN:

                case COS:

                case TG:

                case LN:

                case POW:

                default:
            }
        }

        default:
    }
}

Node* diffAdd(Node* curNode)
{

}

Node* diffSub(Node* curNode)
{

}

Node* diffMul(Node* curNode)
{

}

Node* diffDiv(Node* curNode)
{

}

Node* diffSin(Node* curNode)
{

}

Node* diffCos(Node* curNode)
{

}

Node* diffTg(Node* curNode)
{

}

Node* diffCtg(Node* curNode)
{

}

Node* diffLn(Node* curNode)
{

}

Node* diffPow(Node* curNode)
{

}

// TODO: don't forget other functions too :P