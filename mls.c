#include <dirent.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
typedef struct ml {
  int max_links_w;
  int max_user_w;
  int max_group_w;
  int max_size_w;
} ml;

int whelp(long long int v) {
  int w = 0;
  if (v == 0)
    return 1;
  while (v > 0) {
    w++;
    v /= 10;
  }
  return w;
}
void mode_string(char *str, mode_t mode) {
  switch (mode & S_IFMT) {
  case S_IFBLK: str[0] = 'b'; break;
  case S_IFCHR: str[0] = 'c'; break;
  case S_IFDIR: str[0] = 'd'; break;
  case S_IFIFO: str[0] = 'p'; break;
  case S_IFLNK: str[0] = 'l'; break;
  case S_IFREG: str[0] = '-'; break;
  case S_IFSOCK: str[0] = 's'; break;
  default: str[0] = '?'; break;
  }
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
int pass(const char *path, struct dirent *entry, ml *width) {
  static long long int sizemax = 0;
  char fullpath[4096];
  struct stat st;
  if (strcmp(path, "/") == 0) {
    snprintf(fullpath, sizeof(fullpath), "/%s", entry->d_name);
  } else {
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
  }
  if (lstat(fullpath, &st) == -1) {
    return -1;
  }
  if (st.st_size > sizemax) {
    sizemax = st.st_size;
    width->max_size_w = whelp(sizemax);
  }

  struct passwd *pw = getpwuid(st.st_uid);
  const char *user_name_ptr = (pw) ? pw->pw_name : "unknown";
  int user_length = strlen(user_name_ptr);
  if (user_length > width->max_user_w) {
    width->max_user_w = user_length;
  };
  struct group *grp = getgrgid(st.st_gid);
  const char *grp_name_ptr = (grp) ? grp->gr_name : "unknown";
  int grp_length = strlen(grp_name_ptr);
  if (grp_length > width->max_group_w) {
    width->max_group_w = grp_length;
  }
  int link = whelp(st.st_nlink);
  if (link > width->max_links_w) {
    width->max_links_w = link;
  }
  return (width->max_size_w);
}
void printf_long(const char *path, struct dirent *entry) {
  char fullpath[4096];
  extern int record;
  extern ml width;
  char str[11] = {[0 ... 10] = '\0'};
  struct stat st;
  if (strcmp(path, "/") == 0) {
    snprintf(fullpath, sizeof(fullpath), "/%s", entry->d_name);
  } else {
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
  }

  if (lstat(fullpath, &st) == -1) {
    return ;
  }
  mode_t mode = st.st_mode;

  mode_string(str, mode);
  char timebuf[64];
  char grp_name[64];
  char user_name[64];
  struct tm *tm = localtime(&st.st_mtim.tv_sec);
  strftime(timebuf, sizeof(timebuf), "%b %e %H:%M", tm);

  struct group *grp = getgrgid(st.st_gid);
  const char *grp_name_ptr = (grp) ? grp->gr_name : "unknown";
  strncpy(grp_name, grp_name_ptr, sizeof(grp_name));
  struct passwd *pw = getpwuid(st.st_uid);
  const char *user_name_ptr = (pw) ? pw->pw_name : "unknown";
  strncpy(user_name, user_name_ptr, sizeof(user_name));

  char target_path[1024] = {0};
  char symarrows[16] = "";
  // readlink does not add a null-terminator (\0), so we must handle it
  if (S_ISLNK(mode)) {
    ssize_t len = readlink(fullpath, target_path, sizeof(target_path) - 1);
    if (len != -1) {
      target_path[len] = '\0'; // Null-terminate the string
      strncpy(symarrows, " -> ", sizeof(symarrows));
    }
  }

  printf("%s %*d %-*s %-*s %*ld %s %s %s %s\n", str, 
         width.max_links_w, (int)st.st_nlink, 
         width.max_user_w, user_name, 
         width.max_group_w, grp_name,
         width.max_size_w, (long) st.st_size, 
         timebuf, entry->d_name, symarrows, target_path);
}

int record;
ml width ={0};
int main(int argc, char *argv[]) {
//  char str[4096];
  int opt;
  int show_all = 0;
  int long_format = 0;
  while ((opt = getopt(argc, argv, "al")) != -1) {
    switch (opt) {
    case 'a':
      show_all = 1;
      break;
    case 'l':
      long_format = 1;
      break;
    default:
      printf("usage : ./my_prog [-a] [dir]\n");
    }
  }
  int start = optind;
  int end = (argc == optind) ? argc + 1 : argc;
  for (int i = start; i < end; i++) {
    const char *path = (argc == start) ? "." : argv[i];
    DIR *dir = opendir(path);
    if (!dir) {
      perror("usage: ls [-l][-a] directory\n");
      return 13;
    }
    struct dirent *entry;
    while ((entry = readdir(dir))) {
      if ((show_all == 0) && (entry->d_name[0] == '.'))
        continue;
      if (long_format) {
        record = pass(path, entry, &width);
      }
    }
    closedir(dir);
  }

  start = optind;
  end = (argc == optind) ? argc + 1 : argc;
  for (int i = start; i < end; i++) {
    const char *path = (argc == start) ? "." : argv[i];
    if (argc != start) {
        printf("%s:\n", path);
    }
    DIR *dir = opendir(path);
    if (!dir) { continue; }
    struct dirent *entry;
    while ((entry = readdir(dir))) {
      if ((show_all == 0) && (entry->d_name[0] == '.'))
        continue;
      if (long_format) {
        printf_long(path, entry);
      } else {
        printf("%s\n", entry->d_name);
      }
    }
     if (argc != start && i < end - 1) {
        printf("\n");
    }
    closedir(dir);
  }

  //  printf("link:%d user:%d grp:%d filesize:%ld \n",
  //         width.max_links_w, width.max_group_w, width.max_user_w,
  //         width.max_size_w);
  return 0;
}
