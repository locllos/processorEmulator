#include "Assembler.h"

#include "enum.h"

const uint8_t AMOUNT_COMMANDS = 14;
const uint8_t AMOUNT_REGISTERS = 4;
const uint8_t BASE_CAPACITY = 10;
const uint64_t NUM_ALGORITHM_PASS = 2;
const elem_t DAM = -1;
const size_t AMOUNT_PARTSYMBOLS = 3;

const char* REGISTERS_STRING = "[rax] [rbx] [rcx] [rdx]";

const char PARTSYMBOLS[] = " \t\n";
const char VALID_SYMBOLS[] = ".+-";

const char* REGISTERS[AMOUNT_REGISTERS] = 
{   
    "rax",
    "rbx",
    "rcx",
    "rdx"
};

//SERVICE FUNCS:
uint64_t approxLength(const char* filename)
{
	assert(filename);
	
    struct stat* buff = (struct stat*)calloc(1, sizeof(struct stat));

    stat(filename, buff);

	return buff->st_size;	
}

void clearCloseBracket(char* string)
{
    char* close_bracket = strchr(string, ']');

    if (close_bracket != NULL)
        *close_bracket = '\0';
}

int indexStringInArray(char* string, const char* arr[], const size_t length)
{
    clearCloseBracket(string);
    for (size_t cur_string = 0; cur_string < length; ++cur_string)
        if (strcmp(string, arr[cur_string]) == 0)
            return cur_string;
    
    return -1;
}

void Copy(void* value_to, const void* value_from, const size_t elem_size) 
{
    for (size_t i = 0; i < elem_size; ++i) 
        *((uint8_t*)value_to + i) = *((uint8_t*)value_from + i);
}

void copyToData(void* array, const void* value, const size_t elem_size, const size_t offset)
{
    //Предполагается, что массив состоит из элементов одного типа размера elem_size
    //А это значит, что использовать эту функцию для копирования значений в массив с 
    //неравномерным кодом нет смысла
    Copy((void*)((uint8_t*)array + elem_size * offset), value, elem_size);
}

void* updData(void* data, size_t* capacity, const size_t size, const size_t elem_size)
{
    void* region = data;
    if (size + 1 > *capacity)
    {
        *capacity = (*capacity) * 2;

        region = realloc(data, (*capacity) * elem_size);
        assert(region != NULL);
    }

    return region;
}

char* readNext(char* string)
{
    static char* residual = NULL;
    
    if (string == NULL && residual == NULL)
    {
        return NULL;
    }
    
    if (string != NULL)
    {
        residual = strtok(string, " \t\n");
    }
    //Если перед словом стоят символы разделители, мы просто их пропускаем,
    //хотя это можно было продолжать обрабатывать strtok'ом, но лишний раз
    //изменять строку не хотелось бы, как и не хотелось бы, чтобы у стртока 
    //сбрасывался сдвиг
    while (residual != NULL && strchr(" \n\t", *residual) != NULL)
    {
        ++residual; 
    }
    string = residual;
    residual = strtok(NULL, " \t\n");
    
    return string;
}

int isValidSymbol(char symbol)
{
    return (strchr(VALID_SYMBOLS, symbol) != NULL);
}

int isNumber(char* string)
{
    if (string == NULL)
        return 0;

    while(*string != '\0' && *string != ']')
    {
        if (!isdigit(*string) && !isValidSymbol(*string))
        {
            // printf("not a number: %c\n", *string);
            return 0;
        }
        ++string;
    }
    return 1;
}

int isRegister(char* string)
{   
    if (string == NULL)
        return 0;

    return (strstr(REGISTERS_STRING, string) != NULL);
}

int isLabel(char* string)
{
    if (string == NULL)
        return 0;
    
    return (string[strlen(string) - 1] == ':');
}

int isCtrlFlow(int ctrl_flow)
{
    return (ctrl_flow == 1);
}

int isAdditional(AsmLine* line, size_t pos)
{    
    if (pos >= line->amount_lexemes - 1)
        return 0;

    return (strchr(line->lexemes[pos], '+') != NULL);
}

int isOpenBracket(char* string)
{
    if (string == NULL)
        return 0;
    
    return (*string == '[');
}

int isCloseBracketInTheEnd(char* string)
{
    if (string == NULL)
        return 0;

    return (string[strlen(string) - 1] == ']');
}

