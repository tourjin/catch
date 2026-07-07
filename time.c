#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <locale.h>
int main(){
  setlocale(LC_ALL, "");
  struct stat sb;
  lstat("time.c", &sb);
  struct tm *tm;
  tm = localtime(&sb.st_mtime);
  char str[64];
  strftime(str, sizeof(str), "%b %e %H:%M", tm);
  printf("%s\n", str);
  
  return 0;
}
