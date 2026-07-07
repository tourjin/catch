#include <stdio.h>
#include <stdint.h>
#include <limits.h>
int main(){
  printf("SIZE_MAX is %zu in sizeof %d bytes\n", SIZE_MAX, (int) sizeof(SIZE_MAX));
  printf("INT_MAX is %dbytes, value of %d\n",(int)sizeof(int),INT_MAX);
}
