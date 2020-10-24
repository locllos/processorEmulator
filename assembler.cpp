#include "headers/cpu.h"

#define DEF_CMD(name, num, arg)                                                         \
        if (strcmp(cmd, #name) == 0)                                                    \
        {                                                                               \
            cmd_num = num;                                                              \
            fwrite(&cmd_num, sizeof(uint8_t), 1, binary_file);                          \
            /*printf("FIND COMMAND: %s\n", cmd);*/                                      \
            printf("SET COMMAND: %d\n", num);                                           \
            if (arg == 1)                                                               \
            {                                                                           \
                if (sscanf(cur_string + pos, "%lg%n", &value, &tmp_pos) == 1)           \
                {                                                                       \
                    flag = NUMBER;                                                      \
                    printf("FIND NUMBER.\nSET FLAG: %d\n", flag);                       \
                    fwrite(&flag, sizeof(uint8_t), 1, binary_file);                     \
                    printf("VALUE OF NUMBER: %lg\n", value);                            \
                    fwrite(&value, sizeof(elem_t), 1, binary_file);                     \
                                                                                        \
                }                                                                       \
                else if (sscanf(cur_string + pos, "%s%n", reg, &tmp_pos) == 1)          \
                {                                                                       \
                    reg_index = indexStringInArray(reg, REGISTERS, AMOUNT_REGISTERS);   \
                    if (reg_index > -1 && reg_index < AMOUNT_REGISTERS)                 \
                    {                                                                   \
                        flag = REGISTER;                                                \
                        printf("FIND REGISTER.\nSET FLAG: %d\nREGISTER ADDRESS: %s\n", flag, reg);\
                        fwrite(&flag, sizeof(uint8_t), 1, binary_file);                 \
                        fwrite(&reg_index, sizeof(int8_t), 1, binary_file);             \
                    }                                                                   \
                    else                                                                \
                    {                                                                   \
                        printf("ERROR! WRONG REGISTER NAME AT LINE %llu!\n", i + 1);    \
                        printf("->%s\n", reg);                                          \
                        return 1;                                                       \
                    }                                                                   \
                }                                                                       \
                else                                                                    \
                {                                                                       \
                    printf("ARGUMENT DOESNT FIND AT LINE %llu!", i + 1);                \
                    return 3;                                                           \
                }                                                                       \
                pos += tmp_pos;                                                         \
            }                                                                           \
        }                                                                               \
        else    

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
    //Sorry for legacy code. This is from Onegin project. I was hot and young...
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

    for (size_t i = 0; i < text->number_of_lines; ++i)
    {
        free(text->lines[i].string);
    }

    free(text->lines);

    if (text->buffer != NULL)
    {
        free(text->buffer);
    }
}

int codeProcessing(const char* filename, const char* binary_filename)
{   
    Text* code = parseContents(filename);

    if (binary_filename == NULL)
    {
        printf("NULL POINTER FILE NAME!\n");

        return 3;
    }

    FILE* binary_file = fopen(binary_filename, "wb");

    uint64_t amount_lines = code->number_of_lines;

    elem_t value = 0;
    uint64_t cmd_num = 0;
    char* cur_string = NULL;
    char* cmd = (char*)calloc(16, sizeof(char));
    uint64_t pos = 0;
    int64_t tmp_pos = -1;
    char* reg = (char*)calloc(16, sizeof(char));
    int8_t reg_index = -1;
    FLAG flag = NUMBER;
    char* def_end = NULL;
    char* check_ending = (char*)calloc(1, sizeof(char));
    for (uint64_t i = 0; i < amount_lines; ++i)
    {
        value = 0;
        cmd_num = 0;
        pos = 0;
        tmp_pos = -1;
        flag = NUMBER;
        reg_index = -1;
        def_end = NULL;
        cur_string = code->lines[i].string;
        *check_ending = '\0';


        //Delete comments
        def_end = strchr(cur_string, ';');                                               
        if (def_end != NULL)                                                            
        {    
            if (cur_string == def_end)
            {
                continue;
            }                                                                     
            *def_end = '\0';    

        }

        strcpy(cmd, "upd cmd");                                                            
        sscanf(cur_string, "%s%n", cmd, &tmp_pos);   
        pos += tmp_pos;

        #include "commands.h"
        /*else*/
        if (strcmp(cmd, "upd cmd") == 0)
        {
            continue;
        }
        else
        {
            printf("UNKNOWN COMMAND AT LINE %llu!\n", i + 1);
            printf("->%s\n", cmd);
            return 2;
        }

        sscanf(cur_string + pos, "%s", &check_ending);
        if (*check_ending != '\0')
        {
            sscanf(cur_string + pos, "%s", cmd);
            printf("UNKNOWN COMMAND AT LINE %llu!\n", i + 1);
            printf("->%s", cmd);
            return 2;
        }

    }
    fclose(binary_file);

    #undef DEF_CMD

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