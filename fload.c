#include <stdio.h>
#include <stdint.h>
void print_byte(const unsigned char byte) {
  unsigned int mask = 1U;
  for (int i = 24; i < 32; i++) {
    printf("%c", (byte & (mask << (31 - i))) ? '1' : '0');
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
  float f = 13.f;
  char c1 = 13; char c2 =-13;
  print_float(f);
  print_float(-f);
  int value = 0x12345678;
  unsigned char *p = (unsigned char *)&value;
  for(int i=0; i<sizeof(int); ++i){
    printf("%02x " , p[i]);
  }
  printf("\n");
  signed char value1= 127;
  ++value1;
  printf("value: %d\n", value1);
  print_byte(c1);
  printf("\n");
  print_byte(c2);
  printf("\n");
union{ float f;
  uint32_t i;} u;
u.i =1u;
float sub = u.f;
print_float(u.f);
printf("%.20e\n", u.f);
  return 0;
}
