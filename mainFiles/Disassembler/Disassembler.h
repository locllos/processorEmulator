#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>
#include <sys\stat.h> //Only for windows users :(

typedef double elem_t;

typedef enum FLAGS
{
    NUM = 0x01,
    REG = 0x02,
    RAM = 0x04,
} FLAGS;

typedef enum FLAG
{
    NUMBER = 0,
    REGISTER = 1

} FLAG;

typedef struct BinaryCode
{
    uint8_t* code;
    uint64_t offset;
    uint64_t length;
    uint64_t capacity;

} BinaryCode;

uint64_t approxLength(const char* filename);

int isCommand(BinaryCode* bcode, const uint8_t num);

int isNumber(uint8_t flag);

int isCtrlFlow(uint8_t flag);

int isRegister(uint8_t flag);

int isRAM(uint8_t flag);

void writeTextNumber(BinaryCode* bcode, FILE* file);

void writeTextRegister(BinaryCode* bcode, FILE* file, uint8_t flag);

void writeTextCommand(BinaryCode* bcode, FILE* file, const char* name);

BinaryCode* openBinaryCode(const char* filename);

void writeTextOpenBracket(BinaryCode* bcode, FILE* file);

void writeTextCloseBracket(BinaryCode* bcode, FILE* file);

void writeOneArgument(BinaryCode* bcode, FILE* file);

void binaryCodeProcessing(const char* binary_filename, const char* filename);
