#include "cpu.h"

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
    char* cur_string = NULL;
    char* command = NULL;
    int pos = 0;
    int tmp_pos = -1;
    char* register_name = NULL;
    for (size_t cur_line = 0; cur_line < amount_lines; ++cur_line)
    {   
        printf("========================\n");
        value = 0;
        cur_string = NULL;
        command = (char*)calloc(128, sizeof(char));
        pos = 0;
        tmp_pos = -1;
        register_name = (char*)calloc(32, sizeof(char));

        cur_string = code->lines[cur_line].string;

        //printf("CUR STRING: %s\n", cur_string);
        sscanf(cur_string + pos, "%s%n", command, &tmp_pos);

        pos += tmp_pos;

        if (command == NULL)
        {
            printf("CANNNOT READ COMMAND!\n");
            return 1;
        }
        
        if (command[0] == COMMANDS[DO_NOTHING_COMMAND][0])
            continue;

        uint8_t cur_command = indexStringInArray(command, COMMANDS, AMOUNT_COMMANDS);
        if (cur_command < 0)
        {
            printf("ERROR!\n COMMAND NAME DOESNT FIND AT LINE: %d\n", cur_line);
            return 1;
        }
        printf("FOUND COMMAND: |%s|\n", COMMANDS[cur_command]);

        if (cur_command > 0)
        {
            fwrite(&cur_command, sizeof(uint8_t), 1, binary_file);
            printf("ADD COMMAND: %s\n", COMMANDS[cur_command]);
            //printf("START READING FROM: |%s|\n", cur_string + pos);

            if (sscanf(cur_string + pos, "%lg%n", &value, &tmp_pos) > 0)
            {
                fwrite(&NUM, sizeof(uint8_t), 1, binary_file);
                printf("ADD MODIFICATOR: %d\n", NUM);
                
                //printf("TMP POS: %d\n", tmp_pos);

                fwrite(&value, sizeof(double), 1, binary_file);
                printf("ADD NUMBER: %lg\n", value);

            }
            else if (sscanf(cur_string + pos, "%s%n", register_name, &tmp_pos) > 0)
            {   
                uint8_t cur_register = indexStringInArray(register_name, REGISTERS, AMOUNT_REGISTERS);
                if (cur_register >= 0)
                {
                    fwrite(&REG, sizeof(uint8_t), 1, binary_file);
                    printf("ADD MODIFICATOR: %d\n", REG);

                    fwrite(&cur_register, sizeof(uint8_t), 1, binary_file);
                    printf("ADD REGISTER: %d\n", cur_register);

                }
                else
                {
                    printf("ERROR!\n REGISTER NAME DOESNT FIND AT LINE: %d\n", cur_line);
                    return 2;
                }
            }
        }
    }

    fclose(binary_file);

    return 0;
}    


int main(int argC, char* argV[])
{   
    char* filename = NULL;
    char* binary_filename = "bc.bin";

    /*
    if (argC < 2)
    {
        printf("FILE NAME NOT FOUND!\n");
        return 1;
    }
    else if (argC == 2)
    {   
        filename = argV[1];
        codeProcessing(filename, binary_filename);
    }
    else if (argC == 3)
    {
        filename = argV[1];
        binary_filename = argV[2];
    }
    else
    {
        printf("ERROR!\n");
        return 1;
    }
    */
    
    codeProcessing("my_code.txt", "bc.bin");
    return 0;
}