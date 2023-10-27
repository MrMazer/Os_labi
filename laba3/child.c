#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>


char SEM_NAME[] = "/my_semaphore";
char SHARED_FILE[] = "shared_file";

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <имя_файла>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *file_name = argv[1];

    
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    float sum = 0;
    float number;

    while (fscanf(file, "%f", &number) != EOF) {
        sum += number;
    }

    fclose(file);

    int file_descriptor = open(SHARED_FILE, O_RDWR);
    if (file_descriptor == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    size_t file_size = sizeof(float);
    float *shared_data = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    *shared_data = sum;

    // Открываем семафор
    sem_t *semaphore = sem_open(SEM_NAME, 0);
    if (semaphore == SEM_FAILED) {
        perror("semaphore_open");
        exit(EXIT_FAILURE);
    }

    sem_post(semaphore); 

    munmap(shared_data, file_size);
    close(file_descriptor);

    exit(EXIT_SUCCESS);
    
    return 0;
}
