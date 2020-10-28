 #include "fast_func.h"
 
 #define IN_FUNC                                        \
    {                                                   \
    SCAN_VALUE;                                         \
                                                        \
    PUSH_VALUE;                                         \
                                                        \
    PC_INCR_1;                                          \
    }

#define OUT_FUNC                                        \
{                                                       \
    PRINT_TOP;                                          \
                                                        \
    PC_INCR_1;                                          \
}

#define PUSH_FUNC                                       \
{                                                       \
    PC_INCR_1;                                          \
    HOLD_FLAG;                                          \
    if (IS_RAM_FLAG)                                    \
    {                                                   \
        RAM_PROCESSING_PUSH;                            \
    }                                                   \
    else                                                \
    {                                                   \
        DEFUALT_PROCCESSING_PUSH;                       \
    }                                                   \
    PUSH_VALUE;                                         \
}

#define POP_FUNC                                        \
{                                                       \
    PC_INCR_1;                                          \
    HOLD_FLAG;                                          \
    if (IS_RAM_FLAG)                                    \
    {                                                   \
        RAM_PROCESSING_POP;                             \
    }                                                   \
    else                                                \
    {                                                   \
        DEFUALT_PROCCESSING_POP;                        \
    }                                                   \
}

#define ADD_FUNC                                        \
{                                                       \
    POP_TO_VALUE(value_a);                              \
    POP_TO_VALUE(value_b);                              \
    ADD_VALUES;                                         \
    PUSH_RESULT;                                        \
    PC_INCR_1;                                          \
                                                        \
}   

#define SUB_FUNC                                        \
{                                                       \
    PC_INCR_1;                                          \
    POP_TO_VALUE(value_a);                              \
    POP_TO_VALUE(value_b);                              \
                                                        \
    SUB_VALUES;                                         \
                                                        \
    PUSH_RESULT;                                        \
}

#define MUL_FUNC                                        \
{                                                       \
    PC_INCR_1;                                          \
                                                        \
    POP_TO_VALUE(value_a);                              \
    POP_TO_VALUE(value_b);                              \
                                                        \
    MUL_VALUES;                                         \
                                                        \
    PUSH_RESULT;                                        \
}

#define DIV_FUNC                                        \
{                                                       \
    PC_INCR_1;                                          \
                                                        \
    POP_TO_VALUE(value_a);                              \
    POP_TO_VALUE(value_b);                              \
                                                        \
    DIV_VALUES;                                         \
                                                        \
    PUSH_RESULT;                                        \
}

#define POW_FUNC                                        \
{                                                       \
    PC_INCR_1;                                          \
                                                        \
    POP_TO_VALUE(value_a);                              \
    POP_TO_VALUE(value_b);                              \
                                                        \
    POW_VALUES;                                         \
                                                        \
    PUSH_RESULT;                                        \
}

#define SQRT_FUNC                                       \
{                                                       \
    PC_INCR_1;                                          \
                                                        \
    POP_TO_VALUE(value);                                \
                                                        \
    SQRT_VALUE;                                         \
                                                        \
    PUSH_RESULT;                                        \
} 

#define SIN_FUNC                                        \
{                                                       \
    PC_INCR_1;                                          \
                                                        \
    POP_TO_VALUE(value);                                \
                                                        \
    SIN_VALUE;                                          \
                                                        \
    PUSH_RESULT;                                        \
} 

#define COS_FUNC                                        \
{                                                       \
    PC_INCR_1;                                          \
                                                        \
    POP_TO_VALUE(value);                                \
                                                        \
    COS_VALUE;                                          \
                                                        \
    PUSH_RESULT;                                        \
}

#define HLT_FUNC                                        \
{                                                       \
    PRINT_END;                                          \
    CPU_EXIT;                                           \
}

#define JMP_FUNC                                        \
{                                                       \
    PC_INCR_1;                                          \
    PC_INCR_1;                                          \
    TAKE_ADDRESS_FROM_FLOW;                             \
}                                                       

#define JB_FUNC                                         \
{                                                       \
    PC_INCR_1;                                          \
    POP_TO_VALUE(value_a);                              \
    POP_TO_VALUE(value_b);                              \
    PC_INCR_1;                                          \
    if (value_a > value_b)                              \
    {                                                   \
        TAKE_ADDRESS_FROM_FLOW;                         \
    }                                                   \
    else                                                \
    {                                                   \
        PC_INCR_8;                                      \
    }                                                   \
}                                         

