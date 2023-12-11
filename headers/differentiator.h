#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "textfuncs.h"
#include "utils.h"
#include "tree.h"

Node* differentiateTree(Node* curNode, FILE* outFile);

size_t Optimise(Node* curNode);



#endif