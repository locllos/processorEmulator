#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#define EPS 1e-9
#define NOLL NAN

#define constructStack(pseudo_address, start_capacity)                      \
    constructStack_simple(pseudo_address, start_capacity, #pseudo_address)

typedef double elem_t;

typedef enum OPERATION
{
    PUSH,
    POP

} OPERATION;

typedef enum ERROR_MESSAGE
{
    NO_ERROR,

    NULL_STACK_ERROR,
    STACK_CANARRY_A_ERROR,
    STACK_CANARRY_B_ERROR,
    BUFFER_CANARRY_A_ERROR,
    BUFFER_CANARRY_B_ERROR,
    NULL_BUFFER_ERROR,
    BAD_CAPACITY_ERROR,
    BAD_SIZE_ERROR,
    BIG_SIZE_ERROR,
    CHANGE_SOME_DATA_ERROR,
    NOLL_ELEMENT_ERROR

} ERROR_MESSAGE;

typedef struct Stack
{
    long int* stack_canarry_a;

    int size;
    int capacity;
    elem_t* buffer;
    const char* var_name;
    int check_sum;
    
    elem_t* buffer_canarry_a;
    elem_t* buffer_canarry_b;

    long int* stack_canarry_b;

} Stack;


int checkSum(Stack* stack);

int sizeStack_simple(Stack* stack);

void constructStack_simple(Stack* stack, const int start_capacity, const char* var_name);

Stack* newStack_simple(const int start_capacity);

void pushStack_simple(Stack* stack, const elem_t value);

elem_t topStack_simple(Stack* stack);

void eraseStack_simple(Stack* stack);

elem_t popStack_simple(Stack* stack);

void deleteStack_simple(Stack* stack);

ERROR_MESSAGE stackOk_simple(Stack* stack);

void stackDump_simple(Stack* stack, const ERROR_MESSAGE result_of_operation, const int line_number);

void stackDumpConsole_simple(Stack* stack, const ERROR_MESSAGE result_of_operation, const int line_number);


