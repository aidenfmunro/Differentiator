// TODO: "plot ..." <- equation

// TODO: tg ctg -> tan cot

// TODO: pow is ** in gnuplot

// TODO: example: plot sin(x), x

// TODO: clean memory leaks in c

#include "plotfunc.h"

const int TIME_TO_SEE = 30;

#define pipePlot(...) fprintf(outFile, __VA_ARGS__)

ErrorCode DumpPlot(const char* equation)
{
    FILE* outFile = popen("gnuplot", "w");

    pipePlot("plot %s\n", equation);

    fflush(outFile);

    sleep(TIME_TO_SEE);   

    return OK;
}
