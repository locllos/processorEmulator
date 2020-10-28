#include "headers/cpu.h"

uint64_t approxLength(const char* filename)
{
	assert(filename);
	
	struct stat* buff = (struct stat*)calloc(1, sizeof(struct stat));

	stat(filename, buff);

	return buff->st_size;
}

BinaryCode* openBinaryCode(const char* filename)
{
    BinaryCode* binary_code = (BinaryCode*)calloc(1, sizeof(BinaryCode));
    binary_code->length = approxLength(filename);
    FILE* file = fopen(filename, "rb");

    if (file == NULL)
    {
        printf("FILE DOESNT FIND!\n");

        return NULL;
    }

    binary_code->code = (uint8_t*)calloc(binary_code->length, sizeof(uint8_t));
    binary_code->length = fread(binary_code->code, sizeof(uint8_t), binary_code->length, file);
    binary_code->offset = 0;
    
    return binary_code;

}

#define DEF_CMD(name, num, arg, ctrl_flow)                  \
    if (bcode->code[bcode->offset] == num)                  \
    {                                                       \
        fprintf(file, #name);                               \
                                                            \
        printf("FIND COMMAND: %s\n", #name);                \
        bcode->offset += sizeof(uint8_t);                   \
        if (arg == 1)                                       \
        {                                                   \
            fprintf(file, " ");                             \
                                                            \
            flag = (FLAG)bcode->code[bcode->offset];        \
            bcode->offset += sizeof(uint8_t);               \
            printf("FIND FLAG: %d\n", flag);                \
            if (flag == NUMBER)                             \
            {                                               \
                num_address = bcode->code + bcode->offset;  \
                value = *((elem_t*)num_address);            \
                                                            \
                printf("FIND NUMBER: %lg\n", value);        \
                fprintf(file, "%lg", value);                \
                                                            \
                bcode->offset += sizeof(elem_t);            \
            }                                               \
            else if (flag == REGISTER)                      \
            {                                               \
                reg_index = bcode->code[bcode->offset];     \
                printf("FIND NUMBER: %s\n", REGISTERS[reg_index]);\
                fprintf(file, REGISTERS[reg_index]);         \
                                                            \
                bcode->offset += sizeof(uint8_t);           \
            }                                               \
        }                                                   \
        fprintf(file, "\n");                                \
    } else                                                  

void binaryCodeProcessing(const char* binary_filename, const char* filename)
{
    BinaryCode* bcode = openBinaryCode(binary_filename);

    FILE* file = fopen(filename, "w");

    elem_t value = 0;
    uint64_t reg_index = 0;
    uint8_t* num_address = NULL;
    FLAG flag = NUMBER;

    while (bcode->offset < bcode->length)
    {
        value = 0;
        reg_index  = 0;
        num_address = NULL;
        flag = NUMBER;


        #include "commands.h"

        /*else*/
        {
            printf("ERROR COMMAND DOESNT FIND!\n");
            printf("COMMAND CODE: %d\n", bcode->code[bcode->offset]);
            printf("OFFSET: %llu\n", bcode->offset);
            printf("length file: %llu\n", bcode->length);
            break;
        }

    }
}

#undef DEF_CMD

int main(int argC, const char* argV[])
{
    
    char* filename = (char*)calloc(32, sizeof(char));
    char* binary_filename = (char*)calloc(32, sizeof(char));

    const char* default_filename = "your_code.txt";
    const char* default_bin_filename = "bc.bin";

    printf("ARG C: %d\n", argC);
    for (int i = 0; i < argC; ++i)
    {
        printf("ARG V[%i] = %s\n", i, argV[i]);
    }

    if (argC == 1)
    {
        binaryCodeProcessing(default_bin_filename, default_filename);
    }
    else if (argC == 2)
    {   
        binaryCodeProcessing(argV[1], default_filename);

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