#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

int exec_mode = 0;
int cmd_argc = 0;
char *cmd_argv[MAXARG];

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

    if(strcmp(name, target) == 0){
      if(exec_mode == 0){
        printf("%s\n", path);
      } else {
        int pid; 
        int i;
        char *exec_argv[MAXARG];

        for(i = 0; i < cmd_argc; i++){
          exec_argv[i] = cmd_argv[i];
        }

        exec_argv[cmd_argc] = path;
        exec_argv[cmd_argc + 1] = 0;

        pid = fork();

      if (pid < 0) {
        fprintf(2, "find: fork failed\n");
      } else if (pid == 0) {
        exec(cmd_argv[0], exec_argv);
        fprintf(2, "find: exec failed\n");
        exit(1);
      } else {
        wait(0);
      }
    }
  }

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
  int i;
  int exec_index = -1;

  if (argc < 3) {
    fprintf(2, "usage: find path name [-exec command...]\n");
    exit(1);
  }

  for (i = 3; i < argc; i++){
    if(strcmp(argv[i], "-exec") == 0){
      exec_index = i;
      break;
    }
  }

  if(exec_index >= 0){
    if(exec_index + 1 >= argc){
      fprintf(2, "find: missing command\n");
      exit(1);
    }

    exec_mode = 1;
    cmd_argc = argc - exec_index - 1;

    if(cmd_argc + 2 > MAXARG){
      fprintf(2, "find: too many arguments\n");
      exit(1);
    }

    for (i = 0; i < cmd_argc; i++){
      cmd_argv[i] = argv[exec_index + 1 + i];
    }
  }

  find(argv[1], argv[2]);

  exit(0);
}