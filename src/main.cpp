#include <stdio.h>
#include "differentiator.h"

int main(void)
{
    Tree tree = {};

    ConstructTree(&tree, "expression.txt");

    DumpTreeTxt(&tree, "out.txt");

    printf("%p\n", tree.root);

    myOpen("equation.tex", "w", outFile);

    fprintf(outFile, BeginTexDocument);

    DumpTreeGraph(tree.root);

    tree.root = differentiateTree(tree.root, outFile);

    fprintf(outFile, EndTexDocument);

    DumpTreeGraph(tree.root);

    myClose(outFile);

    DestroyTree(&tree);

    return OK; 
}