#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]){

    int n;

    if (argc != 2){
        printf("Wrong Usage: Sleep seconds\n");
        exit(1);
    }

    n = atoi(argv[1]);

    pause(n);

    exit(0);
}