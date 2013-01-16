void printInt(const int value);
void printString(const char* value);
void error();
int readInt();
char* readString();

int main()
{
   printInt(readInt());
   char* read_str = readString();
   printString(read_str);
   return 0;
}
