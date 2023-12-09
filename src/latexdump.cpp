#include "latexdump.h"

static ErrorCode _dumpTreeTex(Node* node, FILE* outFile);

#define dumpTex(...) fprintf(outFile, __VA_ARGS__);

const int MAX_FILENAME_LENGTH = 256;

const int MAX_COMMAND_LENGTH  = 256;

ErrorCode DumpTreeTex(Tree* tree)
{
    static int TEX_DUMP_NUM = 0;

    char filename[MAX_FILENAME_LENGTH] = {};

    sprintf(filename, "result_%d", TEX_DUMP_NUM);


    char command0[MAX_COMMAND_LENGTH] = {};

    sprintf(command0, "log/tex/%s.tex", filename);

    myOpen(command0, "w", outFile);


    dumpTex(BeginTexDocument);

    dumpExpressionTex(tree->root, outFile);

    tree->root = differentiateTree(tree->root, outFile);

    dumpExpressionTex(tree->root, outFile);

    dumpTex(EndTexDocument);


    myClose(outFile);


    char command1[MAX_COMMAND_LENGTH] = {};

    sprintf(command1, "pdflatex --output-directory=log/pdf log/tex/%s.tex", filename);

    system(command1);


    char command2[MAX_COMMAND_LENGTH] = {};

    sprintf(command2, "open log/pdf/%s.pdf", filename);

    system(command2);

    TEX_DUMP_NUM++;

    return OK;
}

#define DUMP_L_TREE(node) _dumpTreeTex(node->left,  outFile)
#define DUMP_R_TREE(node) _dumpTreeTex(node->right, outFile)

static ErrorCode _dumpTreeTex(Node* node, FILE* outFile)
{
    AssertSoft(node, NULL_PTR);

    switch (node->type)
    {
        case CONST:
        {
            dumpTex("%lg", node->data.constVal);
            break;
        }   
        case VAR:
        {
            dumpTex("%c", node->data.var);
            break;
        }
        case FUNC:
        {
            switch (node->data.func)
            {
            case ADD:
            {
                dumpTex("(");

                DUMP_L_TREE(node);

                dumpTex(" + ");

                DUMP_R_TREE(node);

                dumpTex(")");

                break;
            }

            case SUB:
            {
                dumpTex("(");

                DUMP_L_TREE(node);

                dumpTex(" - ");

                DUMP_R_TREE(node);

                dumpTex(")");

                break;
            }

            case MUL:
            {
                DUMP_L_TREE(node);

                dumpTex(" \\cdot ");

                DUMP_R_TREE(node);

                break;
            }
            case DIV:
            {
                dumpTex(" \\frac{");

                DUMP_L_TREE(node);

                dumpTex(" }{ ");

                DUMP_R_TREE(node);

                dumpTex(" } ");

                break;
            }
            case SIN:
            {
               dumpTex("\\sin");

               dumpTex("(");

               DUMP_R_TREE(node);

               dumpTex(")")

               break;
            }
            case COS:
            {
                dumpTex("\\cos");

                dumpTex("(");

                DUMP_R_TREE(node);

                dumpTex(")");

                break;
            }
            case TG:
            {
                dumpTex("\\tan");

                dumpTex("(");

                DUMP_R_TREE(node);

                dumpTex(")");

                break;
            }
            case CTG:
            {
                dumpTex("\\cot");

                dumpTex("(");

                DUMP_R_TREE(node);

                dumpTex(")");

                break;
            }

            case LN:
            {
                dumpTex("\\ln");

                dumpTex("(");

                DUMP_R_TREE(node);

                dumpTex(")");

                break;
            }
            case POW:
            {
                dumpTex("(")

                DUMP_L_TREE(node);

                dumpTex("^{");

                DUMP_R_TREE(node);

                dumpTex("}");

                dumpTex(")");

                break;
            }

            default:
            {
                printf("Unknown function: %d!\n", node->data.func);
                return NULL;
            }
            }

            break;
        }
        default:
        {
            printf("Unknown type: %d!\n", node->type);
            return NULL;
        }
        
    }

    return OK;
}

#undef DUMP_L_TREE
#undef DUMP_R_TREE

ErrorCode beginExpressionTex(FILE* outFile)
{
    AssertSoft(outFile, UNABLE_TO_OPEN_FILE);

    dumpTex("\n\n"
            "\\begin{center}\n"
            "\\begin{equation}\n");

    return OK;
}

ErrorCode endExpressionTex(FILE* outFile)
{
    AssertSoft(outFile, UNABLE_TO_OPEN_FILE);

    dumpTex("\n\\end{equation}\n"
            "\\end{center}"
            "\n\n");

    return OK;
}

ErrorCode dumpExpressionTex(Node* node, FILE* outFile)
{
    AssertSoft(node, NULL_PTR);
    AssertSoft(outFile, UNABLE_TO_OPEN_FILE);

    dumpPhrase(outFile);

    beginExpressionTex(outFile);

    _dumpTreeTex(node, outFile);

    endExpressionTex(outFile);

    return OK;
}

ErrorCode dumpDiffResExpressionTex(Node* node, FILE* outFile)
{
    AssertSoft(node, NULL_PTR);
    AssertSoft(outFile, UNABLE_TO_OPEN_FILE);

    dumpPhrase(outFile);

    beginExpressionTex(outFile);

    _dumpTreeTex(node, outFile);

    endExpressionTex(outFile);

    return OK;   
}

ErrorCode dumpDiffExpressionTex(Node* node, FILE* outFile)
{
    AssertSoft(node, NULL_PTR);
    AssertSoft(outFile, UNABLE_TO_OPEN_FILE);

    dumpPhrase(outFile);

    beginExpressionTex(outFile);

    dumpTex("(");

    _dumpTreeTex(node, outFile);

    dumpTex(")'");

    endExpressionTex(outFile);

    return OK;
}

ErrorCode dumpPhrase(FILE* outFile)
{
    int randLineNum = rand() % numLines;

    dumpTex("\\begin{center}\n");

    dumpTex("%s\n", phrases[randLineNum]);

    dumpTex("\\end{center}");

    return OK;
}