#define XSTR(a) STR(a)
#define STR(a) #a
#define PRINT_ERR() puts(XSTR(__LINE__))
PRINT_ERR()
