#include <stdio.h>

int calculate(int x, int y) {
    int result = x * 2;
    result = result + y;
    return result;
}

int main() {
    int a = 4;
    int b = 3;

    int c = calculate(a, b);
    int d = calculate(c, 2);

    printf("d = %d\n", d);
    return 0;
}