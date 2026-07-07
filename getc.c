#include <stdio.h>
int
main ()
{

  char str[10];
  printf ("Enter str:\n");
  scanf ("%9s", str);		// Limits input to 9 chars to prevent buffer overflow

// Clear the buffer (swallow the leftover '\n' and any extra chars)
  int c=97;
  while ((c = getchar ()) != '\n' && c != EOF)printf("%c",c);

  printf ("Press Enter to exit...\n");
  getc (stdin);			// This will now properly pause and wait for a fresh Enter!
  return 0;
}
