#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){

    int pid; 
    int status;

    if(argc < 2){
        printf("Usage: runonce command [arg...]\n");
        exit(1);
    }

    pid  = fork();

    if(pid < 0){
        printf("Fork Failed!\n");
        exit(1);
    }

    if(pid == 0){
        exec(argv[1], &argv[1]);
        printf("exec failed: %s\n", argv[1]);
        exit(1);
    }
    else{
        int child_pid = wait(&status);

        if(child_pid < 0){
            printf("Wait failed!\n");
            exit(1);
        }

        printf("Child PID: %d, Exit status: %d\n", child_pid, status);
    }
    exit(0);
}