#include <stdio.h>
int
main ()
{
  FILE *fp;
  fp = fopen ("HellowWorld.txt", "r");
  if (!fp) return 1;
  int character;
  while ( (character = getc( fp)) != EOF){
    putc( character, stdout);
  }
  fclose (fp);
  return 0;
}
