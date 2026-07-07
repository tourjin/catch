#include <stdio.h>
void decay( char s1[], char *s2){

  s1[2]= '\0';
  //*(s2+2)= '\0';//seg fault
  printf("%s ", s1); printf("%s\n", s2);
  return;
}
int main()
{
  char s1[]={ 'h', 'e', 'l','l','o'};
  char *test =s1;
  char *s2 = "world";
  printf("%s ", s1); printf("%s\n", s2);
  s1[2]= '\0';
  s1[2] = 'l';
  
  test[2]= 'z';
  printf("%s ", s1); printf("%s\n", s2);

  decay( s1, s2);

  return 0;
}
