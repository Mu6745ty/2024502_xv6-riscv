#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void memdump(char *fmt, char *data, int len){
    char *p = data;

    while (*fmt != '\0'){

        if (*fmt == 'i') {
            if (len >= 4) {
                int value = *(int *)p;
                printf("%d\n", value);
                p += 4;
                len -= 4;
            }

            else{
                printf("memdump: not enough data for 'i'\n");
                exit(1);
            }
    }

        else if (*fmt == 'h') {
            if (len >= 2) {
                uint16 value = *(uint16 *)p;
                printf("%d\n", value);
                p += 2;
                len -= 2;
            }
            else{
                printf("memdump: not enough data for 'h'\n");
                exit(1);
            }
        }

        else if(*fmt == 'c') {
            if (len >= 1) {
                char value = *p;
                printf("%c\n", value);
                p += 1;
                len -= 1;
            }
            else {
                printf("memdump: not enough data for 'c'\n");
                exit(1);
            }
        }

        else if(*fmt == 'p') {
            if (len >= 8){
                uint64 value = *(uint64 *)p;
                printf("%lx\n", value);
                p += 8;
                len  -= 8;
            }
            else{
                printf("memdump: not enough data for 'p'\n");
                exit(1);
            }
        }

        else if (*fmt == 's') {
            if (len >= 8) {
                uint64 value = *(uint64 *)p;
                char *str = (char *)value;
                printf("%s\n", str);
                p += 8;
                len -= 8;
            }
            else{
                printf("memdump: not enough data for 's'\n");
                exit(1);
            }
        }
        fmt++;
    }
    
}


int main(int argc, char *argv[]){
    if (argc < 2) {
        printf("Usage: memdump format [data]\n");
        exit(1);
    }

    char buf[100];
    int n;

    if (argc == 2) {
        n = read(0, buf, sizeof(buf));
        memdump(argv[1], buf, n);
    }
    else {
        memdump(argv[1], argv[2], strlen(argv[2]));
    }


    exit(0);
}