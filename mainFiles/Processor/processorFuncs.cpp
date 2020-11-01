#include "Processor.h"

const uint8_t AMOUNT_REGISTERS = 4;
const uint8_t BASE_CAPACITY = 10;
const size_t RAM_SIZE = 1024;
const size_t sizeX = 120;
const size_t sizeY = 30;
const size_t GRAM_SIZE = sizeX * sizeY;


const char* REGISTERS[AMOUNT_REGISTERS] = 
{   
    "rax",
    "rbx",
    "rcx",
    "rdx"
};

typedef enum FLAGS
{
    NUM = 0x01,
    REG = 0x02,
    RAM = 0x04,
} FLAGS;

#include "enum.h"

typedef enum FLAG
{
    NUMBER = 0,
    REGISTER = 1

} FLAG;

elem_t Pow(const elem_t value_a, const elem_t value_b)
{
    return pow(value_b, value_a);
}

size_t findPower16(size_t number)
{
    int power = 0;
    while (number / 16 != 0)
    {
        ++power;
        number /= 16;
    }
    
    return power;
}

uint64_t approxLength(const char* filename)
{
	assert(filename);
	
	struct stat* buff = (struct stat*)calloc(1, sizeof(struct stat));

	stat(filename, buff);

	return buff->st_size;
	
}

void prepareBcodeDamp(FILE* file, uint64_t power)
{
    fprintf(file, "\n");
    for (uint64_t i = 0; i < power; ++i)
    {
        fprintf(file, " ");
    }
    fprintf(file, "   ");
    for (uint64_t i = 0; i < 16; ++i)
    {
        fprintf(file, "0%X ", i);
    }
    fprintf(file, "\n");
    for (uint64_t i = 0; i < power; ++i)
    {
        fprintf(file, " ");
    }
    fprintf(file, "   ");
    for (uint64_t i = 0; i < 16; ++i)
    {
        fprintf(file, "-- ");
    }
}

void pcAtFirstLine(uint8_t* bcode, FILE* file, uint64_t pc, uint64_t length)
{
    fprintf(file, "\n");
    fprintf(file, "%X: ", 16);
    for (uint64_t i = 0; i < length; ++i)
    {
        fprintf(file, "%02X ", bcode[i]);
    }
    fprintf(file, "\n    ");
    for (uint64_t i = 0; i < (pc % 16) * 2 + (pc % 16); ++i)
    {
        fprintf(file, "~");
    }
    fprintf(file, "^");
    fprintf(file, "\n");
}

void pcAtLastLine(FILE* file, uint64_t pc, uint64_t length, uint64_t power, uint64_t power_i)
{
    fprintf(file, "\n");
    for (uint64_t j = 0; j < power + power_i + 2; ++j)
    {
        fprintf(file, " ");
    }
    
    for (uint64_t k = 0; k < (pc % 16) * 2 + (pc % 16); ++k)
    {
        fprintf(file, "~");
    }
    
    fprintf(file, "^");
}

void bcodeDamp(uint8_t* bcode, uint64_t pc, uint64_t length)
{
    FILE* file = fopen("logs.txt", "a");

    fprintf(file, "\n================================================\n");
    fprintf(file, "DAMP OF CPU:\n");

    uint64_t power = findPower16(length) + 1;
    uint64_t power_i = 0;

    prepareBcodeDamp(file, power);
    
    if (length <= 16)
    {   
        pcAtFirstLine(bcode, file, pc, length);
        return;
    }
    
    for(uint64_t i = 0; i < length; ++i)
    {   
        if (i % 16 == 0)
        {   
            if (pc / 16 == i / 16 - 1)
            {
                fprintf(file, "\n");
                power_i = findPower16(i + 1);
                for (uint64_t j = 0; j < power + power_i + 2; ++j)
                {
                    fprintf(file, " ");
                }
                for (uint64_t k = 0; k < (pc % 16) * 2 + (pc % 16); ++k)
                {
                    fprintf(file, "~");
                }
                fprintf(file, "^");
            }
            fprintf(file, "\n");
            power_i = findPower16(i + 1);
            for (uint64_t j = 0; j < power - power_i; ++j)
            {
                fprintf(file, "0");
            }
            fprintf(file, "%X:", i);
            fprintf(file, " ");

        }
        
        fprintf(file, "%02X ", bcode[i]);
    }
    
    if (length / 16 == pc / 16)
    {   
        pcAtLastLine(file, pc, length, power, power_i);
    }  
    fprintf(file, "\npc = %llu\n", pc);    

    fclose(file);
}

void registerDamp(elem_t* registers)
{
    FILE* file = fopen("logs.txt", "a");

    fprintf(file, "\n");
    fprintf(file, "rax = %lg\n", registers[0]);
    fprintf(file, "rbx = %lg\n", registers[1]);
    fprintf(file, "rcx = %lg\n", registers[2]);
    fprintf(file, "rdx = %lg\n", registers[3]);

    fclose(file);
}

void ramDump(CPU* cpu)
{   
    FILE* file = fopen("logs.txt", "a");

    fprintf(file, "\nRAM DAMP\n=============================\n");
    size_t count_relevant = 0;
    for (size_t i = 0; i < RAM_SIZE; ++i)
    {
        if (count_relevant % 16 == 0 && count_relevant != 0)
        {
            fprintf(file, "\n");
        }
        if (!isnan(cpu->RAM[i]))
        {
            fprintf(file, "[%zu] = %lg |", i, cpu->RAM[i]);
            ++count_relevant;
        }
    }
    if (count_relevant == 0)
        fprintf(file, "[empty]");
    fprintf(file, "\n=============================\n");

    fclose(file);
}

