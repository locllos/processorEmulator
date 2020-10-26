#define PC_INCR_1 cpu->pc += sizeof(uint8_t)

#define PC_INCR_8 cpu->pc += sizeof(elem_t)

#define TAKE_VALUE_FROM_FLOW value = *((elem_t*)(cpu->bcode + cpu->pc))

#define TAKE_VALUE_FROM_REGS value = cpu->registers[cpu->bcode[cpu->pc]]

#define TAKE_ADDRESS_FROM_FLOW cpu->pc = (uint64_t)(*((elem_t*)(cpu->bcode + cpu->pc)))

#define TAKE_ADDRESS_FROM_REGS cpu->pc = (uint64_t)REGISTERS[cpu->bcode[cpu->pc]]

#define TAKE_ADDRESS_FROM_STACK cpu->pc = popStack_simple(cpu->call_stack)

#define PUSH_VALUE pushStack_simple(cpu->stack, value)

#define PUSH_RESULT pushStack_simple(cpu->stack, result)

#define ERASE eraseStack_simple(cpu->stack)

#define POP_TO_REGS cpu->registers[cpu->bcode[cpu->pc]] = popStack_simple(cpu->stack)

#define POP_TO_VALUE(value) value = popStack_simple(cpu->stack)

#define ADD_VALUES result = value_a + value_b

#define SUB_VALUES result = value_b - value_a

#define MUL_VALUES result = valua_b * value_a

#define DIV_VALUES result = value_b / value_a

#define POW_VALUES result = pow(value_b, value_a)

#define SQRT_VALUE result = sqrt(value)

#define SIN_VALUE result = sin(value)

#define COS_VALUE result = cos(value)

#define PUSH_ADDRESS pushStack_simple(cpu->call_stack, cpu->pc + sizeof(elem_t))

#define SCAN_VALUE scanf("%lg", &value)

#define PRINT_TOP printf("%lg\n", topStack_simple(cpu->stack)) 

#define PRINT_END printf("END OF PROGRAMM.\n")