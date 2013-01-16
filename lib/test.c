void printInt(const int value);
void printString(const char* value);
void error();
int readInt();
char* readString();

int main()
{
   printInt(readInt());
   printString(readString());
   return 0;
}
