#include "headers/assembler.h"

#define DEF_CMD(name, num, arg, ctrl_flow)                                              \
        if (strcmp(cur_line->cmd, #name) == 0)                                          \
        {                                                                               \
            cmd_num = num;                                                              \
            pushFromOffset(bcode, &cmd_num, sizeof(uint8_t));                           \
            printf("SET COMMAND: %d\n", num);                                           \
            if (arg == 1)                                                               \
            {                                                                           \
                if (oneArgumentProcessing(cur_line, bcode, labels, ctrl_flow, algorithm_pass) != 0)\
                {                                                                       \
                    return 1;                                                           \
                }                                                                       \
                cur_line->pos += cur_line->tmp_pos;                                     \
            }                                                                           \
        }                                                                               \
        else if (isLabel(cur_line))                                                     \
        {                                                                               \
            printf("cur_line: %s|\ncmd: %s\n", cur_line->string, cur_line->cmd);        \
            if (algorithm_pass == 0)                                                    \
            {                                                                           \
                writeLabel(cur_line, bcode, labels);                                    \
            }                                                                           \
        }                                                                               \
        else
    
void Copy(void* value_to, const void* value_from, const int elem_size) 
{
    for (size_t i = 0; i < elem_size; ++i) 
    {
        *((uint8_t*)value_to + i) = *((uint8_t*)value_from + i);
    }
}

Labels* newLabels()
{
    Labels* labels = (Labels*)calloc(1, sizeof(Labels));
    labels->capacity = 10;
    labels->size = 0;
    labels->lb = (Label*)calloc(10, sizeof(Label));

    return labels;
}

void addLabel(Labels* labels, const char* label_name, const elem_t mark)
{
    if (labels->size + 1 > labels->capacity)
    {
        labels->capacity *= 2;

        labels->lb = (Label*)realloc(labels->lb, sizeof(Label) * labels->capacity);
    }
    labels->lb[labels->size].label_name = (char*)calloc(strlen(label_name) + 1, sizeof(char));
    strcpy(labels->lb[labels->size].label_name, label_name);
    labels->lb[labels->size].mark = mark;

    ++labels->size;
}

void destructLabels(Labels* labels)
{
    for (size_t i = 0; i < labels->size; ++i)
    {
        free(labels->lb[i].label_name);
    }
    free(labels->lb);
    free(labels);
}

Label* findLabel(Labels* labels, const char* label_name)
{

    for (size_t i = 0; i < labels->size; ++i)
    {   
        if (strcmp(labels->lb[i].label_name, label_name) == 0)
        {   
            return labels->lb + i;
        }
    }
    return NULL;
}

uint64_t approxLength(const char* filename)
{
	assert(filename);
	
    struct stat* buff = (struct stat*)calloc(1, sizeof(struct stat));

    stat(filename, buff);

	return buff->st_size;
	
}

int64_t indexStringInArray(const char* string, const char* arr[], const size_t length)
{
    for (size_t cur_string = 0; cur_string < length; ++cur_string)
    {
        if (strcmp(string, arr[cur_string]) == 0)
        {
            return cur_string;
        }
    }
    return -1;
}

char* readContents(const char* filename, uint64_t* length)
{
	const uint64_t approx_length = approxLength(filename);

	FILE* file = fopen(filename, "r");

    if (file == NULL)
    {   
        FILE* log_file = fopen("logs.txt", "a");

        fprintf(log_file, "\n====================\n");
        fprintf(log_file, "TIME: %s\n", __TIME__);
        fprintf(log_file, "LINE: %s\n", __LINE__);
        fprintf(log_file, "FILE DOES NOT EXTST!");
        fprintf(log_file, "\n====================\n");

        fclose(log_file);

        return NULL;
    }

	char *buffer = (char*)calloc(approx_length + 1, sizeof(char));

	*length = fread(buffer, sizeof(char), approx_length + 1, file);

	buffer[*length] = '\0';

	fclose(file);
	
	return buffer;
}

Text* parseContents(const char* filename)
{   
    uint64_t length = 0;
    char** buffer = (char**)calloc(1, sizeof(char*));
    uint64_t* number_of_lines = (uint64_t*)calloc(1, sizeof(uint64_t));

	*buffer = readContents(filename, &length);

    if (*buffer == NULL)
    {
        FILE* log_file = fopen("logs.txt", "a");

        fprintf(log_file, "\n=====================\n");
        fprintf(log_file, "TIME: %s\n", __TIME__);
        fprintf(log_file, "LINE: %s\n", __LINE__);
        fprintf(log_file, "BUFFER HAS NULL POINTER!");
        fprintf(log_file, "\n======================\n");

        fclose(log_file);

        return NULL;    }

	*number_of_lines = 0;

	for (int i = 0; i < length; ++i)
		if ((*buffer)[i] == '\n')
			(*number_of_lines)++;
	
	Line* lines = (Line*)calloc(*number_of_lines + 1, sizeof(Line)); 

	int cur_size = 0,
		cur_line = 0;
	for (int i = 0; i < length; ++i)
	{
		if ((*buffer)[i] != '\n')
			++cur_size;
		else
		{
			lines[cur_line].size = cur_size;
			lines[cur_line].string = *buffer + i - cur_size;
			lines[cur_line].string[cur_size] = '\0';
			cur_line++;
			cur_size = 0;
		}
	}

	//На случай если в конце файла не было проставлено символов перехода на следующую строку
	if ((*buffer)[length] != '\n')
	{
		(*buffer)[length] = '\0';
		lines[*number_of_lines].size = cur_size;
		lines[*number_of_lines].string = *buffer + length - cur_size;
		(*number_of_lines)++;
	}

    Text* text = (Text*)calloc(1, sizeof(Text));
    text->buffer = *buffer;
    text->length = length;
    text->lines = lines;
    text->number_of_lines = *number_of_lines;

	return text;
}

void destructText(Text* text)
{

    free(text->lines);

    if (text->buffer != NULL)
    {
        free(text->buffer);
    }
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

void insertTo(BinaryCode* bcode, const void* value, const uint64_t elem_size, const uint64_t index)
{
    Copy(bcode->code + index, value, elem_size);
}

int ctrlFlowProcess(BinaryCode* bcode, Labels* labels, CurLine* cur_line, uint64_t alg_pass)
{
    cur_line->flag = 0x00;  

    printf("FIND CTRL_FLOW.\nSET FLAG: %d\n", cur_line->flag); 

    if (alg_pass == 1)                                            
    {    
        if (sscanf(cur_line->string + cur_line->pos, "%s", cur_line->cmd) == 1)                   
        {                                                               
            printf("TRY TO FIND: %s|\n", cur_line->cmd);                          
            cur_line->cur_label = findLabel(labels, cur_line->cmd);                         
            if (cur_line->cur_label != NULL)                                      
            {                                                           
                printf("FIND LABEL: %s\n", cur_line->cur_label->label_name);      
                printf("SET LABEL NUM: %lg\n", cur_line->cur_label->mark);    

                pushFromOffset(bcode, &cur_line->cur_label->mark, sizeof(elem_t));
            }                                                           
            else                                                        
            {                                                           
                printf("CANNOT FIND LABEL NAME AT LINE %llu:\n->%s\n", cur_line->line, cur_line->cmd);
                return 1;                                               
            }                                                           
        }                                                               
        else                                                            
        {                                                               
            printf("CANNOT READ LABEL NAME AT LINE %llu:\n->%s\n", cur_line->line, cur_line->cmd);
            return 1;                                                   
        }                                                                                                                         
    }                                                                   
    else                                                                
    {
        pushFromOffset(bcode, &DAM, sizeof(elem_t));                    
    }       
    outputBinary(bcode);

    return 0;                                               
}

void movePosAfterOpenBrackets(CurLine* cur_line)
{
    printf("STRING BEFORE OPEN BRACKET: %s\n", cur_line->string + cur_line->pos);

    char* find_brackets = strchr(cur_line->string + cur_line->pos, '[');
    
    cur_line->pos =  (find_brackets + 1 - cur_line->string);
    
    cur_line->flag |= 0x04;

    printf("POS AFTER OPEN BRACKET: %s\n", cur_line->string + cur_line->pos);

}

void movePosAfterAdditioal(CurLine* cur_line)
{   
    printf("STRING BEFORE ADD: %s\n", cur_line->string + cur_line->pos);
    char* find_additional = strchr(cur_line->string + cur_line->pos, '+');
    cur_line->pos =  (find_additional + 1 - cur_line->string);
    printf("POS AFTER ADD: %s\n", cur_line->string + cur_line->pos);

}

int isNumber(CurLine* cur_line)
{
    if (sscanf(cur_line->string + cur_line->pos, "%lg", &cur_line->value) == 1)
    {
        return 1;
    }

    return 0;
}

int isString(CurLine* cur_line)
{
    if (sscanf(cur_line->string + cur_line->pos, "%s", cur_line->reg) == 1)
    {        
        return 1;
    }

    return 0;
}

int isRegister(CurLine* cur_line)
{
    cur_line->reg_index = indexStringInArray(cur_line->cmd, REGISTERS, AMOUNT_REGISTERS);

    return (cur_line->reg_index > -1 && cur_line->reg_index < AMOUNT_REGISTERS);
}

int isLabel(CurLine* cur_line)
{
    return (cur_line->cmd[strlen(cur_line->cmd) - 1] == ':');
}

int isAdditional(CurLine* cur_line)
{
    return (strchr(cur_line->string + cur_line->pos, '+') != NULL);
}

int isOpenBrackets(CurLine* cur_line)
{
    return (cur_line->cmd[0] == '[');
}

int isCloseBrackets(CurLine* cur_line)
{
    return (strchr(cur_line->cmd, ']') != NULL);
}

void readNext(CurLine* cur_line)
{
    int tmp_pos = 0;
    sscanf(cur_line->string + cur_line->pos, "%s%n", cur_line->cmd, &tmp_pos);
    
    cur_line->tmp_pos += tmp_pos;
}

void clearBrackets(char* string)
{
    char* left = string;
    char* right = string + strlen(string) - 1;

    while (left < right)
    {
        if (*left == '[')
        {
            *left = ' ';
        }

        if (*right == ']')
        {
            *right = ' ';
        }

        --right;
        ++left;
    }
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

int registerError(CurLine* cur_line)
{
    printf("ERROR! WRONG REGISTER NAME AT LINE %llu!\n", cur_line->line);

    printf("->%s\n", cur_line->reg);

    return 4;
}

int argumentError(CurLine* cur_line)
{
    printf("ARGUMENT DOESNT FIND AT LINE %llu!\n", cur_line->line); 
    printf("->%s\n", cur_line->cmd);

    return 3;
}

int isCtrlFlow(const uint8_t ctrl_flow)
{
    return (ctrl_flow == 1);
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

int oneArgumentProcessing(CurLine* cur_line, BinaryCode* bcode, Labels* labels, const uint8_t ctrl_flow, const uint8_t alg_pass)
{
    cur_line->flag = 0x00;
    uint64_t flag_address = bcode->offset; 
    pushFromOffset(bcode, &cur_line->flag, sizeof(int8_t));

    if (isCtrlFlow(ctrl_flow))                                          
    {                                                                       
        if (ctrlFlowProcess(bcode, labels, cur_line, alg_pass) != 0)  
        {            
            return 4;                                                       
        }            
        return 0;                                                           
    }                                                                       
    if (isNumber(cur_line))                                             
    {   
        writeNumber(cur_line, bcode);   
        outputBinary(bcode);                                    
    }                                                                       
    else if (isString(cur_line))                                        
    {                             
        readNext(cur_line);
        printf("READ: %s\n", cur_line->cmd);                                      
        if (isOpenBrackets(cur_line))
        {
            movePosAfterOpenBrackets(cur_line);
            readNext(cur_line);
            clearBrackets(cur_line->cmd);
        }
        if (isNumber(cur_line))
        {
            writeNumber(cur_line, bcode);
            if (!isCloseBrackets)
            {
                return argumentError(cur_line);
            }
            // outputBinary(bcode);
        }   
        else if (isRegister(cur_line))                                       
        {   
            writeRegister(cur_line, bcode);

            if (isAdditional(cur_line))
            {
                movePosAfterAdditioal(cur_line);
                if (isNumber(cur_line))
                {
                    writeNumber(cur_line, bcode);
                }
                else
                {
                    return argumentError(cur_line);
                }
                readNext(cur_line);
                if (!isCloseBrackets)
                {
                    return argumentError(cur_line);
                }
            }
            // outputBinary(bcode);
        }                                                        
        else                                                                
        {                                                         
            return argumentError(cur_line);                                 
        }                                                             
    }                                                                       
    else                                                                    
    {                                                                       
        return argumentError(cur_line);                                     
    }

    insertTo(bcode, &cur_line->flag, sizeof(uint8_t), flag_address);
    return 0;
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

CurLine* newCurLine()
{
    CurLine* cur_line = (CurLine*)calloc(1, sizeof(CurLine)); 

    cur_line->cmd = (char*)calloc(32, sizeof(char));
    cur_line->reg = (char*)calloc(8, sizeof(char));
    cur_line->tmp_pos = -1;
    cur_line->pos = 0;
    cur_line->value = 0;
    cur_line->line = 0;
    cur_line->flag = NUMBER;
    cur_line->reg_index = 0;
    cur_line->cur_label = NULL;

    return cur_line;
}

int commentsProcessing(CurLine* cur_line)
{
    char* def_end = strchr(cur_line->string, ';');                                              
    if (def_end != NULL)                                                            
    {    
        if (cur_line->string == def_end)
        {
            return 1;
        }                                                                     
        *def_end = '\0';    
    }
    return 0;
}

int endingsProcess(CurLine* cur_line)
{
    char check_ending = '\0';

    if (sscanf(cur_line->string + cur_line->pos, "[^ \n\t]%c", &check_ending) > 0 && check_ending != '\0')
    {
        sscanf(cur_line->string + cur_line->pos, "%s", cur_line->cmd);
        printf("UNKNOWN COMMAND AT LINE %llu!\n", cur_line->line);
        printf("->%s", cur_line->cmd);
        return 1;
    }


    return 0;
}

void resetCurLine(CurLine* cur_line)
{
    strcpy(cur_line->cmd, "upd_сmd");
    strcpy(cur_line->reg, "upd_reg");
    cur_line->tmp_pos = 0;
    cur_line->pos = 0;
    cur_line->value = 0;
    cur_line->line = 0;
    cur_line->flag = NUMBER;
    cur_line->reg_index = 0;
    cur_line->cur_label = NULL;
}

int codeProcessing(const char* filename, const char* binary_filename)
{   
    Text* code = parseContents(filename);

    if (binary_filename == NULL)
    {
        printf("NULL POINTER FILE NAME!\n");

        return 3;
    }

    BinaryCode* bcode = newBinaryCode(BASE_CAPACITY);
    Labels* labels = newLabels();
    uint64_t amount_lines = code->number_of_lines;
    uint64_t cmd_num = 0;

    CurLine* cur_line = newCurLine();

    printf("{AMOUNT OF LINES: %llu}\n", amount_lines);

    for (uint64_t algorithm_pass = 0; algorithm_pass < NUM_ALGORITHM_PASS; ++algorithm_pass)
    {
        bcode->offset = 0;
        printf("\n======\nALGORITHM PASS #%llu\n=======\n", algorithm_pass + 1);
        for (uint64_t i = 0; i < amount_lines; ++i)
        {   
            
            resetCurLine(cur_line);
            cur_line->string = code->lines[i].string;
            cur_line->line = i + 1;

            //"Delete" comments 
            if (commentsProcessing(cur_line) != 0)
            {
                continue;
            }

            int tmp_pos = 0;
            sscanf(cur_line->string, "%s%n", cur_line->cmd, &tmp_pos);
            cur_line->tmp_pos = tmp_pos;
            
            cur_line->pos += cur_line->tmp_pos;

            #include "commands.h"
            /*else*/
            if (strcmp(cur_line->cmd, "upd_сmd") == 0)
            {
                continue;
            }
            else
            {
                printf("UNKNOWN COMMAND AT LINE %llu!\n", cur_line->line);
                printf("LINE: %s|\n", cur_line->string);
                printf("->%s|\n", cur_line->cmd);
                return 2;
            }

            if (endingsProcess(cur_line) != 0)
            {
                return 2;
            }

        }
    }

    writeToBinary(bcode, binary_filename);
    destructText(code);

    return 0;
}

int main(const int argC, const char* argV[])
{
    const char* default_filename = "my_code.txt";
    const char* default_bin_filename = "bc.bin";

    printf("ARG C: %d\n", argC);
    for (int i = 0; i < argC; ++i)
    {
        printf("ARG V[%i] = %s\n", i, argV[i]);
    }

    if (argC == 1)
    {
        return codeProcessing(default_filename, default_bin_filename);
    }
    if (argC == 2)
    {   
        return codeProcessing(argV[1], default_bin_filename);
    }
    if (argC == 4 && strcmp(argV[2], "~") == 0)
    {
        return codeProcessing(argV[1], argV[3]);
    }
    else
    {
        printf("UNKNOWN COMMANDS!\n");

        return 1;
    }
}
