#include "stack.h"
#include <string.h>
#include <math.h>
#include <assert.h>
#include <sys\stat.h> //Only for windows users :(

const uint8_t LENGTH_IN_STRING = 2;
const uint8_t LENGTH_OUT_STRING = 3;
const uint8_t LENGTH_POP_STRING = 3;
const uint8_t LENGTH_POW_STRING = 3;
const uint8_t LENGTH_SUB_STRING = 3;
const uint8_t LENGTH_ADD_STRING = 3;
const uint8_t LENGTH_MUL_STRING = 3;
const uint8_t LENGTH_SIN_STRING = 3;
const uint8_t LENGTH_COS_STRING = 3;
const uint8_t LENGTH_HLT_STRING = 3;
const uint8_t LENGTH_PUSH_STRING = 4;
const uint8_t LENGTH_SQRT_STRING = 4;
const uint8_t LENGTH_REGISTER_NAME = 3;

const uint8_t AMOUNT_COMMANDS = 14;
const uint8_t AMOUNT_REGISTERS = 4;

const char* COMMANDS[AMOUNT_COMMANDS] = 
{   
    ";",
    "IN",
    "OUT",
    "PUSH",
    "POP",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "POW",
    "SQRT",
    "SIN",
    "COS",
    "HLT"
};

const char* REGISTERS[AMOUNT_REGISTERS] = 
{   
    "rax",
    "rbx",
    "rcx",
    "rdx"
};

typedef enum FLAG
{
    NUMBER = 0,
    REGISTER = 1

} FLAG;

/*typedef enum COMMAND
{
    DO_NOTHING_COMMAND = 0,

    IN_COMMAND = 1,
    OUT_COMMAND = 2,
    PUSH_COMMAND = 3,
    POP_COMMAND = 4,
    ADD_COMMAND = 5,
    SUB_COMMAND = 6,
    MUL_COMMAND = 7,
    DIV_COMMAND = 8,
    POW_COMMAND = 9,
    SQRT_COMMAND = 10,
    SIN_COMMAND = 11,
    COS_COMMAND = 12,
    HLT_COMMAND = 13,

} COMMAND; */
#include "enum.h"

typedef struct CPU
{
    Stack* stack;

    uint8_t* bcode;
    uint64_t length;

    uint64_t pc;
    
    elem_t registers[4];

} CPU;

typedef struct Line
{
	char* string;
	int	  size;

} Line;

typedef struct Text
{
    Line* lines;
    uint64_t number_of_lines;
    uint64_t length;
    char* buffer;
} Text;

uint64_t approxLength(const char* filename);