void skipSpaceSymbols(char** string)
{
    char* cur_char = *string;
    while (*cur_char != '\0' && (isspace(*cur_char)))
        ++cur_char;
    
    *string = cur_char;
}

void jumpOverValidSymbols(char** string, bool* symbol_pass)
{
    char* cur_char = *string;
    while (*cur_char != '\0' && (!isspace(*cur_char)))
    {
        ++cur_char;
        *symbol_pass = true;
    }
    *string = cur_char;
}

size_t getAmountLexemes(char* string)
{
    size_t amount_lexemes = 0;
    bool symbol_pass = false;

    while (*string != '\0')
    {
        //Пропустить пробелы или табы перед лексемой
        symbol_pass = false;
        skipSpaceSymbols(&string);
        //Ищем символы разделители после лексемы
        jumpOverValidSymbols(&string, &symbol_pass);
        //Обрабытваем случай, если строка состояла исключительно из табов или пробелом
        if (symbol_pass == true)
            ++amount_lexemes;
    }
    return amount_lexemes;
}

char* nextLexeme(AsmLine* asm_line, size_t* pos)
{   
    *pos += 1;

    return asm_line->lexemes[*pos];
}

void outputMyBuffer(AsmLine* line)
{
    for (size_t i = 0; i < line->length; ++i)
    {
        if (line->buffer[i] != '\0') 
            printf("%c", line->buffer[i]);
        else printf(" ");
    }
}

void labelError(AsmLine* line)
{
    printf("LABEL ERROR!\n");
    outputMyBuffer(line);
    printf("\n");
}

int argumentError(AsmLine* line, size_t pos, size_t num_line)
{
    printf("UNKNOWN COMMAND AT LINE %zu!\n", num_line);
    printf("Amount of lexemes: %zu\n", line->amount_lexemes);
    printf("Cur pos: %zu\n", pos);
    outputMyBuffer(line);
    if (pos < line->amount_lexemes)
        printf("\n->%s|\n", line->lexemes[pos]);

    return 1;
}

int tooManyArgumentsError(AsmLine* line, size_t pos, size_t num_line)
{
    printf("Too many arguments\n");
    return argumentError(line, pos + 1, num_line);
}

//LINE:
void constructAsmLine(AsmLine* line, char* string, size_t size)
{
    line->length = size;
    line->amount_lexemes = getAmountLexemes(string);
    line->lexemes = (char**)calloc(line->amount_lexemes, sizeof(char*));
    line->buffer = string;

    char* lexeme = readNext(string);

    size_t j = 0;
    while (lexeme != NULL)
    {   
        line->lexemes[j++] = lexeme;

        lexeme = readNext(NULL);
    }
}

void destructAsmLine(AsmLine* asm_line)
{
    free(asm_line->lexemes);
}

AsmLine* newAsmLine(char* string, size_t size)
{
    AsmLine* asm_line = (AsmLine*)calloc(1, sizeof(AsmLine));

    constructAsmLine(asm_line, string, size);

    return asm_line;
}

AsmLine* deleteAsmLine(AsmLine* asm_line)
{
    destructAsmLine(asm_line);
    free(asm_line);
    
    return NULL;
}


//ASM LISTING:
void constructAsmListing(AsmListing* asm_listing, char* buffer, size_t length)
{
    size_t number_of_lines = 0;
	for (int i = 0; i < length; ++i)
		if (buffer[i] == '\n')
			number_of_lines++;

	AsmLine* lines = (AsmLine*)calloc(number_of_lines + 1, sizeof(AsmLine)); 
	int cur_size = 0;
	int	cur_line = 0;
	for (int i = 0; i < length; ++i)
	{
		if (buffer[i] != '\n')
			++cur_size;
		else
		{   
            buffer[i] = '\0';
            constructAsmLine(&lines[cur_line], buffer + i - cur_size, cur_size);
			cur_line++;
			cur_size = 0;
		}
	}
	if (buffer[length - 1] != '\n')
	{
		buffer[length] = '\0';
        constructAsmLine(&lines[number_of_lines], buffer + length - cur_size, cur_size);
		number_of_lines++;
	}
    asm_listing->main_buffer = buffer;
    asm_listing->length = length;
    asm_listing->asm_lines = lines;
    asm_listing->amount_lines = number_of_lines;
}

AsmListing* newAsmListing(char* buffer, size_t length)
{
    AsmListing* asm_listing = (AsmListing*)calloc(1, sizeof(AsmListing));

    constructAsmListing(asm_listing, buffer, length);

    return asm_listing;
}

