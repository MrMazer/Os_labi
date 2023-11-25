#include "library.h"
#include <stdlib.h>

int GCF(int A, int B) {
    int min = (A < B) ? A : B;
    int gcf = 1;

    for (int i = 1; i <= min; i++) {
        if (A % i == 0 && B % i == 0) {
            gcf = i;
        }
    }

    return gcf;
}

char* translation(long x) {
    int base = 3;
    int bits = 64;
    char* result = (char*)malloc(bits + 1);
    result[bits] = '\0';

    for (int i = bits - 1; i >= 0; i--) {
        result[i] = (x % base) + '0';
        x /= base;
    }

    return result;
}
