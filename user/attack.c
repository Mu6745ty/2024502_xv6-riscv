
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PGSIZE 4096
#define NPAGES 64

int
main(int argc, char *argv[])
{
  char *mem = sbrk(NPAGES * PGSIZE);
  if (mem == (char *)-1)
    exit(1);

  uint64 addr = ((uint64)mem + PGSIZE - 1) & ~(PGSIZE - 1);
  char *start = (char *)addr;

  char marker[] = "Here it is: ";
  int marker_len = strlen(marker);
  int total = NPAGES * PGSIZE;

  for (int i = 0; i <= total - marker_len; i++) {
    int j = 0;

    while (j < marker_len && start[i + j] == marker[j])
      j++;

    if (j != marker_len)
      continue;

    char *p = start + i + marker_len;
    int len = 0;

    while (len < 128) {
      char c = p[len];

      if (!((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z')))
        break;

      len++;
    }

    if (len > 0 && len < 128 && p[len] == '\0') {
      write(1, p, len);
      write(1, "\n", 1);
      exit(0);
    }
  }

  exit(0);
}
