#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

int main() {
    const char* lib_Path_1 = "./library_first.so";
    const char* lib_Path_2 = "./library_second.so";

    void *library = dlopen(lib_Path_1, RTLD_LAZY);

    if (!library) {
        fprintf(stderr, "Error loading initial library.\n");
        return 1;
    }

    int (*gcf_Function)(int, int) = dlsym(library, "GCF");
    char* (*translation_Function)(long) = dlsym(library, "translation");

    if (!gcf_Function || !translation_Function) {
        fprintf(stderr, "Error getting function pointers.\n");
        dlclose(library);
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
                dlclose(library);
                if (library == dlopen(lib_Path_1, RTLD_LAZY)) {
                    library = dlopen(lib_Path_2, RTLD_LAZY);
                } else {
                    library = dlopen(lib_Path_1, RTLD_LAZY);
                }

                if (!library) {
                    fprintf(stderr, "Error loading library.\n");
                    return 1;
                }

                gcf_Function = dlsym(library, "GCF");
                translation_Function = dlsym(library, "translation");

                if (!gcf_Function || !translation_Function) {
                    fprintf(stderr, "Error getting function pointers.\n");
                    dlclose(library);
                    return 1;
                }

                break;

            case 1: {
                printf("Enter two numbers\n");
                int arg1, arg2;
                scanf("%d %d", &arg1, &arg2);
                int result = gcf_Function(arg1, arg2);
                printf("GCF result: %d\n", result);
                break;
            }

            case 2: {
                printf("Enter number, which u would translate\n");
                long arg;
                scanf("%ld", &arg);
                char* result = translation_Function(arg);
                printf("Translation result: %s\n", result);
                free(result);
                break;
            }

            case 3:
                printf("Exiting the program.\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

    } while (choice != 3);

    dlclose(library);

    return 0;
}
