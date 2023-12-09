#include "recursivedescent.h"
#include "differentiator.h"
#include "latexdump.h"

int main(void)
{
    Tree tree = {};

    /*

    ConstructTree(&tree, "expression.txt");

    DumpTreeTex(&tree);

    DumpTreeGraph(tree.root);

    */

    tree.root = GetG();

    DumpTreeGraph(tree.root);

    DumpTreeTex(&tree);

    DumpTreeGraph(tree.root);

    // DumpTreeTex(&tree);

    // DestroyTree(&tree);

    // Tree tree1 = {};

    // ConstructTree(&tree1, "expression1.txt");

    // DumpTreeTex(&tree, "ssss");

    // DestroyTree(&tree);

    return OK; 
}