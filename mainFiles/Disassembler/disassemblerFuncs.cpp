#include "Disassembler.h"
#include "enum.h"

const char* REGISTERS[4] = 
{   
    "rax",
    "rbx",
    "rcx",
    "rdx"
};

uint64_t approxLength(const char* filename)
{
	assert(filename);
	
	struct stat* buff = (struct stat*)calloc(1, sizeof(struct stat));

	stat(filename, buff);

	return buff->st_size;
}

int isCommand(BinaryCode* bcode, const uint8_t num)
{
    return (bcode->code[bcode->offset] == num);
}

int isNumber(uint8_t flag)
{
    return (flag & 0x01) != 0;
}

int isCtrlFlow(uint8_t flag)
{
    return (flag == 0);
}

int isRegister(uint8_t flag)
{
    return (flag & 0x02) != 0;
}

int isRAM(uint8_t flag)
{
    return (flag & 4) != 0;
}

void writeTextNumber(BinaryCode* bcode, FILE* file)
{
    uint8_t* num_address = bcode->code + bcode->offset; 
    elem_t value = *((elem_t*)num_address);            
                                                
    printf("FIND NUMBER: %lg\n", value);        
    fprintf(file, "%lg", value);                
                                                
    bcode->offset += sizeof(elem_t);  
}

void writeTextRegister(BinaryCode* bcode, FILE* file, uint8_t flag)
{
    uint8_t reg_index = bcode->code[bcode->offset];     
    printf("FIND REGISTER: %s\n", REGISTERS[reg_index]);
    fprintf(file, REGISTERS[reg_index]);         
                                                
    bcode->offset += sizeof(uint8_t);

    if (isRAM(flag) && isNumber(flag))
    {
        fprintf(file, " + ");
    }
}

void writeTextCommand(BinaryCode* bcode, FILE* file, const char* name)
{
        fprintf(file, name);                           
                                                            
        printf("FIND COMMAND: %s\n", name);                
        bcode->offset += sizeof(uint8_t);   
}

BinaryCode* openBinaryCode(BinaryCode* binary_code, const char* filename)
{
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

void writeTextOpenBracket(BinaryCode* bcode, FILE* file)
{
    fprintf(file, "[");
}

void writeTextCloseBracket(BinaryCode* bcode, FILE* file)
{
    fprintf(file, "]");
}

void writeOneArgument(BinaryCode* bcode, FILE* file)
{
    uint8_t flag = bcode->code[bcode->offset];
    bcode->offset += sizeof(uint8_t);      
    printf("FIND FLAG: %d\n", flag);       
    if (isRAM(flag))                       
    {                                      
        writeTextOpenBracket(bcode, file);
    }                                      
    if (isRegister(flag))                  
    {                                      
        writeTextRegister(bcode, file, flag);    
    }                                      
    if (isNumber(flag) || isCtrlFlow(flag))
    {                                      
        writeTextNumber(bcode, file);
    }                                      
    if (isRAM(flag))                       
    {                                      
        writeTextCloseBracket(bcode, file);
    }
}

#define DEF_CMD(name, num, arg, ctrl_flow)                  \
    if (isCommand(&bcode, num))                              \
    {                                                       \
        writeTextCommand(&bcode, file, #name);               \
        fprintf(file, " ");                                 \
        if (arg == 1)                                       \
        {                                                   \
            writeOneArgument(&bcode, file);                  \
        }                                                   \
        fprintf(file, "\n");                                \
    } else                                                  

void binaryCodeProcessing(const char* binary_filename, const char* filename)
{
    BinaryCode bcode = {};
    openBinaryCode(&bcode, binary_filename);

    FILE* file = fopen(filename, "w");

    elem_t value = 0;
    uint64_t reg_index = 0;
    uint8_t* num_address = NULL;
    FLAG flag = NUMBER;

    while (bcode.offset < bcode.length)
    {
        value = 0;
        reg_index  = 0;
        num_address = NULL;
        flag = NUMBER;


        #include "../commands.h"

        /*else*/
        {
            printf("ERROR COMMAND DOESNT FIND!\n");
            printf("COMMAND CODE: %d\n", bcode.code[bcode.offset]);
            printf("OFFSET: %llu\n", bcode.offset);
            printf("length file: %llu\n", bcode.length);
            break;
        }

    }
    fclose(file);
}

#undef DEF_CMD