void cpuDamp(CPU* cpu)
{    
    FILE* file = fopen("logs.txt", "a");

    bcodeDamp(cpu->bcode, cpu->pc, cpu->length);
    registerDamp(cpu->registers);
    stackDump_simple(cpu->stack, stackOk_simple(cpu->stack), __LINE__);
    stackDump_simple(cpu->call_stack, stackOk_simple(cpu->call_stack), __LINE__);
    ramDump(cpu);
    fprintf(file, "######################################\n");

    fclose(file);

}

void screenGPU(CPU* cpu)
{
    char* GRAM = (char*)(cpu->RAM + RAM_SIZE);

    for (uint64_t i = 0; i < sizeX + 2; ++i)
    {
        printf("#");
    }
    printf("\n");
    for (uint64_t y = 0; y < sizeY; ++y)
    {   
        printf("#");
        for (uint64_t x = 0; x < sizeX; ++x)
        {
            printf("%c", GRAM[y * sizeY + x]);
        }
        printf("#\n");
    }
    for (uint64_t i = 0; i < sizeX + 2; ++i)
    {
        printf("#");
    }
    printf("\n");
}

void loadBytecode(CPU* cpu, const char* filename)
{
    FILE* bytecode = fopen(filename, "rb");

    if (bytecode == NULL)
    {
        printf("ERROR READING FILE!\n");
        return;
    }
    cpu->length = approxLength(filename);

    if (cpu->bcode != NULL)
    {
        free(cpu->bcode);
    }

    cpu->bcode = (uint8_t*)calloc(cpu->length + 1, sizeof(uint8_t));
    cpu->length = fread(cpu->bcode, sizeof(uint8_t), cpu->length, bytecode);

    cpu->pc = 0;

    fclose(bytecode);
}

void loadRegisters(CPU* cpu)
{
    cpu->registers[0] = 0;
    cpu->registers[1] = 0;
    cpu->registers[2] = 0;
    cpu->registers[3] = 0;
}

void loadStacks(CPU* cpu)
{   
    cpu->stack = (Stack*)calloc(1, sizeof(Stack));
    cpu->call_stack = (Stack*)calloc(1, sizeof(Stack));

    constructStack(cpu->stack, 1);
    constructStack(cpu->call_stack, 1);
}

void loadRAM(CPU* cpu)
{
    cpu->RAM = (elem_t*)calloc(RAM_SIZE * sizeof(elem_t) + GRAM_SIZE * sizeof(char), 1);
    for (uint64_t i = 0; i < RAM_SIZE; ++i)
    {
        cpu->RAM[i] = NAN; 
    }

    char* GRAM = (char*)(cpu->RAM + RAM_SIZE);
    for (uint64_t i = 0; i < GRAM_SIZE; ++i)
    {
        GRAM[i] = ' ';//to const
    }
}


#define DEF_CMD(name, num, arg, ctrl_flow)  \
    case name##_COMMAND:                    \
        name##_FUNC                         \
        /*printf("CMD: %s\n", #name);*/     \
        break;   

void constructHandler(Handler* hdl)
{
    hdl->value = 0;
    hdl->value_a = 0;
    hdl->value_b = 0;
    hdl->result = 0;
    hdl->flag = 0;
    hdl->gpu_mod = 0;
    hdl->ram_address = 0;
    hdl->coordX = 0;
    hdl->coordY = 0;
}

Handler* newHandler()
{   
    Handler* hdl = (Handler*)calloc(1, sizeof(Handler));

    constructHandler(hdl);

    return hdl;
}

void loadCPU(CPU* cpu, const char* filename)
{
    loadBytecode(cpu, filename);
    loadStacks(cpu);
    loadRAM(cpu);
    loadRegisters(cpu);
}

void destructCPU(CPU* cpu)
{
    deleteStack_simple(cpu->stack);
    deleteStack_simple(cpu->call_stack);
    free(cpu->bcode);
    free(cpu);
}

void resetHandler(Handler* hdl)
{
    hdl->value = 0;
    hdl->value_a = 0;
    hdl->value_b = 0;
    hdl->result = 0;
    hdl->flag = 0;
    /*hdl->gpu_mod = 0;*/
    hdl->ram_address = 0;
    hdl->coordX = 0;
    hdl->coordY = 0;
}

int isNotEndOfFile(CPU* cpu)
{
    return (cpu->bcode[cpu->pc] != HLT_COMMAND && cpu->pc < cpu->length && cpu->bcode[cpu->pc] != EOF);
}

void Execution(CPU* cpu)
{
    Handler hdl = {};
    constructHandler(&hdl);
    while (isNotEndOfFile(cpu))
    {    
        cpuDamp(cpu);
        switch(cpu->bcode[cpu->pc])
        {
            #include "commands.h"

            default:
                printf("UNKNOWN COMMAND!\n");
                break;
            
        }
        // screenGPU(cpu);
        resetHandler(&hdl);
    }
}

#undef DEF_CMD

void Executing(const char* filename)
{   
    CPU cpu = {};
    loadCPU(&cpu, filename);
    
    Execution(&cpu);

    destructCPU(&cpu);
}