#define JBE_FUNC                                        \
{                                                       \
    PC_INCR_1;                                          \
    POP_TO_VALUE(value_a);                              \
    POP_TO_VALUE(value_b);                              \
    PC_INCR_1;                                          \
    if (value_a >= value_b)                              \
    {                                                   \
        TAKE_ADDRESS_FROM_FLOW;                         \
    }                                                   \
    else                                                \
    {                                                   \
        PC_INCR_8;                                      \
    }                                                  \
}                                                       

#define JE_FUNC                                         \
{                                                       \
    PC_INCR_1;                                          \
    POP_TO_VALUE(value_a);                              \
    POP_TO_VALUE(value_b);                              \
    PC_INCR_1;                                          \
    if (value_a == value_b)                              \
    {                                                   \
        TAKE_ADDRESS_FROM_FLOW;                         \
    }                                                   \
    else                                                \
    {                                                   \
        PC_INCR_8;                                      \
    }                                                   \
}                   

#define JNE_FUNC                                        \
{                                                       \
    PC_INCR_1;                                          \
    POP_TO_VALUE(value_a);                              \
    POP_TO_VALUE(value_b);                              \
    PC_INCR_1;                                          \
    if (value_a != value_b)                              \
    {                                                   \
        TAKE_ADDRESS_FROM_FLOW;                         \
    }                                                   \
    else                                                \
    {                                                   \
        PC_INCR_8;                                      \
    }                                                   \
}  

#define JA_FUNC                                         \
{                                                       \
    PC_INCR_1;                                          \
    POP_TO_VALUE(value_a);                              \
    POP_TO_VALUE(value_b);                              \
    PC_INCR_1;                                          \
    if (value_a < value_b)                              \
    {                                                   \
        TAKE_ADDRESS_FROM_FLOW;                         \
    }                                                   \
    else                                                \
    {                                                   \
        PC_INCR_8;                                      \
    }                                                   \
}

#define JAE_FUNC                                        \
{                                                       \
    PC_INCR_1;                                          \
    POP_TO_VALUE(value_a);                              \
    POP_TO_VALUE(value_b);                              \
    PC_INCR_1;                                          \
    if (value_a <= value_b)                             \
    {                                                   \
        TAKE_ADDRESS_FROM_FLOW;                         \
    }                                                   \
    else                                                \
    {                                                   \
        PC_INCR_8;                                      \
    }                                                   \
}

#define JT_FUNC                                         \
{                                                       \
    PC_INCR_1;                                          \
                                                        \
    struct tm time_in = { 0, 0, 0, 9, 10, 2016 - 1900 };\
    time_t time_temp = mktime(&time_in);                \
    struct tm* time_out = localtime(&time_temp);        \
                                                        \
    if (cpu->bcode[cpu->pc] == NUMBER)                  \
    {                                                   \
        PC_INCR_1;                                      \
                                                        \
        if (time_out->tm_wday == 2)                     \
        {                                               \
            TAKE_ADDRESS_FROM_FLOW;                     \
        }                                               \
        else                                            \
        {                                               \
            PC_INCR_8;                                  \
        }                                               \
    }                                                   \
    else if (cpu->bcode[cpu->pc] == REGISTER)           \
    {                                                   \
        PC_INCR_1;                                      \
                                                        \
        if (time_out->tm_wday == 2)                     \
        {                                               \
            TAKE_ADDRESS_FROM_REGS;                     \
        }                                               \
        else                                            \
        {                                               \
            PC_INCR_1;                                  \
        }                                               \
    }                                                   \
                                                        \
    free(time_out);                                     \
}   

#define CALL_FUNC                                       \
{                                                       \
    PC_INCR_1;                                          \
    PC_INCR_1;                                          \
                                                        \
    PUSH_ADDRESS;                                       \
                                                        \
    TAKE_ADDRESS_FROM_FLOW;                             \
}

#define RET_FUNC                                        \
{                                                       \
    TAKE_ADDRESS_FROM_CALLSTACK;                            \
}