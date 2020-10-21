 #define IN_FUNC                                        \
    {                                                   \
    scanf("%lg", &value);                               \
                                                        \
    pushStack_simple(cpu->stack, value);                \
                                                        \
    cpu->pc += sizeof(uint8_t);                         \
    }

#define OUT_FUNC                                        \
{                                                       \
    printf("%lg\n", topStack_simple(cpu->stack));       \
                                                        \
    cpu->pc += sizeof(uint8_t);                         \
}

#define PUSH_FUNC                                       \
{                                                       \
    cpu->pc += sizeof(uint8_t);                         \
                                                        \
    if (cpu->bcode[cpu->pc] == NUMBER)                  \
    {                                                   \
        cpu->pc += sizeof(uint8_t);                     \
                                                        \
        value = *((elem_t*)(cpu->bcode + cpu->pc));     \
                                                        \
        cpu->pc += sizeof(elem_t);                      \
    }                                                   \
    else if (cpu->bcode[cpu->pc] == REGISTER)           \
    {                                                   \
        cpu->pc += sizeof(uint8_t);                     \
                                                        \
        value = cpu->registers[cpu->bcode[cpu->pc]];    \
                                                        \
        cpu->pc += sizeof(uint8_t);                     \
    }                                                   \
                                                        \
    pushStack_simple(cpu->stack, value);                \
}

#define POP_FUNC                                        \
{                                                       \
    cpu->pc += sizeof(uint8_t);                         \
                                                        \
    if (cpu->bcode[cpu->pc] == NUMBER)                  \
    {                                                   \
        eraseStack_simple(cpu->stack);                  \
    }                                                   \
    else if (cpu->bcode[cpu->pc] == REGISTER)           \
    {                                                   \
        cpu->pc += sizeof(uint8_t);                     \
                                                        \
        cpu->registers[cpu->bcode[cpu->pc]] = popStack_simple(cpu->stack);\
    }                                                   \
    cpu->pc += sizeof(uint8_t);                         \
}

#define ADD_FUNC                                        \
{                                                       \
    valua_a = popStack_simple(cpu->stack);              \
    value_b = popStack_simple(cpu->stack);              \
                                                        \
    result = valua_a + value_b;                         \
                                                        \
    pushStack_simple(cpu->stack, result);               \
                                                        \
    cpu->pc += sizeof(uint8_t);                         \
                                                        \
}   

#define SUB_FUNC                                        \
{                                                       \
    cpu->pc += sizeof(uint8_t);                         \
                                                        \
    valua_a = popStack_simple(cpu->stack);              \
    value_b = popStack_simple(cpu->stack);              \
                                                        \
    result = value_b - valua_a;                         \
                                                        \
    pushStack_simple(cpu->stack, result);               \
}

#define MUL_FUNC                                        \
{                                                       \
    cpu->pc += sizeof(uint8_t);                         \
                                                        \
    valua_a = popStack_simple(cpu->stack);              \
    value_b = popStack_simple(cpu->stack);              \
                                                        \
    result = valua_a * value_b;                         \
                                                        \
    pushStack_simple(cpu->stack, result);               \
}

#define DIV_FUNC                                        \
{                                                       \
    cpu->pc += sizeof(uint8_t);                         \
                                                        \
    valua_a = popStack_simple(cpu->stack);              \
    value_b = popStack_simple(cpu->stack);              \
                                                        \
    result = value_b / valua_a;                         \
                                                        \
    pushStack_simple(cpu->stack, result);               \
}

#define POW_FUNC                                        \
{                                                       \
    cpu->pc += sizeof(uint8_t);                         \
                                                        \
    valua_a = popStack_simple(cpu->stack);              \
    value_b = popStack_simple(cpu->stack);              \
                                                        \
    result = pow(value_b, valua_a);                     \
                                                        \
    pushStack_simple(cpu->stack,result);                \
}

#define SQRT_FUNC                                       \
{                                                       \
    cpu->pc += sizeof(uint8_t);                         \
                                                        \
    value = popStack_simple(cpu->stack);                \
                                                        \
    result = sqrt(value);                               \
                                                        \
    pushStack_simple(cpu->stack, result);               \
} 

#define SIN_FUNC                                        \
{                                                       \
    cpu->pc += sizeof(uint8_t);                         \
                                                        \
    value = popStack_simple(cpu->stack);                \
                                                        \
    result = sin(value);                                \
                                                        \
    pushStack_simple(cpu->stack, result);               \
} 

#define COS_FUNC                                        \
{                                                       \
    cpu->pc += sizeof(uint8_t);                         \
                                                        \
    value = popStack_simple(cpu->stack);                \
                                                        \
    result = cos(value);                                \
                                                        \
    pushStack_simple(cpu->stack, result);               \
}

#define HLT_FUNC                                        \
{                                                       \
    printf("END OF PROGRAMM.\n");                       \
}
