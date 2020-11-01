#include "Processor.h"


int main(int argC, const char* argV[])
{
    const char* filename = "bc.bin";

    printf("argC = %d\n", argC);
    for (int i = 0; i < argC; ++i)
    {
        printf("%s\n", argV[i]);
    }
    printf("\n");
    if (argC == 1)
    {
        Executing(filename);
    }
    if (argC == 2)
    {
        Executing(argV[1]);
        return 0;

    }
    else if (argC > 2)
    {
        printf("COMMAND NOT FOUND!\n");
        return 2;
    }
}