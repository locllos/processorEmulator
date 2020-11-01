#include "Disassembler.h"

int main(int argC, const char* argV[])
{
    char* filename = "your_code.txt";
    char* binary_filename = "bc.bin";

    printf("ARG C: %d\n", argC);
    for (int i = 0; i < argC; ++i)
    {
        printf("ARG V[%i] = %s\n", i, argV[i]);
    }

    if (argC == 1)
    {
        binaryCodeProcessing(filename, binary_filename);
    }
    else if (argC == 2)
    {   
        binaryCodeProcessing(argV[1], binary_filename);

    }
    else if (argC == 4 && strcmp(argV[2], "~") == 0)
    {
        binaryCodeProcessing(argV[1], argV[3]);
    }
    else
    {
        printf("UNKNOWN COMMANDS!\n");

        return 1;
    }

    return 0;
}