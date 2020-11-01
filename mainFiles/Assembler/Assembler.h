#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <assert.h>
#include <sys\stat.h> //Only for windows users :(
    
typedef double elem_t;

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
} Label;

typedef struct Labels
{
    Label* lb;
    size_t size;
    size_t capacity;
} Labels;


typedef struct AsmLine
{
    size_t length;
    size_t amount_lexemes;

    char* buffer;
    char** lexemes;
} AsmLine;

typedef struct AsmListing
{
    char* main_buffer;

    AsmLine* asm_lines;

    size_t amount_lines;
    size_t length;
} AsmListing;

//SERVICE FUNCS:
uint64_t approxLength(const char* filename);

void clearCloseBracket(char* string);

int indexStringInArray(char* string, const char* arr[], const size_t length);

void Copy(void* value_to, const void* value_from, const size_t elem_size);

void copyToData(void* array, const void* value, const size_t elem_size, const size_t offset);

void* updData(void* data, size_t* capacity, const size_t size, const size_t elem_size);

char* readNext(char* string);

int isValidSymbol(char symbol);

int isNumber(char* string);

int isRegister(char* string);

int isLabel(char* string);

int isCtrlFlow(int ctrl_flow);

int isAdditional(AsmLine* line, size_t pos);

int isOpenBracket(char* string);

int isCloseBracketInTheEnd(char* string);

void skipSpaceSymbols(char** string);

void jumpOverValidSymbols(char** string, bool* symbol_pass);

size_t getAmountLexemes(char* string);

char* nextLexeme(AsmLine* asm_line, size_t* pos);

void outputMyBuffer(AsmLine* line);

void labelError(AsmLine* line);

int argumentError(AsmLine* line, size_t pos, size_t num_line);

int tooManyArgumentsError(AsmLine* line, size_t pos, size_t num_line);

//LINE:
void constructAsmLine(AsmLine* line, char* string, size_t size);

void destructAsmLine(AsmLine* asm_line);

AsmLine* newAsmLine(char* string, size_t size);

AsmLine* deleteAsmLine(AsmLine* asm_line);


//ASM LISTING:
void constructAsmListing(AsmListing* asm_listing, char* buffer, size_t length);

AsmListing* newAsmListing(char* buffer, size_t length);

char* readContents(const char* filename, size_t* length);

void showAsmListing(AsmListing* asm_listing);

void fillAsmListing(AsmListing* asm_listing, const char* filename);

void destructAsmListing(AsmListing* asm_listing);

AsmListing* deleteAsmListing(AsmListing* asm_listing);


//LABELS:
void constructLabels(Labels* labels);

Labels* newLabels();

Label* findLabel(Labels* labels, const char* label_name);

void addLabel(Labels* labels, const char* label_name, const elem_t mark);

void pushLabel(char* string, BinaryCode* bcode, Labels* labels);

void showLabels(Labels* labels);

void destructLabels(Labels* labels);

Labels* deleteLabels(Labels* labels);


//BINARY CODE:
void constructBinaryCode(BinaryCode* bcode);
BinaryCode* newBinaryCode();

void capacityProcessing(BinaryCode* bcode, const size_t elem_size);

void pushFromOffset(BinaryCode* bcode, const void* elem, size_t elem_size);

void insertTo(BinaryCode* bcode, const void* value, const size_t elem_size, const size_t index);

void outputBinary(BinaryCode* bcode);

void writeToBinary(BinaryCode* bcode, const char* filename);

void writeNumber(char* string, BinaryCode* bcode);

void writeRegister(char* string, BinaryCode* bcode);

void destructBinaryCode(BinaryCode* bcode);

BinaryCode* deleteBinaryCode(BinaryCode* bcode);

void commentsProcessing(AsmLine* asm_line);

int ramProcessing(BinaryCode* bcode, AsmLine* line, size_t* pos, uint8_t* flag, size_t alg_pass);

int ctrlFlowProcessing(BinaryCode* bcode, Labels* labels, AsmLine* line, size_t* pos, size_t alg_pass);

int defualtProcessing(BinaryCode* bcode, AsmLine* line, size_t* pos, uint8_t* flag);

int argumentsProcessing(size_t arg, AsmLine* line, size_t* pos, BinaryCode* bcode, Labels* labels, int ctrl_flow, size_t alg_pass);

int binaryTranslation(BinaryCode* bcode, AsmListing* asm_listing);

int Translator(const char* filename, const char* binary_filename);