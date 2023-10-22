#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>


typedef struct {
    float numbers[100]; // to do сделать произвольное число чисел
    int count;
    float sum;
} SharedData;

int main() {
    pid_t pid;
    SharedData *data;
    int fd;

    fd = shm_open("/my_shm", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }

    ftruncate(fd, sizeof(SharedData));

    data = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    data->count = 0;
    data->sum = 0;

    FILE *file;
        char filename[256];
        
        printf("Введите имя файла с данными: \n");
        if (scanf("%s", filename) != 1) {
            perror("scanf");
            exit(1);
        }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        

        file = fopen(filename, "r");
        if (!file) {
            perror("fopen");
            exit(1);
        }

        char line[256];
        float num;
        while (fgets(line, sizeof(line), file) != NULL) {
            if (sscanf(line, "%f", &num) == 1) {
                data->sum += num;
                data->count++;
            }
        }

        fclose(file);
    } else {
        wait(NULL);
        printf("Сумма чисел: %.2f\n", data->sum);
        munmap(data, sizeof(SharedData));
        shm_unlink("/my_shm");
    }

    return 0;
}
