#include <stdio.h>
#include "tree.h"

int main(void)
{
    Tree tree = {};

    ConstructTree(&tree, "expression.txt");

    DumpTreeTxt(&tree, "out.txt");

    printf("%p\n", tree.root);

    DumpTreeGraph(tree.root);

    DestroyTree(&tree);

    return OK; 
}