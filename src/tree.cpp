#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "tree.h"
#include "utils.h"
#include "textfuncs.h"

const int MAX_COMMAND_LENGTH  = 256;

const int MAX_FILENAME_LENGTH = 256;

static ErrorCode _dumpTreeDot    (Node* node, FILE* outFile);

static ErrorCode _dumpTreeTxt    (Node* node, FILE* outFile);

static ErrorCode _checkTreeLinks (Tree* tree, Node* node, size_t* counter);

static Node*     readTree        (Tree* tree, Text* base, size_t* curTokenNum);

static Node*     readNode        (Tree* tree, Text* base, size_t* curTokenNum);

ErrorCode ConstructTree(Tree* tree, const char* baseFileName)
{
    AssertSoft(tree,         NULL);
    AssertSoft(baseFileName, NULL);

    Text base = {};

    CreateText(&base, baseFileName, NONE);

    size_t curTokenNum = 0;

    tree->root = readTree(tree, &base, &curTokenNum);

    DestroyText(&base);

    return OK;   
}

static Node* readTree(Tree* tree, Text* base, size_t* curTokenNum)
{   
    AssertSoft(tree,                           NULL);
    AssertSoft(base,                           NULL);
    AssertSoft(*curTokenNum < base->numTokens, NULL);

    const Token* token = &base->tokens[(*curTokenNum)++];

    const char* openBracket = strchr(token->string, '(');

    if (openBracket)
        return readNode(tree, base, curTokenNum);

    if (strcmp(token->string, "nil") == 0)
        return NULL;
    
    tree->error = SYNTAX_ERROR;

    return NULL;
}

static Node* readNode(Tree* tree, Text* base, size_t* curTokenNum)
{
    AssertSoft(tree,                           NULL);
    AssertSoft(base,                           NULL);
    AssertSoft(*curTokenNum < base->numTokens, NULL);

    const Token* token = &base->tokens[(*curTokenNum)++];

    if (StringIsEmpty(token))
    {
        tree->error = UNRECOGNISED_TOKEN;
        AssertSoft(! tree->error, NULL);
    }

    Node* leftSubTree  = readTree(tree, base, curTokenNum);

    Node* rightSubTree = readTree(tree, base, curTokenNum); 

    Node* newNode      = createNode(data, leftSubTree, rightSubTree); // change this i guess

    tree->size++; 

    token = &base->tokens[(*curTokenNum)++]; // DSL

    const char* closeBracket = strchr(token->string, ')');

    if (!closeBracket)
    {
        tree->error = SYNTAX_ERROR;

        return NULL;
    }

    return newNode;
}

ErrorCode deleteNode(Node* node)
{
    AssertSoft(node, NULL_PTR);

    if (node->left)
        deleteNode(node->left);
    
    if (node->right)
        deleteNode(node->right);

    node->right  = NULL;
    node->left   = NULL;
    node->parent = NULL;

    free(node); 

    return OK;
}

ErrorCode DestroyTree(Tree* tree)
{
    AssertSoft(tree, NULL_PTR);

    deleteNode(tree->root);

    tree->root = NULL; // ?

    tree->error = 0;

    tree->size = 0;

    return OK;
}

static ErrorCode _checkTreeLinks(Tree* tree, Node* node, size_t* counter)
{
    AssertSoft(tree, NULL_PTR);
    
    // TODO: counter check

    (*counter)++; // TODO: make it in 

    if ((node->left != NULL && node->left->parent != node) || (node->right != NULL && node->right->parent != node))
    {
        tree->error = TREE_LINKING_ERROR;
        return TREE_LINKING_ERROR;
    }

    if (node->left != NULL)
        _checkTreeLinks(tree, node->left, counter);
    
    if (node->right != NULL)
        _checkTreeLinks(tree, node->right, counter);

    return OK;
}

Node* createNode(NodeElem_t* data, char type, Node* left, Node* right) // TODO: put in tree.cpp
{
    AssertSoft(data, NULL);

    SafeCalloc(newNode, 1, Node, NULL);

    if (left)
        left->parent  = newNode; 
        
    if (right)
        right->parent = newNode;
    
    newNode->left  = left;
    
    newNode->right = right;

    newNode->type = type;

    if      (type == CONST)
    {
        newNode->data.value = data->value;
    }
    else if (type == FUNC)
    {
        newNode->data.func  = data->func;
    }
    else if (type == VAR)
    {
        newNode->data.var   = data->var; 
    }
    else
    {
        return NULL; // TODO: return error message
    }


    return newNode;    
}

Node* copyNode(Node* originalNode)
{
    AssertSoft(originalNode, NULL);

    SafeCalloc(newNode, 1, Node, NULL);

    newNode->type = originalNode->type;

    switch (originalNode->type)
    {
        case CONST:
        {
            newNode->data.value = originalNode->data.value;
            break;
        }

        case FUNC:
        {
            newNode->data.func = originalNode->data.func;
            break;
        }    

        case VAR:
        {
            newNode->data.var = originalNode->data.var;
            break;
        }

        default:
        {
            free(newNode);
            return NULL; // TODO: return error message
        }
    }

    if (originalNode->left)
        newNode->left = copyNode(originalNode->left);
    
    if (originalNode->right)
        newNode->right = copyNode(originalNode->right);

    return newNode;
}

