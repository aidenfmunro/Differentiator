#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>
#include "tree.h"
#include "utils.h"
#include "textfuncs.h"

const int MAX_COMMAND_LENGTH  = 256;

const int MAX_FILENAME_LENGTH = 256;

struct ParseRes
{
    char       type;

    NodeElem_t data;
};

static ErrorCode _dumpTreeDot    (Node* node, FILE* outFile);

static ErrorCode _dumpTreeTxt    (Node* node, FILE* outFile);

static ErrorCode _checkTreeLinks (Tree* tree, Node* node, size_t* counter);

static ErrorCode parseToken      (char** buffer, Node* curNode);

static Node*     readTree        (Tree* tree, char** buffer, Node* parent);


ErrorCode ConstructTree          (Tree* tree, const char* baseFileName)
{
    AssertSoft(tree,         NULL);
    AssertSoft(baseFileName, NULL);

    Text base = {};

    CreateText(&base, baseFileName, NONE);

    printf("%s\n", base.buffer);

    printf("%d\n", getSize(baseFileName));

    /*
    for (size_t i  = 0; i < base.size; i++)
    {
        printf("[%d]: %s\n", i, base.tokens[i]);
    }
    */

    char* buffer = base.buffer;

    size_t curTokenNum = 0;

    tree->root = readTree(tree, &buffer, tree->root);

    DestroyText(&base);

    return OK;   
}

static Node* readTree(Tree* tree, char** buffer, Node* parent)
{   
    AssertSoft(tree,                             NULL);
    AssertSoft(buffer,                           NULL);
    
    if (**buffer != '(')
        return NULL;

    (*buffer)++;

    Node* curNode = createNode({.constVal = NAN}, CONST, NULL, NULL);

    Node* leftSubTree  = NULL;

    Node* rightSubTree = NULL;

    if (**buffer == '(')
        leftSubTree = readTree(tree, buffer, curNode);

    if (parseToken(buffer, curNode) != OK)
        return NULL;
    
    if (**buffer == '(')
        rightSubTree = readTree(tree, buffer, curNode);

    if (**buffer != ')')
        return NULL;    

    (*buffer)++;

    connectNode(curNode, leftSubTree, rightSubTree);

    tree->size++;

    return curNode;
}

Node* createFuncNode(Function funcName, Node* left, Node* right)
{
    return createNode({.func = funcName},  FUNC,   left, right);
}

Node* createConstNode(double value, Node* left, Node* right)
{
    return createNode({.constVal = value}, CONST,  left, right);
}

Node* createVarNode(char var, Node* left, Node* right)
{
    return createNode({.var = var},        VAR,    left, right);
}

ErrorCode fillFuncNode(Node* curNode, Function funcName)
{
    curNode->type      = FUNC;

    curNode->data.func = funcName;

    return OK;
}

ErrorCode fillVarNode(Node* curNode, char varName)
{
    curNode->type      = VAR;

    curNode->data.var  = varName;

    return OK;
}

ErrorCode fillConstNode(Node* curNode, double value)
{
    curNode->type          = CONST;

    curNode->data.constVal = value;

    return OK;
}

ErrorCode parseToken(char** buffer, Node* curNode)
{
    char* end = strpbrk(*buffer, "()");

    printf("%p %p\n", end, *buffer);

    int tokenLen = end - *buffer;

    printf("tokenlen %d\n", tokenLen);

    AssertSoft(tokenLen > 0, UNRECOGNISED_TOKEN);

    if      (strncmp("+", *buffer, 1) == 0) // TODO: обертку над функцией и sizeof() - 1;
    {
        fillFuncNode(curNode, ADD);
    }
    else if (strncmp("-", *buffer, 1) == 0)
    {
        fillFuncNode(curNode, SUB);
    }
    else if (strncmp("*", *buffer, 1) == 0)
    {
        fillFuncNode(curNode, MUL);
    }
    else if (strncmp("/", *buffer, 1) == 0)
    {
        fillFuncNode(curNode, DIV);
    }
    else if (strncmp("^", *buffer, 1) == 0)
    {
        fillFuncNode(curNode, POW);
    }
    else if (strncmp("sin", *buffer, 3) == 0)
    {
        fillFuncNode(curNode, SIN);
    }
    else if (strncmp("cos", *buffer, 3) == 0)
    {
        fillFuncNode(curNode, COS);
    }
    else if (strncmp("tg", *buffer, 2) == 0)
    {
        fillFuncNode(curNode, TG);
    }
    else if (strncmp("ctg", *buffer, 3) == 0)
    {
        fillFuncNode(curNode, CTG);
    }
    else if (strncmp("ln", *buffer, 2) == 0)
    {
        fillFuncNode(curNode, LN);
    }
    else if (isalpha(**buffer))
    {
        fillVarNode(curNode, **buffer);
    }
    else if (isdigit(**buffer))
    {
        fillConstNode(curNode, atof(*buffer));
    }
    else
    {
        return UNRECOGNISED_TOKEN;
    }
    
    *buffer += tokenLen;
    
    return OK;
}

