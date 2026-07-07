#include <stdio.h>
#define BUFSIZE 56 
int main(int argc, char **argv){
  char item[BUFSIZE];
  char delim;
  while( scanf("%s", item) != EOF){
      printf("%s\n", item);

  }

  return 0;
}
