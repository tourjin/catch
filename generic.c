#include <stdio.h>
#define print_variable(x) printf(_Generic((x), \
    int: "Integer: %d\n",                      \
    double: "Double: %f\n",                    \
    char*: "String: %s\n",                     \
    default: "Unknown type\n"                  \
), x)

int main() {
    int a = 10;
    double b = 5.5;
    char* c = "Hello World";
    print_variable(a); // Resolves to: printf("Integer: %d\n", a);
    print_variable(b); // Resolves to: printf("Double: %f\n", b);
    print_variable(c); // Resolves to: printf("String: %s\n", c);
}
