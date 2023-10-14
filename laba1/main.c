#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main() {
    pid_t process_id;
    int pipe_[2];
    FILE *file;
    char filename[100];

    printf("enter the name of your file\n");
    scanf("%s", filename);


    if(pipe(pipe_) == -1){
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }

    process_id = fork();

    if(process_id == -1){
        perror("Error creating process_id");
        exit(EXIT_FAILURE);
    }

    if(process_id == 0){
        //дочерний процесс
        close(pipe_[0]);

        dup2(pipe_[1], STDOUT_FILENO);

        close(pipe_[1]);

        execlp("./child", "./child", filename, NULL);

        perror("Error run process_id");

        exit(EXIT_FAILURE);

    }


        //родительский процесс

        close(pipe_[1]);

        char buffer[1234];

        ssize_t bytes_read;

        while ((bytes_read = read(pipe_[0], buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytes_read);
        }

        close(pipe_[0]);

        wait(NULL);




    return 0;
}