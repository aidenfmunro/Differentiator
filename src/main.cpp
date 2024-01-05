#include "recursivedescent.h"
#include "differentiator.h"
#include "latexdump.h"
#include "plotfunc.h"

int main(void)
{
    Tree tree = {};

    tree.root = GetG("cos(sin(ln(x)))");

    DumpTreeTex(&tree);

    

    return OK; 
}

// TODO: распечатка функции для графиков

// TODO: вычисление выражений

// TODO: ...