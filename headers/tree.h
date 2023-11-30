#ifndef TREE_H
#define TREE_H

#include "errors.h"

#include <cstddef>

#define SPECIFIER "%c"

enum Function
{
    ADD,
    MUL,
    SUB,
    DIV,
    SIN,
    COS,
    ARCCOS,
    ARCSIN,
    TG,
    CTG,
    ARCTG,
    ARCCTG,
    SH,
    CH,
    TH,
    CTH,
    LN,
    POW
};

enum Type
{
    CONST,
    FUNC,
    VAR
};

union NodeElem_t
{
    double     constVal;

    char       var;

    Function   func;
};

struct Node
{
    NodeElem_t data = {};

    char type        = CONST;

    Node*      parent;

    Node*      left;

    Node*      right;
};

struct Tree
{
    Node*      root;
    
    size_t     size;  

    ErrorCode  error;  
};  

ErrorCode ConstructTree     (Tree* tree, const char* baseFileName);

ErrorCode DestroyTree       (Tree* tree);

ErrorCode DumpTreeGraph     (Node* node);

ErrorCode DumpTreeTxt       (Tree* tree, const char* filename);

ErrorCode VerifyTree        (Tree* tree);

ErrorCode deleteNode        (Node* node);

ErrorCode connectNode       (Node* node, Node* leftChild, Node* rightChild);

Node*     copyNode(Node* originalNode);

Node*     createNode        (NodeElem_t data, char type, Node* left, Node* right);

Node*     createVarNode     (char var, Node* left, Node* right);

Node*     createConstNode   (double value, Node* left, Node* right);

Node*     createFuncNode    (Function funcName, Node* left, Node* right);

char*     getFuncName       (Function func);

#endif