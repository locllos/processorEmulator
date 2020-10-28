#include "cpu.h"

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

void In(CPU* cpu)
{   
    elem_t value = 0;
    scanf("%lg", &value);

    pushStack_simple(cpu->stack, value);

    cpu->pc += sizeof(uint8_t);
}

void Out(CPU* cpu)
{
    elem_t output = 0;

    printf("%lg\n", topStack_simple(cpu->stack));
    
    cpu->pc += sizeof(uint8_t);

}

void Push(CPU* cpu)
{       
    cpu->pc += sizeof(uint8_t);
    
    elem_t value = 0;

    if (cpu->bcode[cpu->pc] == NUMBER)
    {
        cpu->pc += sizeof(uint8_t);

        value = *((elem_t*)(cpu->bcode + cpu->pc));

        cpu->pc += sizeof(elem_t);
    }
    else if (cpu->bcode[cpu->pc] == REGISTER)
    {
        cpu->pc += sizeof(uint8_t);

        value = cpu->registers[cpu->bcode[cpu->pc]];

        cpu->pc += sizeof(uint8_t);
    }

    //printf("PUSHING: %lg\n", value);
    pushStack_simple(cpu->stack, value);
}

void Pop(CPU* cpu)
{
    cpu->pc += sizeof(uint8_t);

    if (cpu->bcode[cpu->pc] == NUMBER)
    {
        eraseStack_simple(cpu->stack);
    }
    else if (cpu->bcode[cpu->pc] == REGISTER)
    {
        cpu->pc += sizeof(uint8_t);
        
        cpu->registers[cpu->bcode[cpu->pc]] = popStack_simple(cpu->stack);
    }
    cpu->pc += sizeof(uint8_t);
}

void Add(CPU* cpu)
{
    const elem_t value_a = popStack_simple(cpu->stack);
    const elem_t value_b = popStack_simple(cpu->stack);

    const elem_t result = value_a + value_b;

    pushStack_simple(cpu->stack, result);
    
    cpu->pc += sizeof(uint8_t);

}

void Sub(CPU* cpu)
{
    cpu->pc += sizeof(uint8_t);

    const elem_t value_a = popStack_simple(cpu->stack);
    const elem_t value_b = popStack_simple(cpu->stack);

    const elem_t result = value_b - value_a;
    
    pushStack_simple(cpu->stack, result);
}

void Mul(CPU* cpu)
{   
    cpu->pc += sizeof(uint8_t);

    const elem_t value_a = popStack_simple(cpu->stack);
    const elem_t value_b = popStack_simple(cpu->stack);

    const elem_t result = value_a * value_b;
    
    pushStack_simple(cpu->stack, result);
}

void Div(CPU* cpu)
{
    cpu->pc += sizeof(uint8_t);

    const elem_t value_a = popStack_simple(cpu->stack);
    const elem_t value_b = popStack_simple(cpu->stack);

    const elem_t result = value_b / value_a;
    
    pushStack_simple(cpu->stack, result);
}

void Pow(CPU* cpu)
{
    cpu->pc += sizeof(uint8_t);

    const elem_t value_a = popStack_simple(cpu->stack);
    const elem_t value_b = popStack_simple(cpu->stack);

    const elem_t result = pow(value_b, value_a);
    
    pushStack_simple(cpu->stack,result);
}

void Sqrt(CPU* cpu)
{
    cpu->pc += sizeof(uint8_t);

    const elem_t value = popStack_simple(cpu->stack);
    
    const elem_t result = sqrt(value);
    
    pushStack_simple(cpu->stack, result);
} 

void Sin(CPU* cpu)
{
    cpu->pc += sizeof(uint8_t);

    const elem_t value = popStack_simple(cpu->stack);

    const elem_t result = sin(value);
    
    pushStack_simple(cpu->stack, result);
} 

void Cos(CPU* cpu)
{
    cpu->pc += sizeof(uint8_t);

    const elem_t value = popStack_simple(cpu->stack);

    const elem_t result = cos(value);
    
    pushStack_simple(cpu->stack, result);
} 

void bcodeDamp(uint8_t* bcode, uint64_t pc, uint64_t length, const char* filename)
{
    FILE* file = fopen(filename, "a");

    fprintf(file, "\n================================================\n");
    fprintf(file, "DAMP OF CPU:\n");

    uint64_t power = findPower16(length) + 1;
    uint64_t power_i = 0;
    //Писалось на коленках

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
    
    if (length <= 16)
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
    fprintf(file, "\n");
    fprintf(file, "pc = %llu", pc);
    fprintf(file, "\n");

    fclose(file);
    
}

void registerDamp(elem_t* registers, const char* filename)
{
    FILE* file = fopen(filename, "a");
    fprintf(file, "\n");

    fprintf(file, "rax = %lg\n", registers[0]);
    fprintf(file, "rbx = %lg\n", registers[1]);
    fprintf(file, "rcx = %lg\n", registers[2]);
    fprintf(file, "rdx = %lg\n", registers[3]);

    fclose(file);
}

void cpuDamp(CPU* cpu)
{       
    bcodeDamp(cpu->bcode, cpu->pc, cpu->length, "logs.txt");
    registerDamp(cpu->registers, "logs.txt");
    stackDump_simple(cpu->stack, stackOk_simple(cpu->stack), __LINE__);
    //printf("CODE AT CPU: %d\n", stackOk_simple(cpu->stack));
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

void loadStack(CPU* cpu)
{   
    cpu->stack = (Stack*)calloc(1, sizeof(Stack));

    constructStack(cpu->stack, 1);
}

void Proccesor(CPU* cpu)
{ 
    cpuDamp(cpu);
    while (cpu->bcode[cpu->pc] != HLT_COMMAND && cpu->pc < cpu->length && cpu->bcode[cpu->pc] != EOF)
    {
        //printf("FUNCTION: %s\n", COMMANDS[cpu->bcode[cpu->pc]]);
    
        switch(cpu->bcode[cpu->pc])
        {
            case IN_COMMAND:
                In(cpu);
                break;

            case OUT_COMMAND:
                Out(cpu);
                break;

            case PUSH_COMMAND:
                Push(cpu);
                break;

            case POP_COMMAND:
                Pop(cpu);
                break;

            case ADD_COMMAND:
                Add(cpu);
                break;

            case SUB_COMMAND:
                Sub(cpu);
                break;
            
            case MUL_COMMAND:
                Mul(cpu);
                break;
                
            case DIV_COMMAND:
                Div(cpu);
                break;

            case POW_COMMAND:
                Pow(cpu);
                break;
            
            case SQRT_COMMAND:
                Sqrt(cpu);
                break;
            
            case SIN_COMMAND:
                Sin(cpu);
                break;

            case COS_COMMAND:
                Cos(cpu);
                break;
            
            case HLT_COMMAND:
                printf("END OF PROGRAMM.\n");
                break;

            default:
                printf("UNKNOWN COMMAND!\n");
                break;
            
        }
        if (cpu->bcode[cpu->pc] != OUT_COMMAND)
        {
            cpuDamp(cpu);
        }
    }

    deleteStack_simple(cpu->stack);
    free(cpu->bcode);
    free(cpu);
}

void Executing(const char* filename)
{   
    CPU* cpu = (CPU*)calloc(1, sizeof(CPU));

    loadBytecode(cpu, filename);

    loadStack(cpu);

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
    printf("======\n");

    if (argC == 2)
    {
        strcpy(filename, argV[1]);
    }
    else if (argC > 2)
    {
        printf("COMMAND NOT FOUND!\n");
        return -1;
    }
    Executing(filename);

    return 0;
}