#include <stdio.h>
#include "differentiator.h"

int main(void)
{
   Tree tree = {};

   ConstructTree(&tree, "expression.txt");

   DumpTreeTex(&tree, "equation.tex");

   DestroyTree(&tree);


    return OK; 
}