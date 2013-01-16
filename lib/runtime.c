#include <stdio.h>
#include <stdlib.h>

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
