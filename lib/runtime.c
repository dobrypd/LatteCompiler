#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printInt(const int value)
{
    printf("%d\n", value);
}

void printString(const char* value)
{
    printf("%s\n", value);
}

void error()
{
    printf("runtime error\n");
    exit(1);
}

int readInt()
{
    int value;
    scanf("%d\n", &value);
    return value;
}

char* readString()
{
    char* line = NULL;
    size_t size = 0;
    getline(&line, &size, stdin);
    return line;
}


char* addStrings_name_with_name_mangling_5594478149272763309697(const char* str1, const char* str2)
{
    size_t len1 = strlen(str1), len2 = strlen(str2);
    char *concat = (char*) malloc(len1 + len2 + 1);

    memcpy(concat, str1, len1);
    memcpy(concat+len1, str2, len2+1);
    return concat;
}

