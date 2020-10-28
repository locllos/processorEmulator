#define PC_INCR_1 cpu->pc += sizeof(uint8_t)

#define PC_DERC_1 cpu->pc -= sizeof(uint8_t)

#define PC_INCR_8 cpu->pc += sizeof(elem_t)

#define TAKE_VALUE_FROM_FLOW value = *((elem_t*)(cpu->bcode + cpu->pc))

#define TAKE_VALUE_FROM_REGS value = cpu->registers[cpu->bcode[cpu->pc]]

#define TAKE_VALUE_FROM_RAM value = cpu->RAM[ram_address]

#define HOLD_FLAG flag = cpu->bcode[cpu->pc]; cpu->pc += sizeof(uint8_t)

#define TAKE_ADDRESS_FROM_FLOW cpu->pc = (uint64_t)(*((elem_t*)(cpu->bcode + cpu->pc)))

#define TAKE_ADDRESS_FROM_REGS cpu->pc = (uint64_t)cpu->registers[cpu->bcode[cpu->pc]]

#define TAKE_ADDRESS_FROM_CALLSTACK cpu->pc = popStack_simple(cpu->call_stack)

#define TAKE_RAM_ADDRESS_FROM_STACK ram_address += (uint64_t)(*((elem_t*)(cpu->bcode + cpu->pc)))

#define TAKE_RAM_ADDRESS_FROM_FLOW ram_address += (uint64_t)(*((elem_t*)(cpu->bcode + cpu->pc)))

#define TAKE_RAM_ADDRESS_FROM_REGS ram_address += (uint64_t)cpu->registers[cpu->bcode[cpu->pc]]

#define PUSH_VALUE pushStack_simple(cpu->stack, value)

#define PUSH_RESULT pushStack_simple(cpu->stack, result)

#define ERASE eraseStack_simple(cpu->stack)

#define POP_TO_REGS cpu->registers[cpu->bcode[cpu->pc]] = popStack_simple(cpu->stack)

#define POP_TO_VALUE(value) value = popStack_simple(cpu->stack)

#define POP_TO_RAM cpu->RAM[ram_address] = value

#define ADD_VALUES result = value_a + value_b

#define SUB_VALUES result = value_b - value_a

#define MUL_VALUES result = value_b * value_a

#define DIV_VALUES result = value_b / value_a

#define POW_VALUES result = pow(value_b, value_a)

#define SQRT_VALUE result = sqrt(value)

#define SIN_VALUE result = sin(value)

#define COS_VALUE result = cos(value)

#define PUSH_ADDRESS pushStack_simple(cpu->call_stack, cpu->pc + sizeof(elem_t))

#define SCAN_VALUE scanf("%lg", &value)

#define PRINT_TOP printf("%lg\n", topStack_simple(cpu->stack))

#define PRINT_END printf("END OF PROGRAMM.\n")

#define CPU_EXIT exit(0);

#define IS_CTRL_FLOW_FLAG flag

#define IS_NUMBER_FLAG (flag & 0x01) != 0

#define IS_REGISTER_FLAG (flag & 0x02) != 0

#define IS_RAM_FLAG (flag & 0x04) != 0

#define RAM_PROCESSING_PUSH \
    cpuDamp(cpu);\
    if (IS_REGISTER_FLAG)   \
    {                       \
        TAKE_RAM_ADDRESS_FROM_REGS;\
        PC_INCR_1;          \
    }                       \
    cpuDamp(cpu);\
    if (IS_NUMBER_FLAG)     \
    {                       \
        TAKE_RAM_ADDRESS_FROM_FLOW;\
        PC_INCR_8;          \
    }                       \
    TAKE_VALUE_FROM_RAM     


#define DEFUALT_PROCCESSING_PUSH\
    if (IS_REGISTER_FLAG)   \
    {                       \
        TAKE_VALUE_FROM_REGS;\
        PC_INCR_1;          \
    }                       \
    else if (IS_NUMBER_FLAG)     \
    {                       \
        TAKE_VALUE_FROM_FLOW;\
        PC_INCR_8;          \
    }       


#define RAM_PROCESSING_POP  \
    if (IS_REGISTER_FLAG)   \
    {                       \
        TAKE_RAM_ADDRESS_FROM_REGS;\
        PC_INCR_1;          \
    }                       \
    if (IS_NUMBER_FLAG)     \
    {                       \
        TAKE_RAM_ADDRESS_FROM_FLOW;\
        PC_INCR_8;          \
    }                       \
    POP_TO_VALUE(value);    \
    POP_TO_RAM              \


#define DEFUALT_PROCCESSING_POP \
    if (IS_REGISTER_FLAG)   \
    {                       \
        TAKE_VALUE_FROM_REGS;\
        POP_TO_REGS;        \
        PC_INCR_1;    \
    }
