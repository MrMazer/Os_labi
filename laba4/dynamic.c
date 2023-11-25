#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

int main() {
    const char* libPath1 = "./library_first.so";
    const char* libPath2 = "./library_second.so";

    void *lib = dlopen(libPath1, RTLD_LAZY);

    if (!lib) {
        fprintf(stderr, "Error loading initial library.\n");
        return 1;
    }

    int (*gcfFunction)(int, int) = dlsym(lib, "GCF");
    char* (*translationFunction)(long) = dlsym(lib, "translation");

    if (!gcfFunction || !translationFunction) {
        fprintf(stderr, "Error getting function pointers.\n");
        dlclose(lib);
        return 1;
    }

    int choice;
    do {
        printf("\nEnter your choice:\n");
        printf("0 - Switch between implementations\n");
        printf("1  Call GCF function with arguments\n");
        printf("2  Call translation function with arguments\n");

        scanf("%d", &choice);

        switch (choice) {
            case 0:
                dlclose(lib);
                if (lib == dlopen(libPath1, RTLD_LAZY)) {
                    lib = dlopen(libPath2, RTLD_LAZY);
                } else {
                    lib = dlopen(libPath1, RTLD_LAZY);
                }

                if (!lib) {
                    fprintf(stderr, "Error loading library.\n");
                    return 1;
                }

                gcfFunction = dlsym(lib, "GCF");
                translationFunction = dlsym(lib, "translation");

                if (!gcfFunction || !translationFunction) {
                    fprintf(stderr, "Error getting function pointers.\n");
                    dlclose(lib);
                    return 1;
                }

                break;

            case 1: {
                printf("Enter two numbers\n");
                int arg1, arg2;
                scanf("%d %d", &arg1, &arg2);
                int result = gcfFunction(arg1, arg2);
                printf("GCF result: %d\n", result);
                break;
            }

            case 2: {
                printf("Enter number, which u would translate\n");
                long arg;
                scanf("%ld", &arg);
                char* result = translationFunction(arg);
                printf("Translation result: %s\n", result);
                free(result);
                break;
            }

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

    } while (choice != -1);

    dlclose(lib);

    return 0;
}
