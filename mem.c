#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(){
  char str[10]= {[0 ... 8]= 'a', [9] = '\0'};
  char str2[6] = {'\0'};
  char * ptr=NULL;
  int ccount =0;
  fprintf(stdout, "sizeof str2[] is %lu:%s\n",sizeof(str2), str2);

  int alloc = sizeof( str2);
  if( ccount < alloc + 1){
   ptr =realloc(ptr, (unsigned long)sizeof(str2));
   ccount = alloc;
   memcpy( ptr, str, ccount);
   ptr[ccount -1]='\0';
  }
  fprintf(stdout, "strlen of char *ptr is %lu:%s\n",strlen(ptr), ptr);

  alloc = sizeof( str);
  if( ccount < alloc+1){
    ptr =realloc(ptr, (unsigned long)sizeof(str));
    ccount = alloc;
    memcpy( ptr, str, sizeof(str));
    ptr[ccount -1]='\0';
  }
  fprintf(stdout, "strlen of char *ptr is %lu :%s\n",strlen( ptr), ptr);
  free(ptr);
  return 0;
}
