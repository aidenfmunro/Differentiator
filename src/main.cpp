#include "recursivedescent.h"
#include "differentiator.h"
#include "latexdump.h"

int main(void)
{
    Tree tree = {};

    tree.root = GetG("x*sin(x)");

    Tree difftree = {};

    difftree.root = copyNode(tree.root);

    DumpTreeTex(&difftree);

    // printf("%p\n", tree.root);

    deleteNode(tree.root);

    return OK; 
}