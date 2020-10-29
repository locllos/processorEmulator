#include "headers/assembler.h"

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

void movePosAfterOpenBrackets(CurLine* cur_line)
{
    // printf("STRING BEFORE OPEN BRACKET: %s\n", cur_line->string + cur_line->pos);

    char* find_brackets = strchr(cur_line->string + cur_line->pos, '[');
    
    cur_line->pos =  (find_brackets + 1 - cur_line->string);
    
    cur_line->flag |= 0x04;

    // printf("POS AFTER OPEN BRACKET: %s\n", cur_line->string + cur_line->pos);
}

void movePosAfterAdditioal(CurLine* cur_line)
{   
    // printf("STRING BEFORE ADD: %s\n", cur_line->string + cur_line->pos);
    char* find_additional = strchr(cur_line->string + cur_line->pos, '+');
    cur_line->pos =  (find_additional + 1 - cur_line->string);
    // printf("POS AFTER ADD: %s\n", cur_line->string + cur_line->pos);

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

int isNumber(CurLine* cur_line)
{
    if (sscanf(cur_line->string + cur_line->pos, "%lg", &cur_line->value) == 1)
    {
        return 1;
    }

    return 0;
}
