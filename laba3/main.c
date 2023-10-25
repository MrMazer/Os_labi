#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    char filename[256];
    printf("Введите имя файла для чтения: ");
    scanf("%s", filename);

    int fd = open("shared_file", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    size_t file_size = sizeof(float);
    if (ftruncate(fd, file_size) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    float *shared_data = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Дочерний процесс

        // Открываем файл для чтения
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        float sum = 0;
        float num;

        while (fscanf(file, "%f", &num) != EOF) {
            sum += num;
        }

        fclose(file);

        // Записываем результат в отображенный файл
        *shared_data = sum;

        munmap(shared_data, file_size);
        close(fd);
        exit(EXIT_SUCCESS);
    } else {
        int status;
        waitpid(child_pid, &status, 0);

        float result = *shared_data;
        printf("Результат: %.2f\n", result);

        munmap(shared_data, file_size);
        close(fd);
        unlink("shared_file");
        exit(EXIT_SUCCESS);
    }
}
