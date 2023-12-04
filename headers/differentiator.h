#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"
#include "utils.h"

Node* differentiateTree(Node* curNode, FILE* outFile);

ErrorCode DumpTreeTex(Tree* tree);

static const char* BeginTexDocument = "\\documentclass[a4paper,12pt]{article}\n"
                                      "\\usepackage{cmap}\n"
                                      "\\usepackage[T2A]{fontenc}\n"
                                      "\\usepackage[utf8]{inputenc}\n"
                                      "\\usepackage[english, russian]{babel}\n"
                                      "\\usepackage{mathtext}\n"
                                      "\\usepackage{amsmath, amsfonts, amssymb, amsthm, mathtools}\n"
                                      "\\usepackage{textcomp}\n"
                                      "\\usepackage{euscript}\n"
                                      "\\usepackage{xcolor}\n"
                                      "\\usepackage{hyperref}\n"
                                      "\\usepackage[normalem]{ulem}\n"
                                      "\\definecolor{urlcolor}{HTML}{799B03}\n"
                                      "\\title{\\textbf{Дифференцирование древних викингов}}\n"
                                      "\\author{Эйден Манро мудрейший из викингов}\n"
                                      "\\date{2023}\n"
                                      "\\begin{document}\n"
                                      "\\maketitle\n";

static const char* EndTexDocument = "\\end{document}\n";

#endif