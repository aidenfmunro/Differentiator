#include <stdio.h>
#include "differentiator.h"

int main(void)
{
   Tree tree = {};

   ConstructTree(&tree, "expression.txt");

   DumpTreeTex(&tree);

   DumpTreeGraph(tree.root);

   DestroyTree(&tree);

   // Tree tree1 = {};

   // ConstructTree(&tree1, "expression1.txt");

   // DumpTreeTex(&tree, "ssss");

   // DestroyTree(&tree);


    return OK; 
}