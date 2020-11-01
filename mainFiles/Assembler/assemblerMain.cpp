#include "Assembler.h"

int main(const int argC, const char* argV[])
{   
    const char* default_filename = "my_code.txt";
    const char* default_bin_filename = "bc.bin";
    size_t length = 0;

    printf("ARG C: %d\n", argC);
    for (int i = 0; i < argC; ++i)
    {
        printf("ARG V[%i] = %s\n", i, argV[i]);
    }
    if (argC == 1)
    {
        return Translator(default_filename, default_bin_filename);
    }
    if (argC == 2)
    {   
        return Translator(argV[1], default_bin_filename);
    }
    if (argC == 4 && strcmp(argV[2], "~") == 0)
    {
        return Translator(argV[1], argV[3]);
    }
    else
    {
        printf("UNKNOWN COMMANDS!\n");
        return 1;
    }
  
    return 0;

}
