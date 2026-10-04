#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
# include "kernel/fcntl.h"

int main(int argc, char *argv[]){
    char c;
    int number = 0;
    int in_number = 0;
    int fd;

    if (argc == 1){
        fd = 0;   // standard input
    }
    else{
        fd = open(argv[1], O_RDONLY);

        if (fd < 0){
            printf("sixfive: cannot open %s\n", argv[1]);
            exit(1);
        }
    }

    while (read(fd, &c, 1) == 1){
        if (c >= '0' && c <= '9'){
            number = number * 10 + (c - '0');
            in_number = 1;
        }

        else if (strchr(" -\r\t\n./,", c) != 0) {
            if(in_number){
                if (number % 5 == 0 || number % 6 == 0){
                    printf("%d\n", number);
            }
            number = 0;
            in_number = 0;
        }
    }

        else{
            number = 0;
            in_number = 0;
        }
    }

    // End of file acts like a separator
    if (in_number){
        if(number % 5 == 0 || number % 6 == 0){
            printf("%d\n", number);
        }
    }

    if (argc > 1)
        close(fd);

    exit(0);
};