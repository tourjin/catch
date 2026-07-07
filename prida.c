#include <stdio.h>
#include <inttypes.h> // Includes <stdint.h> and adds printf macros
void print_byte(const unsigned char byte) {
  unsigned int mask = 1U;
  for (int i = 0; i < 8; i++) {
    printf("%c", (byte & (mask << (7 - i))) ? '1' : '0');
  }
}
void print_float(float f) {
  unsigned char *p = (unsigned char *)&f;

  for (int i = 3; i >= 0; --i) {
    print_byte(p[i]);
    if (i > 0) {
      printf("|");
    }
  }
  printf("\n");
}

int main() {
    int64_t big_int = 9223372036854775807; // Maximum 8-byte signed int
    uint64_t big_uint = 18446744073709551615U; // Maximum 8-byte unsigned int
    unsigned int n = 4294967295; // Maximum value for unsigned int
    unsigned char * p;
    p= ((unsigned char *)&n);

    printf("as signed int:%d , as unsigned int: %u, as char:%c\n", n, n, n);
    for (int i = 3; i >=0; i--){
      if(i !=3) putchar('|');
      print_byte(p[i]);
    }
    putchar('\n');
    n = 4294967295 - 4294967294;
    n += 8;
    n += 4;
    n += 2;
    printf("%u\n", n);
    for (int i = 3; i >=0; i--)
      print_byte(p[i]);
    putchar('\n');

    // PRId64 stringifies to the correct format specifier for your system
    printf("My 8-byte signed int: %" PRId64 "\n", big_int);
    printf("My 8-byte unsigned int: %" PRIu64 "\n", big_uint);

    return 0;
}
