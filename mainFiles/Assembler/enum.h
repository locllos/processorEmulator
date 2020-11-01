#define DEF_CMD(name, num, arg, ctrl_flow) \
        name##_COMMAND  = num,

typedef enum COMMAND
{
    #include "../commands.h"
} COMMAND;

#undef DEF_CMD