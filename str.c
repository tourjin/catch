#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void print_byte(const unsigned char byte) {
  unsigned int mask = 1U;
  for (int i = 0; i < 8; i++) {
    printf("%c", (byte & (mask << (7 - i))) ? '1' : '0');
  }
}
int main(){
  char a[]="123456";
  unsigned char * data = (unsigned char *) a;
  unsigned char inf_bytes[] = {0x00, 0x00, 0x80, 0x7F};
  char b[] = "01111111100000000000000000000000";
  unsigned int row_bits = strtoul(b, NULL, 2);
  float f ;
  memcpy( &f, &row_bits, sizeof(f)); 
  float * ff= (float *)inf_bytes;
  printf("sizeof a[] is %lu \n", (unsigned long)sizeof(a));
  printf("sizeof unsigned char is %lu \n", sizeof(unsigned char));
  for (int i = 0; i <sizeof (a); i++){
    if( i> 0 ) putchar('|');
    print_byte((unsigned char)*(a+i));
  }
  putchar('\n');
  printf("%e\n", f);
  printf("%e\n", *ff);
  return 0;

}
