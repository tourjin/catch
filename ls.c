#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
void mode_string(mode_t mode, char *str) {
  if (S_ISDIR(mode)) str[0] = 'd';
  else if (S_ISLNK(mode)) str[0] = 'l';
  else if (S_ISCHR(mode)) str[0] = 'c';
  else if (S_ISBLK(mode)) str[0] = 'b';
  else if (S_ISFIFO(mode)) str[0] = 'p';
  else if (S_ISSOCK(mode)) str[0] = 's';
  else str[0] = '-';
  str[1] = (mode & S_IRUSR) ? 'r' : '-';
  str[2] = (mode & S_IWUSR) ? 'w' : '-';
  str[3] = (mode & S_IXUSR) ? 'x' : '-';
  str[4] = (mode & S_IRGRP) ? 'r' : '-';
  str[5] = (mode & S_IWGRP) ? 'w' : '-';
  str[6] = (mode & S_IXGRP) ? 'x' : '-';
  str[7] = (mode & S_IROTH) ? 'r' : '-';
  str[8] = (mode & S_IWOTH) ? 'w' : '-';
  str[9] = (mode & S_IXOTH) ? 'x' : '-';
  str[10] = '\0';
}
void print_long(const char *dir, const char *name){
  char fullpath[4096];
  snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, name);
  struct stat st;
  if(lstat(fullpath, &st) < 0){
    perror(name);
    return;
  }
  char modes[11];
  mode_string(st.st_mode, modes);
  struct passwd *pw =getpwuid(st.st_uid);
  struct group *gr =getgrgid(st.st_gid);
  const char *user = pw? pw->pw_name : "?";
  const char *group= gr? gr->gr_name : "?";
  char timebuf[64];
  struct tm *tm = localtime(&st.st_mtim.tv_sec);
  strftime(timebuf, sizeof(timebuf), "%b %e %H:%M", tm);
  printf("%s %3lu %s %s %2ld %s %s\n", 
         modes,
         (unsigned long) st.st_nlink,
         user,
         group,
         (long) st.st_size,
         timebuf,
         name);
  //printf("%s %s %s %s\n", modes, user, group, name);
}
int show_all = 0;
int long_format =0;
int main(int argc, char *argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "al")) != -1) {
    switch (opt) {
    case 'a':
      show_all = 1;
      break;
    case 'l':
      long_format =1;
      break;
    default:
      fprintf(stderr, "Usage: %s [-a] [path]\n", argv[0]);
      return 1;
    }
  }
  for (int i = 0; i < argc; i++) {
    printf("argv[%d]: %s %s\n", i, argv[i],
           (i == optind) ? "<-- optind points here" : "");
  }
  printf("optind is %d, argc is %d\n", optind, argc);

  int start = optind;
  int end = (optind == argc) ? optind + 1 : argc;
  for (int i = start; i < end; i++) {
    const char *path = (i == argc) ? "." : argv[i];
    DIR *dir = opendir(path);
    if (!dir) {
      perror("opendir");
      return 1;
    }
    struct dirent *entry;
    while ((entry = readdir(dir))) {
      if (!show_all && entry->d_name[0] == '.')
        continue;
      if(long_format){ 
        print_long(path, entry->d_name);
      }
      else
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
  }
  return 0;
}
