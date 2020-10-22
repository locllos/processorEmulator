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

#define DEF_CMD(name, num, arg)                             \
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

    strcpy(filename, "your_code.txt");
    strcpy(binary_filename, "bc.bin");

    //когда-нибудь я сделаю, чтобы название можно было вводить через консоль

    binaryCodeProcessing(binary_filename, filename);

    return 0;
}