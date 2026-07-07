#include <stdio.h>

int main() {
    char line[] = "apple 42,100,banana"; // Example input line
    
    char str1[100];
    char str2[100];
    int int1, int2;

    // sscanf works exactly like scanf, but reads from 'line' instead of stdin
    int successfully_read = sscanf(line, "%99s %d , %d , %99s", str1, &int1, &int2, str2);

    if (successfully_read == 4) {
        // Data is successfully stored in your 4 variables
        printf("Parsed: %s, %d, %d, %s\n", str1, int1, int2, str2);
    }
    printf("original is :%s\n", line);

    return 0;
}
