#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



int main() {
    // Открываем pipe для записи
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Создаем дочерний процесс
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Дочерний процесс

        // Закрываем стандартный ввод дочернего процесса
        close(STDIN_FILENO);

        // Дублируем pipe для стандартного ввода
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[1]);

        // Выполняем вычисления
        float sum = 0;
        float num;

        while (scanf("%f", &num) != EOF) {
            sum += num;
        }

        printf("%.2f\n");
        exit(EXIT_SUCCESS);
    } else {
        // Родительский процесс

        // Закрываем стандартный вывод дочернего процесса
        close(STDOUT_FILENO);

        // Дублируем pipe для стандартного вывода
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[0]);

        // Читаем данные из файла и отправляем их в дочерний процесс
        FILE *file = fopen("input.txt", "r");

        if (file == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        float num;

        while (fscanf(file, "%f", &num) != EOF) {
            printf("%.2f\n", num);
        }

        fclose(file);
        close(pipe_fd[1]);

        int status;
        waitpid(child_pid, &status, 0);
        exit(EXIT_SUCCESS);
    }
}
