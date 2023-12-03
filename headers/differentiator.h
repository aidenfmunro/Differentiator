#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <math.h>
#include <stdio.h>

#include "tree.h"
#include "utils.h"

Node* differentiateTree(Node* curNode, FILE* outFile);

#endif