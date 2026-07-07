#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(){
  unsigned char ar[] = { 0x00, 0x00,  0x80, 0x7f};
  float * f = (float*) ar;
  printf("%e\n", *f);
  float f2 = (float ) *ar;
  printf("%e\n", f2);
  float f3 = *((float *) &ar);
  printf("%e\n", f3);

  char b[] = "01111111100000000000000000000000";
  unsigned long  ch = strtoul(b, NULL, 2);
  float target ;
  memcpy(&target, &ch, sizeof(target)) ;
  printf("%e\n", target);
  
  return 0;
}
