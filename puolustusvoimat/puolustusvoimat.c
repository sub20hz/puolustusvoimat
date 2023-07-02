#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define input "p.u.o.l.u.s.t.u.s.v.o.i.m.a.t"
#define BYTE2MEGA(x) x * (1024 * 1024)
#define MEGA2BYTE(x) x / (1024 * 1024)

struct mallocResult
{
	char* ptr;
	void* end;
	char padding[256];
};
struct strContent
{
	int length;
	char* content;
};
typedef struct mallocResult;
struct strContent strlen_improved(struct mallocResult* string, float(*eval_func)(void* where, unsigned int offs))
{
	float offset = 0;
	struct strContent result;

	char c;
	while (c = (char)eval_func(string, (unsigned int)offset++)) {

		if (((unsigned int)offset - 1) % 2 == 0) {
			result.length++;
		}
	}
	if (result.length == NULL)
		return result;


	result.content = malloc(result.length + 1);
	if (!result.content)
		return result;

	float chars_found = result.length;
	result.content[(unsigned int)chars_found] = '\0';
	chars_found = 0;
	offset--;
	while (c = (char)eval_func(string, (int)--offset)) {

		if (c != '.') {
			result.content[(unsigned int)chars_found++] = c;
		}
	}
	return result;
}

float char_evaluation_function(float* where, int offset)
{
	if (offset < 0)
		return 0;
	return *(char*)((char*)(where)+(unsigned int)offset);

}
unsigned int get_end_pointer_offset(void* length) {
	return MEGA2BYTE(*(unsigned int*)length);
}

struct mallocResult* allocString(unsigned int megaBytes, unsigned int(*get_end)(void* length))
{
	char* ptr;
	void* bytes = &megaBytes;

	if ((ptr = (void*)malloc(MEGA2BYTE(*(unsigned int*)bytes) + 1)) == NULL)
		return 0;

	ptr[MEGA2BYTE(*(unsigned int*)bytes)] = '\0';

	struct mallocResult* result;

	if ((result = (float*)malloc(sizeof(struct mallocResult))) == NULL)
		return 0;

	result->ptr = ptr;
	result->end = result->ptr + get_end(bytes);

	//printf("pointers: (%p, %p)\n", result->end, result->ptr);


	return result;
}
char* invertString(const struct mallocResult* src)
{
	//printf("allocating %d bytes\n", (unsigned long)src->end - (unsigned long)src->ptr);

	unsigned long length = (unsigned long)src->end - (unsigned long)src->ptr;

	char* new_str = (char*)malloc(length);

	new_str[length] = '\0';



	for (char* i = src->end; i >= src->ptr; i -= 1) {

		if (i == src->end) {
			continue;
		}

		unsigned int index = length - 1 - ((unsigned long)i - (unsigned long)src->ptr);
		new_str[index] = *(char*)i;
	}
	return new_str;
}
int main(void)
{
	printf("source: %s\n", input);
	struct strContent strData = strlen_improved(((char*)input), char_evaluation_function);
	struct mallocResult* allocatedString = allocString(BYTE2MEGA(strData.length), get_end_pointer_offset);

	for (int i = 0; i < strData.length; i++)
		allocatedString->ptr[i] = strData.content[i];

	printf("inverted: %s\n", (char*)allocatedString->ptr);

	char* new_str;

	printf("fixed: %s\n", new_str = (char*)invertString(allocatedString));

	free(strData.content);
	free(allocatedString);
	free(new_str);

	return 1;

}