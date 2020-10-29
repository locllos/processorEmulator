#include "headers/processing_funcs.h"
#include "headers/cpu.h"

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

void pcAtFirstLine(BinaryCode* bcode, FILE* file, uint64_t pc, uint64_t length)
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

void bcodeDamp(BinaryCode* bcode, uint64_t pc, uint64_t length, FILE* file)
{
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
}

void registerDamp(elem_t* registers, FILE* file)
{
    fprintf(file, "\n");

    fprintf(file, "rax = %lg\n", registers[0]);
    fprintf(file, "rbx = %lg\n", registers[1]);
    fprintf(file, "rcx = %lg\n", registers[2]);
    fprintf(file, "rdx = %lg\n", registers[3]);
}

void ramDump(CPU* cpu, FILE* file)
{
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
            fprintf(file, "%lg ", cpu->RAM[i]);
            ++count_relevant;
        }
    }
    fprintf(file, "\n=============================\n");
}

void cpuDamp(CPU* cpu)
{    
    FILE* file = fopen("logs.txt", "a");
    bcodeDamp(cpu->bcode, cpu->pc, cpu->length, file);
    registerDamp(cpu->registers, file);
    stackDump_simple(cpu->stack, stackOk_simple(cpu->stack), __LINE__);
    stackDump_simple(cpu->call_stack, stackOk_simple(cpu->call_stack), __LINE__);
    ramDump(cpu, file);
    fclose(file);
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
    cpu->RAM = (elem_t*)calloc(RAM_SIZE * sizeof(elem_t) + GRAM_SIZE * sizeof(uint8_t), 1);
    for (uint64_t i = 0; i < RAM_SIZE; ++i)
    {
        cpu->RAM[i] = NAN; 
    }

}

#define DEF_CMD(name, num, arg, ctrl_flow)  \
    case name##_COMMAND:                    \
        name##_FUNC                         \
        break;   



void Proccesor(CPU* cpu)
{
    cpuDamp(cpu);
    while (cpu->bcode[cpu->pc] != HLT_COMMAND && cpu->pc < cpu->length && cpu->bcode[cpu->pc] != EOF)
    {    
        elem_t value = 0;
        elem_t value_a = 0;
        elem_t value_b = 0;
        elem_t result = 0;
        uint8_t flag = 0;
        uint64_t ram_address = 0;

        switch(cpu->bcode[cpu->pc])
        {
            #include "commands.h"

            default:
                printf("UNKNOWN COMMAND!\n");
                break;
            
        }
        if (cpu->bcode[cpu->pc] != OUT_COMMAND)
        {
            cpuDamp(cpu);
        }
        ram_address = 0;
    }

    deleteStack_simple(cpu->stack);
    free(cpu->bcode);
    free(cpu);
}

#undef DEF_CMD

void Executing(const char* filename)
{   
    CPU* cpu = (CPU*)calloc(1, sizeof(CPU));

    loadBytecode(cpu, filename);
    loadStacks(cpu);
    loadRAM(cpu);

    loadRegisters(cpu);


    Proccesor(cpu);
}

int main(int argC, const char* argV[])
{
    char* filename = (char*)calloc(32, sizeof(char));
    strcpy(filename, "bc.bin");

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
        strcpy(filename, argV[1]);
        Executing(filename);
        return 0;

    }
    else if (argC > 2)
    {
        printf("COMMAND NOT FOUND!\n");
        return 2;
    }

}