char* readContents(const char* filename, size_t* length)
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

	char* buffer = (char*)calloc(approx_length + 1, sizeof(char));

	*length = fread(buffer, sizeof(char), approx_length + 1, file);

	buffer[*length] = '\0';

	fclose(file);
	
	return buffer;
}

void showAsmListing(AsmListing* asm_listing)
{
    printf("Amount lines: %zu\n", asm_listing->amount_lines);
    for (size_t i = 0; i < asm_listing->amount_lines; ++i)
    {
        printf("Amount lexems: %zu\n", asm_listing->asm_lines[i].amount_lexemes);
        printf("Parsed on: \n");

        for (size_t j = 0; j < asm_listing->asm_lines[i].amount_lexemes; ++j)
        {
            printf("|%s| ", asm_listing->asm_lines[i].lexemes[j]);
        }
        printf("\n-------\n");
    }
}

void fillAsmListing(AsmListing* asm_listing, const char* filename)
{
    size_t length = 0;
    char* buffer = readContents(filename, &length);

    constructAsmListing(asm_listing, buffer, length);
}

void destructAsmListing(AsmListing* asm_listing)
{
    free(asm_listing->main_buffer);
    for (size_t i = 0; i < asm_listing->amount_lines; ++i)
    {
        destructAsmLine(&asm_listing->asm_lines[i]);
    }
    free(asm_listing->asm_lines);
}

AsmListing* deleteAsmListing(AsmListing* asm_listing)
{
    destructAsmListing(asm_listing);
    free(asm_listing);

    return NULL;
}


//LABELS:
void constructLabels(Labels* labels) 
{
    labels->capacity = BASE_CAPACITY;
    labels->size = 0;
    labels->lb = (Label*)calloc(BASE_CAPACITY, sizeof(Label));
}

Labels* newLabels()
{
    Labels* labels = (Labels*)calloc(1, sizeof(Labels));

    constructLabels(labels);

    return labels;
}

Label* findLabel(Labels* labels, const char* label_name)
{

    if (label_name == NULL)
    {
        return NULL;
    }

    for (size_t i = 0; i < labels->size; ++i)
    {   
        if (strcmp(labels->lb[i].label_name, label_name) == 0)
        {   
            return labels->lb + i;
        }
    }
    return NULL;
}

void addLabel(Labels* labels, const char* label_name, const elem_t mark)
{
    labels->lb = (Label*)updData(labels->lb, &labels->capacity, labels->size, sizeof(Label));

    size_t lbl_len = strlen(label_name);
    labels->lb[labels->size].label_name = (char*)calloc(lbl_len, sizeof(char));
    strcpy(labels->lb[labels->size].label_name, label_name);
    labels->lb[labels->size].label_name[lbl_len - 1] = '\0'; 
    labels->lb[labels->size].mark = mark;

    ++labels->size;
}

void pushLabel(char* string, BinaryCode* bcode, Labels* labels)
{
    printf("FIND LABEL: %s\n", string);
    printf("LABEL OFFSET: %lg\n", (elem_t)bcode->offset);

    addLabel(labels, string, (elem_t)bcode->offset);                                   
}

void showLabels(Labels* labels)
{
    printf(":::::::::::::::::::::\n");
    printf("Showing labels:\n");
    for (size_t i = 0; i < labels->size; ++i)
    {
        printf("%s|\n", labels->lb[i]);
    }
    printf(":::::::::::::::::::::\n");
}

void destructLabels(Labels* labels)
{
    free(labels->lb);
}

Labels* deleteLabels(Labels* labels)
{
    destructLabels(labels);
    free(labels);

    return NULL;
}


//BINARY CODE:
void constructBinaryCode(BinaryCode* bcode)
{
    bcode->code = (uint8_t*)calloc(BASE_CAPACITY, sizeof(uint8_t));
    bcode->capacity = BASE_CAPACITY;   
    bcode->length = 0;
    bcode->offset = 0;
}

BinaryCode* newBinaryCode()
{
    BinaryCode* bcode = (BinaryCode*)calloc(1, sizeof(BinaryCode));

    constructBinaryCode(bcode);

    return bcode;
}