ErrorCode connectNode(Node* node, Node* leftChild, Node* rightChild)
{
    AssertSoft(node, NULL_PTR);

    node->left = leftChild;

    if (leftChild)
        leftChild->parent = node;

    node->right = rightChild;
    
    if (rightChild)
        rightChild->parent = node;

    return OK;
}

ErrorCode VerifyTree(Tree* tree) // TODO: make proper verify 
{
    size_t count = 0;
    _checkTreeLinks(tree, tree->root, &count);

    return OK;
} 

ErrorCode DumpTreeTxt(Tree* tree, const char* filename)
{
    myOpen(filename, "w+", outFile);

    _dumpTreeTxt(tree->root, outFile);

    myClose(outFile);

    return OK;
}

#define dumpText(...) fprintf(outFile, __VA_ARGS__);

static ErrorCode _dumpTreeTxt(Node* node, FILE* outFile) // TODO: create tree with txt file
{
    AssertSoft(outFile, UNABLE_TO_OPEN_FILE);

    if (node == NULL)
    {
        dumpText("nil\n");

        return OK;
    }

    dumpText("(\n"SPECIFIER"\n", node->data); 

    _dumpTreeTxt(node->left, outFile);

    _dumpTreeTxt(node->right, outFile);

    dumpText(")\n");

    return OK;
}

#undef dumpText

#include "graph.h"

#define dumpGraph(...) fprintf(outFile, __VA_ARGS__);

ErrorCode DumpTreeGraph(Node* node)
{
    AssertSoft(node, NULL_PTR);

    static int DUMP_NUM = 0;

    char filename[MAX_FILENAME_LENGTH] = "";

    sprintf(filename, "log/dot/treegraph_%d.dot", DUMP_NUM); // NOTE: check for existence of folder

    myOpen(filename, "w", outFile);

    dumpGraph("  digraph tree"
              "  {\n"
              "  node[ "
              "  shape     = "SHAPE"," 
              "  style     = "STYLE"," 
              "  fillcolor = "BACKGROUND_PARENT_COLOR"," 
              "  fontcolor = "FONT_COLOR","
              "  margin    = "SPACE_BETWEEN_CONTENTS"];\n"
              "  rankdir   = "RANK";\n\n"
              "  label     = "DUMP_NAME";\n");

    _dumpTreeDot(node, outFile);

    dumpGraph("  }");

    myClose(outFile);

    char command[MAX_COMMAND_LENGTH] = "";

    sprintf(command, "dot -Tpng log/dot/treegraph_%d.dot -o log/img/treeimg_%d.png", DUMP_NUM, DUMP_NUM);

    system(command);

    DUMP_NUM++;

    return OK;
}

static ErrorCode _dumpTreeDot(Node* node, FILE* outFile) // TODO: wtf make it more clean, // create_node, connect_node
{
    AssertSoft(node, NULL_PTR);

    if (node->left == NULL && node->right == NULL)
    {
        dumpGraph(" \"" SPECIFIER "\" [shape = "SHAPE", fillcolor = "BACKGROUND_CHILD_COLOR"," 
                  " label = \"{<name>" SPECIFIER " | parent\\n%p | <f0> address\\n%p|"
                  " {<left>left\\n%p | <right>right\\n%p\\n}}\"];\n", 
                  node->data,
                  node->data, node->parent, node,
                  node->left, node->right); // TODO: create_node and connect_node macro/func!
    }    
    else
    {
        dumpGraph(" \"" SPECIFIER "\" [shape = "SHAPE"," 
                  " label = \"{<name>" SPECIFIER " | parent\\n%p | <f0> address\\n%p|"
                  " {<left>left\\n%p | <right>right\\n%p\\n}}\"];\n",
                  node->data, 
                  node->data, node->parent, node, 
                  node->left, node->right);
    }
    
    if (node->left != NULL)
    {
        if (node->left->parent == node)
        {
            dumpGraph("  \"" SPECIFIER "\":left->\"" SPECIFIER "\";\n", node->data, node->left->data);
            // fprintf(outFile, "  \"" SPECIFIER "\":name->\"" SPECIFIER "\";\n", node->left->data, node->data);
        }
        else
        {
            dumpGraph("  edge [color=\"#FE6200\"];\n"
                      "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n"
                      "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n"
                      "  edge [color=\"#000000\"];\n",
                      node->data, node->left->data,
                      node->left->data, node->data);
        }

        _dumpTreeDot(node->left, outFile); // TODO: get rid off copy paste bruh
    }

    if (node->right != NULL)
    {
        if (node->right->parent == node)
        {
            dumpGraph("  \"" SPECIFIER "\":right->\"" SPECIFIER "\";\n", node->data, node->right->data);
           //  fprintf(outFile, "  \"" SPECIFIER "\":name->\"" SPECIFIER "\";\n", node->right->data, node->data);
        }
        else
        {
            dumpGraph("  edge [color=\"#FE6200\"];\n"
                      "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n" 
                      "  \"" SPECIFIER "\"->\"" SPECIFIER "\";\n",
                      "  edge [color=\"#000000\"];\n", 
                      node->data, node->right->data, node->right->data, node->data);
        }

        _dumpTreeDot(node->right, outFile);
    }
    
    return OK;
}

#undef dumpGraph
