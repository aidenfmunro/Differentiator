#include "recursivedescent.h"
#include "differentiator.h"
#include "latexdump.h"
#include "plotfunc.h"

int main(void)
{
    Tree tree = {};

    tree.root = GetG("cos(x^(x+2*x+1))");

    Tree difftree = {};

    difftree.root = copyNode(tree.root);

    DumpTreeTxt(&difftree, "equation.txt");

    DumpTreeTex(&difftree);

    deleteNode(tree.root);

    DumpPlot();

    return OK; 
}

// TODO: распечатка функции для графиков

// TODO: вычисление выражений

// TODO: ...