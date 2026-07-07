#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <string.h>

int main() {
    DIR *dir = opendir("/var/log");
    struct dirent *entry;
    struct stat statbuf;
    char full_path[1024];

    if (!dir) return 1;

    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // 1. Construct the full path
        snprintf(full_path, sizeof(full_path), "/var/log/%s", entry->d_name);

        // 2. Get standard stats
        if (lstat(full_path, &statbuf) == -1) continue;

        // 3. Check for ACLs using llistxattr
        int has_acl = 0;
        
        // Pass a buffer size of 0 to see if any extended attributes exist
        ssize_t xattr_size = llistxattr(full_path, NULL, 0);
        if (xattr_size > 0) {
            char list[xattr_size];
            llistxattr(full_path, list, xattr_size);
            
            // Look for the specific POSIX ACL attribute keys
            char *key = list;
            while (key < list + xattr_size) {
                if (strcmp(key, "system.posix_acl_access") == 0 ||
                    strcmp(key, "system.posix_acl_default") == 0) {
                    has_acl = 1;
                    break;
                }
                key += strlen(key) + 1;
            }
        }

        // 4. Print your custom ls -l output
        // (Simplified example: prints '+' if ACL exists)
        printf("%s%s\n", entry->d_name, has_acl ? "+" : "");
    }

    closedir(dir);
    return 0;
}
