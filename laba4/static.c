#include "library.h"
#include <stdio.h>
#include <stdlib.h>

// Реализация функции GCF с использованием алгоритма Евклида
int GCF(int A, int B) {
    while (B != 0) {
        int temp = B;
        B = A % B;
        A = temp;
    }
    return A;
}

// Реализация функции translation для перевода в двоичную систему
char* translation(long x) {
    int base = 2;
    int bits = 64; // Рассмотрим 64 бита для long
    char* result = (char*)malloc(bits + 1);
    result[bits] = '\0';

    for (int i = bits - 1; i >= 0; i--) {
        result[i] = (x % base) + '0';
        x /= base;
    }

    return result;
}

int main() {
    char command;
    int arg1, arg2;
    long argLong;

    while (1) {
        printf("Enter command (or 0 to exit): ");
        scanf(" %c", &command);

        switch (command) {
            case '1':
                printf("Enter arguments for GCF: ");
                scanf("%d %d", &arg1, &arg2);
                printf("Result of GCF: %d\n", GCF(arg1, arg2));
                break;
            case '2':
                printf("Enter argument for translation: ");
                scanf("%ld", &argLong);
                printf("Result of translation: %s\n", translation(argLong));
                break;
            case '0':
                printf("Exiting the program.\n");
                return 0;
            default:
                printf("Invalid command. Try again.\n");
                break;
        }
    }

    return 0;
}