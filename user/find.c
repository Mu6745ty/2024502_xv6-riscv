#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char *path, char *target) {
  int fd;
  struct stat st;
  struct dirent de;
  char buf[512];
  char *p;

  if ((fd = open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // File
  if (st.type == T_FILE) {
    char *name = path + strlen(path);

    while (name > path && *(name - 1) != '/')
      name--;

    if (strcmp(name, target) == 0)
      printf("%s\n", path);

    close(fd);
    return;
  }

  // Directory
  if (st.type == T_DIR) {

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {

      if (de.inum == 0)
        continue;

      if (strcmp(de.name, ".") == 0 ||
          strcmp(de.name, "..") == 0)
        continue;

      if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        fprintf(2, "find: path too long\n");
        break;
      }

      memmove(buf, path, strlen(path));

      p = buf + strlen(path);
      *p++ = '/';

      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = '\0';

      find(buf, target);
    }
  }

  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(2, "usage: find path name\n");
    exit(1);
  }

  find(argv[1], argv[2]);

  exit(0);
}