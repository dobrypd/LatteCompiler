void printInt(const int value);
void printString(const char* value);
void error();
int readInt();
char* readString();
char* addStrings_name_with_name_mangling_5594478149272763309697(char* str1, char* str2);


int main()
{
    printInt(readInt());
    char* read_str = readString();
    printString(
        addStrings_name_with_name_mangling_5594478149272763309697(read_str, read_str)
    );
    return 0;
}
