#include "kernel/types.h"
#include "user/user.h"

int main(void)
{
    int n = getnproc();
    printf("Number of processes: %d\n", n);
    exit(0);
}