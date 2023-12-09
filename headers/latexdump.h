#ifndef LATEXDUMP_H
#define LATEXDUMP_H

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "differentiator.h"
#include "errors.h"

ErrorCode DumpTreeTex(Tree* tree);

ErrorCode beginExpressionTex (FILE* outFile);

ErrorCode endExpressionTex (FILE* outFile);

ErrorCode dumpExpressionTex (Node* node, FILE* outFile);

ErrorCode dumpDiffExpressionTex (Node* node, FILE* outFile);

ErrorCode dumpDiffResExpressionTex (Node* node, FILE* outFile);

ErrorCode dumpPhrase(FILE* outFile);

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

static const char* phrases[] = {"Кто в армии служил, тот в цирке не смеется",
                                "Чистая совесть - лучшее снотворное",
                                "Жизнь - это собачья упряжка: пока ты не лидер, пейзаж не меняется",
                                "Иногда только промахнувшись, понимаешь, как ты попал",
                                "Красть мысли у одного человека - плагиат. Красть у многих - научное исследование",
                                "Мудрый не тот, кто много размышляет о великом, а тот, кто мало думает о мелочах",
                                "Мало создать шедевр. Надо еще найти людей, которые бы его оценили",
                                "Самый подходящий момент наступает в самое неподходящее время"};

static int numLines = sizeof(*phrases) - 1;

#endif