#include "library.h"
#include <stdlib.h>

int GCF(int A, int B) {
    while (B != 0) {
        int temp = B;
        B = A % B;
        A = temp;
    }
    return A;
}

char* translation(long x) {
    int base = 2;
    int bits = 64;
    char* result = (char*)malloc(bits + 1);
    result[bits] = '\0';

    for (int i = bits - 1; i >= 0; i--) {
        result[i] = (x % base) + '0';
        x /= base;
    }

    return result;
}
