#include "headers/assembler.h"

void Copy(void* value_to, const void* value_from, const int elem_size) 
{
    for (size_t i = 0; i < elem_size; ++i) 
    {
        *((uint8_t*)value_to + i) = *((uint8_t*)value_from + i);
    }
}

void insertTo(BinaryCode* bcode, const void* value, const uint64_t elem_size, const uint64_t index)
{
    Copy(bcode->code + index, value, elem_size);
}

BinaryCode* newBinaryCode(const size_t start_capacity)
{
    BinaryCode* bcode = (BinaryCode*)calloc(1, sizeof(BinaryCode));
    bcode->code = (uint8_t*)calloc(start_capacity, sizeof(uint8_t));
    bcode->capacity = start_capacity;   
    bcode->length = 0;
    bcode->offset = 0;

    return bcode;
}

void capacityProcessing(BinaryCode* bcode, const size_t elem_size)
{
    while (bcode->length + elem_size > bcode->capacity)
    {
        bcode->capacity = bcode->capacity * 2;
    }

    //printf("BCODE->CAPACITY: %llu\n", bcode->capacity);
    uint8_t* new_region = (uint8_t*)realloc(bcode->code, sizeof(uint8_t) * bcode->capacity);
    assert(new_region != NULL);
    bcode->code = new_region;
    //printf("BCODE = %p\n", bcode->code);

}

void pushFromOffset(BinaryCode* bcode, const void* elem, size_t elem_size)
{   
    capacityProcessing(bcode, elem_size);
    if (bcode->offset > bcode->length)
    {
        printf("OFFSET GREATER THAN LENGTH!\n");
        assert(bcode->offset <= bcode->length);
    }
    
    if (bcode->offset + elem_size <= bcode->length)
    {   
        Copy(bcode->code + bcode->offset, elem, elem_size);
    }
    else
    {    
        Copy(bcode->code + bcode->offset, elem, elem_size);

        bcode->length += elem_size - (bcode->length - bcode->offset);
    }
    bcode->offset += elem_size;
    
    /*
    printf("============\n");
    printf("OFFSET: %llu\n", bcode->offset);
    printf("CAPACITY: %llu\n", bcode->capacity);
    printf("LENGTH: %llu\n", bcode->length);
    printf("ELEM_SIZE: %llu\n", elem_size);
    printf("ELEM VALUE: %lg\n", *(elem_t*)elem);
    printf("BCODE->CODE = %p\n", bcode->code);

    for (size_t i = 0; i < bcode->length; ++i)
    {
        printf("%02X ", bcode->code[i]);
    }
    printf("\n\n");
    */
    
}

void writeNumber(CurLine* cur_line, BinaryCode* bcode)
{   
    sscanf(cur_line->string + cur_line->pos, "%lg%n", &cur_line->value, &cur_line->tmp_pos);
    cur_line->flag |= 0x01;        
    //cur_line->flag = (cur_line->flag & 0x01) ? cur_line->flag : cur_line->flag + 0x01;   

    printf("FIND NUMBER.\nSET FLAG: %d\n", cur_line->flag);                       
    //pushFromOffset(bcode, &cur_line->flag, sizeof(uint8_t));                     
    printf("VALUE OF NUMBER: %lg\n", cur_line->value);                     
    pushFromOffset(bcode, &cur_line->value, sizeof(elem_t));  
}


void writeRegister(CurLine* cur_line, BinaryCode* bcode)
{
    //cur_line->flag = REGISTER; 
    cur_line->flag |= 0x02;                                                       
    printf("FIND REGISTER.\nSET FLAG: %d\nREGISTER ADDRESS: %s\n", cur_line->flag, cur_line->reg);
    //pushFromOffset(bcode, &cur_line->flag, sizeof(uint8_t));          
    pushFromOffset(bcode, &cur_line->reg_index, sizeof(int8_t));     
}

void writeLabel(CurLine* cur_line, BinaryCode* bcode, Labels* labels)
{
    cur_line->cmd[strlen(cur_line->cmd) - 1]  = '\0';                                           
    printf("FIND LABEL: %s\n", cur_line->cmd);      
    addLabel(labels, cur_line->cmd, bcode->offset);                                   
}

void destructBinaryCode(BinaryCode* bcode)
{
    free(bcode->code);
    free(bcode);
}

void writeToBinary(BinaryCode* bcode, const char* filename)
{
    FILE* binary_file = fopen(filename, "wb");

    fwrite(bcode->code, sizeof(uint8_t), bcode->length, binary_file);

    fclose(binary_file);
}

void outputBinary(BinaryCode* bcode)
{
    printf("BINARY CODE:\n");
    for (size_t i = 0; i < bcode->length; ++i)
    {
        printf("%02X ", bcode->code[i]);
        
        if (i % 16 == 0 && i != 0) 
            printf("\n");
    }
    printf("\n");
}



