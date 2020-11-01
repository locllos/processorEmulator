// Первый аргумент название команды
// Второй аргумет номер команды в бинарном коде
// Третий аргумент количество принимаемых аргументов
// Четвёртый аргумент: 0 - команда не меняет порядок выполнения, 1 - команда меняет порядок выполения

DEF_CMD(IN, 1, 0, 0)

DEF_CMD(OUT, 2, 0, 0)

DEF_CMD(PUSH, 3, 1, 0)

DEF_CMD(POP, 4, 1, 0)

DEF_CMD(ADD, 5, 0, 0)

DEF_CMD(SUB, 6, 0, 0)

DEF_CMD(MUL, 7, 0, 0)

DEF_CMD(DIV, 8, 0, 0)

DEF_CMD(POW, 9, 0, 0)

DEF_CMD(SQRT, 10, 0, 0)

DEF_CMD(SIN, 11, 0, 0)

DEF_CMD(COS, 12, 0, 0)

DEF_CMD(HLT, 13, 0, 0)

DEF_CMD(JMP, 14, 1, 1)

DEF_CMD(JB, 15, 1, 1)

DEF_CMD(JBE, 16, 1, 1)

DEF_CMD(JE, 17, 1, 1)

DEF_CMD(JAE, 18, 1, 1)

DEF_CMD(JA, 19, 1, 1)

DEF_CMD(JNE, 20, 1, 1)

DEF_CMD (JT, 21, 1, 1)

DEF_CMD(CALL, 22, 1, 1)

DEF_CMD(RET, 23, 0, 1)

/*
DEF_CMD(PIX, 24, 1, 0)

DEF_CMD(UNPIX, 25, 0, 0)

DEF_CMD(SHOW, 26, 0, 0)

DEF_CMD(UNSHOW, 27, 0, 0)
*/