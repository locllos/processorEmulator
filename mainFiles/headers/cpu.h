#include "stack.h"
#include <string.h>
//#include <math.h>
#include <time.h>
#include <assert.h>
#include <sys\stat.h> //Only for windows users :(
    
const uint8_t AMOUNT_COMMANDS = 14;
const uint8_t AMOUNT_REGISTERS = 4;
const uint8_t BASE_CAPACITY = 10;
const uint64_t NUM_ALGORITHM_PASS = 2;
const elem_t DAM = -1;
const size_t RAM_SIZE = 1024;
const size_t GRAM_SIZE = 128 * 128;

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
    Stack* call_stack;
    elem_t* RAM;

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

typedef struct BinaryCode
{
    uint8_t* code;
    uint64_t offset;
    uint64_t length;
    uint64_t capacity;

} BinaryCode;

typedef struct Label
{
    char* label_name;
    elem_t mark;
    uint64_t offset;
} Label;

typedef struct Labels
{
    Label* lb;
    uint64_t size;
    uint64_t capacity;
} Labels;

typedef struct CurLine
{
    char* string;
    char* cmd;
    char* reg;

    elem_t value;
    uint8_t reg_index;
    uint64_t pos;
    int tmp_pos;
    uint64_t line;

    uint8_t flag;
    Label* cur_label;

} CurLine;

uint64_t approxLength(const char* filename);