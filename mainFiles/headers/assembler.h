#include "cpu.h"

void Copy(void* value_to, const void* value_from, const int elem_size);

Labels* newLabels();

void addLabel(Labels* labels, const char* label_name, const elem_t mark);

void destructLabels(Labels* labels);

Label* findLabel(Labels* labels, const char* label_name);

uint64_t approxLength(const char* filename);

int64_t indexStringInArray(const char* string, const char* arr[], const size_t length);

char* readContents(const char* filename, uint64_t* length);

Text* parseContents(const char* filename);

void destructText(Text* text);

BinaryCode* newBinaryCode(const size_t start_capacity);

void capacityProcessing(BinaryCode* bcode, const size_t elem_size);

void pushFromOffset(BinaryCode* bcode, const void* elem, size_t elem_size);

int ctrlFlowProcess(BinaryCode* bcode, Labels* labels, CurLine* cur_line, uint64_t alg_pass);

int isNumberNext(CurLine* cur_line);

int isStringNext(CurLine* cur_line);

int isRegisterNext(CurLine* cur_line);

int isLabelNext(CurLine* cur_line);

void writeNumber(CurLine* cur_line, BinaryCode* bcode);

void writeRegister(CurLine* cur_line, BinaryCode* bcode);

void writeLabel(CurLine* cur_line, BinaryCode* bcode, Labels* labels);

int registerError(CurLine* cur_line);

int argumentError(CurLine* cur_line);

int isCtrlFlow(const uint8_t ctrl_flow);

void destructBinaryCode(BinaryCode* bcode);

void writeToBinary(BinaryCode* bcode, const char* filename);

int oneArgumentProcessing(CurLine* cur_line, BinaryCode* bcode, Labels* labels, const uint8_t ctrl_flow, const uint8_t alg_pass);

CurLine* newCurLine();

int commentsProcessing(CurLine* cur_line);

int endingsProcess(CurLine* cur_line);

void resetCurLine(CurLine* cur_line);

int codeProcessing(const char* filename, const char* binary_filename);

void insertTo(BinaryCode* bcode, const void* value, const uint64_t elem_size, const uint64_t index);

void outputBinary(BinaryCode* bcode);

void clearCloseBracket(char* string);