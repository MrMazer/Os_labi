#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

char SEM_NAME[] = "/my_semaphore";
char SHARED_FILE[] = "shared_file";

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <имя_файла>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *file_name = argv[1];

   
    int file_descriptor = shm_open(SHARED_FILE, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (file_descriptor == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    
    size_t file_size = sizeof(float);
    if (ftruncate(file_descriptor, file_size) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    
    float *shared_data = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    
    float sum = 0;
    float number;

    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%f", &number) != EOF) {
        sum += number;
    }

    fclose(file);

    
    *shared_data = sum;

    
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
