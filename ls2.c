#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
//#include <math.h>

// A helper structure to store metadata for the first pass
typedef struct {
    char name[256];
    struct stat st;
    char user[256];
    char group[256];
    char timebuf[64];
    char modes[11];
} FileInfo;

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

// Helper to find the digit length of a number
int num_digits(long long num) {
    if (num == 0) return 1;
    int len = 0;
    while (num > 0) {
        len++;
        num /= 10;
    }
    return len;
}

int show_all = 0;
int long_format = 0;

int main(int argc, char *argv[]) {
  int opt;
  while ((opt = getopt(argc, argv, "al")) != -1) {
    switch (opt) {
    case 'a': show_all = 1; break;
    case 'l': long_format = 1; break;
    default:
      fprintf(stderr, "Usage: %s [-a] [-l] [path]\n", argv[0]);
      return 1;
    }
  }

  int start = optind;
  int end = (optind == argc) ? optind + 1 : argc;

  for (int i = start; i < end; i++) {
    const char *path = (i == argc) ? "." : argv[i];
    DIR *dir = opendir(path);
    if (!dir) {
      perror("opendir");
      return 1;
    }

    // We need an array to hold file info for the 2-pass system
    FileInfo *files = NULL;
    int file_count = 0;
    int capacity = 0;

    // Width tracking variables
    int max_links_w = 0;
    int max_user_w = 0;
    int max_group_w = 0;
    int max_size_w = 0;

    struct dirent *entry;
    
    // --- PASS 1: Collect Data and Calculate Max Widths ---
    while ((entry = readdir(dir))) {
      if (!show_all && entry->d_name[0] == '.')
        continue;

      if (file_count >= capacity) {
        capacity = capacity == 0 ? 32 : capacity * 2;
        files = realloc(files, capacity * sizeof(FileInfo));
      }

      FileInfo *f = &files[file_count];
      strncpy(f->name, entry->d_name, sizeof(f->name));

      if (long_format) {
        char fullpath[4096];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, f->name);
        
        if (lstat(fullpath, &f->st) < 0) {
          perror(f->name);
          continue; // Skip failed stats
        }

        mode_string(f->st.st_mode, f->modes);
        
        struct passwd *pw = getpwuid(f->st.st_uid);
        // FIX: Changed st_uid to st_gid for the group lookup
        struct group *gr = getgrgid(f->st.st_gid); 

        strncpy(f->user, pw ? pw->pw_name : "?", sizeof(f->user));
        strncpy(f->group, gr ? gr->gr_name : "?", sizeof(f->group));

        struct tm *tm = localtime(&f->st.st_mtim.tv_sec);
        strftime(f->timebuf, sizeof(f->timebuf), "%b %e %H:%M", tm);

        // Update maximum column widths dynamically
        int links_w = num_digits((long)f->st.st_nlink);
        int user_w  = strlen(f->user);
        int group_w = strlen(f->group);
        int size_w  = num_digits((long)f->st.st_size);

        if (links_w > max_links_w) max_links_w = links_w;
        if (user_w > max_user_w)   max_user_w = user_w;
        if (group_w > max_group_w) max_group_w = group_w;
        if (size_w > max_size_w)   max_size_w = size_w;
      }
      
      file_count++;
    }
    closedir(dir);

    // --- PASS 2: Print Everything with Dynamic Padding ---
    for (int j = 0; j < file_count; j++) {
      if (long_format) {
        // %*lu right-aligns links based on max_links_w
        // %-*s left-aligns user string based on max_user_w
        // %-*s left-aligns group string based on max_group_w
        // %*ld right-aligns size based on max_size_w
        printf("%s %*lu %-*s %-*s %*ld %s %s\n",
               files[j].modes,
               max_links_w, (unsigned long)files[j].st.st_nlink,
               max_user_w,  files[j].user,
               max_group_w, files[j].group,
               max_size_w,  (long)files[j].st.st_size,
               files[j].timebuf,
               files[j].name);
      } else {
        printf("%s\n", files[j].name);
      }
    }

    free(files);
  }
  return 0;
}
