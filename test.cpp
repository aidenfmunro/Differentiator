#include <stdio.h>

int main(void)
{
    int matrix[10][20] = {0};

    int* ptr = (int*)matrix;

    printf("%d\n", *ptr);
}