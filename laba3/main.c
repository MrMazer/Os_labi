#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>

char SEM_NAME[] = "/my_semaphore";
char SHARED_FILE[] = "shared_file";
char FILE_NAME[256];
size_t FILE_SIZE = sizeof(float);

int main() {
    printf("Введите имя файла для чтения: ");
    scanf("%s", FILE_NAME);

    int file_descriptor = shm_open(SHARED_FILE, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (file_descriptor == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    size_t file_size = FILE_SIZE;
    if (ftruncate(file_descriptor, file_size) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    float *shared_data = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // cемафор
    sem_t *semaphore = sem_open(SEM_NAME, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
    if (semaphore == SEM_FAILED) {
        if (errno == EEXIST) {
            semaphore = sem_open(SEM_NAME, 0);
        } else {
            perror("semaphore_open");
            exit(EXIT_FAILURE);
        }
    }

    pid_t process_id = fork();

    if (process_id == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (process_id == 0) {
        execl("./child", "./child", FILE_NAME, (char *)NULL);
    } else {
        int status;
        waitpid(process_id, &status, 0);

        float result;
        sem_wait(semaphore); 
        result = *shared_data;
        printf("Результат: %.2f\n", result);

        sem_close(semaphore);
        sem_unlink(SEM_NAME); 
        munmap(shared_data, file_size);
        close(file_descriptor);
        shm_unlink(SHARED_FILE); 
        exit(EXIT_SUCCESS);
    }

    return 0;
}