ErrorCode deleteNode(Node* node)
{
    AssertSoft(node, NULL_PTR);

    if (node->left  != NULL)
        deleteNode(node->left);
    
    if (node->right != NULL)
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

Node* createNode(NodeElem_t data, char type, Node* left, Node* right)  
{
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
        newNode->data.constVal = data.constVal;
    }
    else if (type == FUNC)
    {
        newNode->data.func  = data.func;
    }
    else if (type == VAR)
    {
        newNode->data.var   = data.var; 
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
            newNode->data.constVal = originalNode->data.constVal;
            break;
        }

        case FUNC:
        {
            newNode->data.func     = originalNode->data.func;
            break;
        }    

        case VAR:
        {
            newNode->data.var      = originalNode->data.var;
            break;
        }

        default:
        {
            free(newNode);

            return NULL;   // TODO: return error message
        }
    }

    if (originalNode->left)
        newNode->left  = copyNode(originalNode->left);
    
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
        dumpText("nil ");

        return OK;
    }

    _dumpTreeTxt(node->left, outFile);

    dumpText("("SPECIFIER" ", node->data); 

    _dumpTreeTxt(node->right, outFile);

    dumpText(") ");

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
    AssertSoft(outFile, UNABLE_TO_OPEN_FILE);
    AssertSoft(node,    NULL_PTR);

    if (!node)
        return OK;

    static size_t nodeNum = 0;

    size_t curNodeNum = nodeNum;
    
    if (node->type == FUNC)
    {
        dumpGraph("\"node%zu\" [shape = \"record\", label = \"{%s}\", fillcolor = \"coral\"]\n",
                                                          nodeNum, getFuncName(node->data.func));
    }

    if (node->type == CONST)
    {
        dumpGraph("\"node%zu\" [shape = \"record\", label = \"{%lg}\", fillcolor = \"aqua\"]\n",
                                                                   nodeNum, node->data.constVal);
    }

    if (node->type == VAR)
    {
        dumpGraph("\"node%zu\" [shape = \"record\", label = \"{%c}\", fillcolor = \"aquamarine\"]\n",
                                                                             nodeNum, node->data.var);
    }

    nodeNum++;

    if (node->left)
    {
        size_t childNum = nodeNum;

        _dumpTreeDot(node->left, outFile);

        dumpGraph("\"node%zu\" -> \"node%zu\"\n",
                            curNodeNum, childNum);
    }

    if (node->right)
    {
        size_t childNum = nodeNum;

        _dumpTreeDot(node->right, outFile);

        dumpGraph("\"node%zu\" -> \"node%zu\"\n",
                            curNodeNum, childNum);
    }
    
    
    return OK;
}

char* getFuncName(Function func)
{
    switch (func)
    {
        case ADD:
            return "+";
        
        case SUB:
            return "-";

        case MUL:
            return "*";
        
        case DIV:
            return "/";
        
        case SIN:
            return "sin";
        
        case COS:
            return "cos";
        
        case TG:
            return "tg";
        
        case CTG:
            return "ctg";

        case ARCCTG:
            return "arcctg";
        
        case ARCTG:
            return "arctg";
            
        case ARCCOS:
            return "arccos";

        case ARCSIN:
            return "arcsin";

        case SH:
            return "sh";

        case CH:
            return "ch";

        case TH:
            return "th";

        case CTH:
            return "cth";    
        
        case LN:
            return "ln";
        
        case POW:
            return "^";

        default:
            return "UNKNOWN";
    }
}

#undef dumpGraph
