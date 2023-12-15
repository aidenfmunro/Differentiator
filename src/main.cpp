#include "recursivedescent.h"
#include "differentiator.h"
#include "latexdump.h"
#include "plotfunc.h"

int main(void)
{
    Tree tree = {};

    tree.root = GetG("cos(15*x+7)");

    // difftree.root = copyNode(tree.root);

   //  DumpTreeTxt(&difftree, "equation.txt");
    
    printf("%p\n", tree.root);

    DumpTreeTex(&tree);

    // printf("%p\n", tree.root);

    deleteNode(tree.root);

    // DumpPlot();

    return OK; 
}

// TODO: распечатка функции для графиков

// TODO: вычисление выражений

// TODO: ...