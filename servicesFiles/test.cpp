#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


//#define CONSOLE_DEBUG_FOR_NOODLES 

#ifdef CONSOLE_DEBUG_FOR_NOODLES
#define HOLD_PROGRAMM                                                           \
    char i_hope_there_is_no_human_who_want_to_name_variables_like_this = getchar();
#else
#define HOLD_PROGRAMM {}
#endif

const int length = 12;

const char* OPERATIONS[length] = 
{
    "IN",
    "OUT",
    "POP",
    "POW",
    "SUB",
    "ADD",
    "MUL",
    "SIN",
    "COS",
    "HLT",
    "PUSH",
    "SQRT"
};

int main()
{   
 
    for (int i = 0; i < 10; ++i) printf("ITS WOOOOOOOOOOOOOORK!\n");
    int pos_a = -1;
    int pos_b = -1;
    int number = 0;
    char* string = (char*)calloc(32, sizeof(char));

    char* buffer = (char*)calloc(32, sizeof(char));

    strcpy(buffer, "  Magic thatsCool\n");
    printf("BUFFER: %s\n", buffer);

    printf("AMOUNT OF READ VARIABLES: %d\n", sscanf(buffer, " %s%n", string, &pos_b));

    printf("POS A: %d\n", pos_a);
    printf("POS B: %d\n", pos_b);

    printf("STRING: ");
    printf("%s\n", string); 
    printf("A REST OF STRING:\n%s\n", buffer+pos_b);

    int tmp = 0;
    int readalbe = sscanf(buffer + pos_b, " %d %n", &number, &tmp);
    pos_b += tmp;
    printf("CAN READ: %d\n", readalbe);
    printf("POSSIBLE NUMBER: %d\n", number);


    if (readalbe == 1)
    {
        printf("READ!\n");
        printf("POS A: %d\n", pos_a);
        printf("POS B: %d\n", pos_b);

        printf("%d\n", number);

        printf("AMOUNT OF READ VARIABLES: %d\n", sscanf(buffer + pos_b, " %s %n", string, &tmp));
        pos_b += tmp;

        printf("POS A: %d\n", pos_a);
        printf("POS B: %d\n", pos_b);

        printf("STRING: ");
        printf("%s\n", string); 
        printf("A REST OF STRING:\n%s\n", buffer+pos_b);
    }
    else
    {
        printf("DOESN'T READ!\n");
        printf("POS B: %d\n", pos_b);

    }


    FILE* file = fopen("try.bin", "wb");

    fwrite(&number, sizeof(int), 1, file);

    fclose(file);

    //system("pause");
    /*
    fFILE* ile_ = fopen("logs.txt", "r");

    char* buffer = (char*)calloc(32, sizeof(char));
    fread(buffer, sizeof(char), 32, file);

    printf("%s", buffer);

    fclose(file);
    */

}