#include <stdalign.h>
#include <stdio.h>
#include <stdint.h>
int main() {
  printf("%zu\n", alignof(int));
  printf("%lu\n", sizeof(int));
  int v1;
  int v2;

  printf("%p\n", (void *) &v1);
  printf("%p\n", (void *) &v2);
  intptr_t diff = (intptr_t)&v2 - (intptr_t)&v1;
  printf("Byte difference = %ld\n", diff);
  int value =97;
  typedef unsigned char *charptr;
  charptr p = (charptr)&value;
  printf("address of value: %p and value is %d (int)  %c (char)\n", p, *p, *p);
  for ( int i =0 ; i < 4 ; i++){
    printf("%8b\t", *(p+i));}
  printf("\n");

  return 0;
}
