#include "stack.h"
#include "processing_funcs.h"
#include <ctype.h>
#include <time.h>
#include <sys\stat.h> //Only for windows users :(


typedef struct CPU
{
    Stack* stack;
    Stack* call_stack;
    elem_t* RAM;

    uint8_t* bcode;
    uint64_t length;

    uint64_t pc;
    
    elem_t registers[4];

} CPU;

typedef struct Handler
{
    elem_t value;
    elem_t value_a;
    elem_t value_b;
    elem_t result;

    uint8_t flag;
    uint8_t gpu_mod;
    
    uint64_t ram_address;
    uint64_t coordX;
    uint64_t coordY;

    char symbol;

} Handler;


elem_t Pow(const elem_t value_a, const elem_t value_b);

size_t findPower16(size_t number);

uint64_t approxLength(const char* filename);

void prepareBcodeDamp(FILE* file, uint64_t power);

void pcAtFirstLine(uint8_t* bcode, FILE* file, uint64_t pc, uint64_t length);

void pcAtLastLine(FILE* file, uint64_t pc, uint64_t length, uint64_t power, uint64_t power_i);

void bcodeDamp(uint8_t* bcode, uint64_t pc, uint64_t length, FILE* file);

void registerDamp(elem_t* registers, FILE* file);

void ramDump(CPU* cpu, FILE* file);

void cpuDamp(CPU* cpu, FILE* file);

void screenGPU(CPU* cpu);

void loadBytecode(CPU* cpu, const char* filename);

void loadRegisters(CPU* cpu);

void loadStacks(CPU* cpu);

void loadRAM(CPU* cpu);

void constructHandler(Handler* hdl);

Handler* newHandler();

void loadCPU(CPU* cpu, const char* filename);

void destructCPU(CPU* cpu);

void resetHandler(Handler* hdl);

int isNotEndOfFile(CPU* cpu);

void Execution(CPU* cpu);

void Executing(const char* filename);