void capacityProcessing(BinaryCode* bcode, const size_t elem_size)
{
    while (bcode->length + elem_size > bcode->capacity)
    {
        bcode->capacity = bcode->capacity * 2;
    }

    uint8_t* new_region = (uint8_t*)realloc(bcode->code, sizeof(uint8_t) * bcode->capacity);
    assert(new_region != NULL);
    bcode->code = new_region;
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

void insertTo(BinaryCode* bcode, const void* value, const size_t elem_size, const size_t index)
{
    Copy(bcode->code + index, value, elem_size);
}

void outputBinary(BinaryCode* bcode)
{
    printf("BINARY CODE:\n");
    printf("CUR OFFSET: %llu:\n", bcode->offset);
    for (size_t i = 0; i < bcode->length; ++i)
    {
        printf("%02X ", bcode->code[i]);
        
        if (i % 16 == 0 && i != 0) 
            printf("\n");
    }
    printf("\n");
}

void writeToBinary(BinaryCode* bcode, const char* filename)
{
    FILE* binary_file = fopen(filename, "wb");

    fwrite(bcode->code, sizeof(uint8_t), bcode->length, binary_file);

    fclose(binary_file);
}

void writeNumber(char* string, BinaryCode* bcode)
{
    elem_t number = strtod(string, NULL);

    printf("SET NUMBER: %lg\n", number);

    pushFromOffset(bcode, &number,  sizeof(elem_t));
}

void writeRegister(char* string, BinaryCode* bcode)
{
    uint8_t reg_index = string[1] - 'a';

    printf("SET REGISTER: %s\n", REGISTERS[reg_index]);

    pushFromOffset(bcode, &reg_index, sizeof(uint8_t));
}

void destructBinaryCode(BinaryCode* bcode)
{
    free(bcode->code);
}

BinaryCode* deleteBinaryCode(BinaryCode* bcode)
{
    destructBinaryCode(bcode);
    free(bcode);

    return NULL;
}


//CODE PROCESSING
#define DEF_CMD(name, num, arg, ctrl_flow)                  \
    if (strcmp(line->lexemes[pos], #name) == 0)             \
    {                                                       \
        cmd_num = num;                                      \
        ++pos;                                              \
        pushFromOffset(bcode, &cmd_num, sizeof(uint8_t));   \
        printf("SET COMMAND: %d\n", num);                   \
        if (arg > 0)                                        \
        {                                                   \
            if (argumentsProcessing(arg, line, &pos, bcode, &labels, ctrl_flow, alg_pass) != 0)\
            {                                               \
                return argumentError(line, pos, i + 1);     \
            }                                               \
        }                                                   \
    }                                                       \
    else if (isLabel(line->lexemes[pos]))                   \
    {                                                       \
        if (alg_pass == 0)                                  \
        {                                                   \
            pushLabel(line->lexemes[pos], bcode, &labels);  \
        }                                                   \
    }                                                       \
    else


void commentsProcessing(AsmLine* asm_line)
{
    char* def_end = NULL;
    if (asm_line->amount_lexemes < 1)
        return;

    for (size_t i = 0; i < asm_line->amount_lexemes; ++i)
    {
        def_end = strchr(asm_line->lexemes[i], ';');
        if (def_end != NULL)
        {
            *def_end = '\0';
            if (def_end == asm_line->lexemes[i])
            {
                //Если начало комментария совпало с началом новой лексемы
                asm_line->amount_lexemes = i;
            }
            else asm_line->amount_lexemes = i + 1;

            break;
        }
    }
}

int ramProcessing(BinaryCode* bcode, AsmLine* line, size_t* pos, uint8_t* flag, size_t alg_pass)
{
    char* lexeme = line->lexemes[*pos] + 1; //Jump over open bracket
    *pos += 1;
    if (isRegister(lexeme))
    {
        writeRegister(lexeme, bcode);
        *flag |= 0x02;
    }
    else if (isNumber(lexeme))
    {
        *flag |= 0x01;
        writeNumber(lexeme, bcode);
        return 0;
    }
    if (*pos > line->amount_lexemes)
        return 1; 

    if (!isAdditional(line, *pos))
        return 0;

    *pos += 1;
    lexeme = line->lexemes[*pos];
    printf("cur lexeme: %s\n", lexeme);
    if (isNumber(lexeme))
    {
        *flag |= 0x01;
        writeNumber(lexeme, bcode);
    }
    if (!isCloseBracketInTheEnd(lexeme) && alg_pass == 0)
        return 1;

    return 0;
}

int ctrlFlowProcessing(BinaryCode* bcode, Labels* labels, AsmLine* line, size_t* pos, size_t alg_pass)
{
    if (alg_pass == 1)                                            
    {    
        printf("TRY TO FIND: %s|\n", line->lexemes[*pos]);  

        Label* label = findLabel(labels, line->lexemes[*pos]);                         
        if (label != NULL)                                      
        {                                                           
            printf("FIND LABEL: %s\n", label->label_name);      
            printf("SET LABEL NUM: %lg\n", label->mark);    

            pushFromOffset(bcode, &label->mark, sizeof(elem_t));
        }                                                           
        else                                                        
        {                                                           
            printf("CANNOT FIND LABEL NAME\n->%s\n", line->lexemes[*pos]);
            return 1;                                               
        }                                                                                                                       
    }                                                                   
    else                                                                
    {
        pushFromOffset(bcode, &DAM, sizeof(elem_t));                    
    }       
    // outputBinary(bcode);

    return 0;                                               
}

int defualtProcessing(BinaryCode* bcode, AsmLine* line, size_t* pos, uint8_t* flag)
{
    if (isRegister(line->lexemes[*pos]))
    {
        writeRegister(line->lexemes[*pos], bcode);
        *flag |= 0x02;
    }
    if (isAdditional(line, *pos + 1))
        *pos += 2;
    
    if (*pos > line->amount_lexemes)
    {
        printf("Error.Pos out of line\n->%s\n", line->lexemes[line->amount_lexemes-1]);
        return 1;
    }
    if (isNumber(line->lexemes[*pos]))
    {
        *flag |= 0x01;
        writeNumber(line->lexemes[*pos], bcode);
    }

    return 0;
}

int argumentsProcessing(size_t arg, AsmLine* line, size_t* pos, BinaryCode* bcode, Labels* labels, int ctrl_flow, size_t alg_pass)
{
    uint8_t flag = 0x00;
    uint64_t flag_adrress = bcode->offset;
    pushFromOffset(bcode, &flag, sizeof(uint8_t));

    if (!isCtrlFlow(ctrl_flow))
    {
        for (size_t i = 0; i < arg; ++i)
        {
            if (isOpenBracket(line->lexemes[*pos]))
            {
                printf("SET RAM CALL\n");
                flag |= 0x04;
                if (ramProcessing(bcode, line, pos, &flag, alg_pass) != 0)
                    return 1;
            }
            else 
            {
                if (defualtProcessing(bcode, line, pos, &flag) != 0)
                    return 1;
            }
        }
        *pos += 1;
        printf("SET FLAG: %d\n", flag);
        insertTo(bcode, &flag, sizeof(uint8_t), flag_adrress);
        flag = 0x00;
        flag_adrress = bcode->offset;
    }
    else
    {
        printf("FIND CTRL_FLOW.\nSET FLAG: %d\n", flag); 
        if (ctrlFlowProcessing(bcode, labels, line, pos, alg_pass) != 0)
        {
            labelError(line);
            return 1;
        }
    }
    return 0;
}

int binaryTranslation(BinaryCode* bcode, AsmListing* asm_listing) 
{
    Labels labels = {};
    constructLabels(&labels);

    AsmLine* line = NULL;

    uint8_t cmd_num = 0;
    size_t pos = 0;

    for (size_t alg_pass = 0; alg_pass < NUM_ALGORITHM_PASS; ++alg_pass)
    {
        bcode->offset = 0;
        for (size_t i = 0; i < asm_listing->amount_lines; ++i)
        {
            line = &asm_listing->asm_lines[i];
            pos = 0;

            commentsProcessing(line);
            if (line->amount_lexemes < 1)
                continue;
            
            #include "commands.h"
            /*else*/
            if (pos < line->amount_lexemes - 1)
                return tooManyArgumentsError(line, pos, i + 1);

            else
                return argumentError(line, pos, i + 1);
        }
    }
    
    destructLabels(&labels);
    printf("Binary translation was successful!\n");
    return 0;
}

int Translator(const char* filename, const char* binary_filename) 
{
    assert(filename);
    assert(binary_filename);

    int result = 0;
    AsmListing asm_listing = {};
    fillAsmListing(&asm_listing, filename);
    BinaryCode bcode = {};
    constructBinaryCode(&bcode);

    result = binaryTranslation(&bcode, &asm_listing);

    writeToBinary(&bcode, binary_filename);

    destructAsmListing(&asm_listing);
    destructBinaryCode(&bcode);

    return result;
}