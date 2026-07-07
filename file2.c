#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main() {
    // 1. Tell C to use the user's system locale (crucial for UTF-8/Korean support)
    setlocale(LC_ALL, "");

    FILE *fp = fopen("HellowWorld.txt", "r");
    if (!fp) return 1;

    wint_t character; // Use wint_t instead of int for wide characters

    // 2. Use fgetwc instead of getc to read full multi-byte characters
    while ((character = fgetwc(fp)) != WEOF) {
        putwc(character, stdout); // Use putwc instead of putc
    }

    fclose(fp);
    return 0;
}
