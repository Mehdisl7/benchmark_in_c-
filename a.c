#include <stdio.h>
int main(void) {
    long s = 0;
    for (long i = 0; i < 1000000; ++i) s += i;
    printf("%ld\n", s);
    return 0;
}
