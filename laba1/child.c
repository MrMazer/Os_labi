#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "Using: %s <file_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *filename = argv[1];
    FILE *file = fopen(filename, "r");

    if(!file){
        perror("error open file");
        exit(EXIT_FAILURE);
    }

    float sum = 0;
    float number;

    while(fscanf(file, "%f", &number) == 1){
        sum += number;
    }

    fclose(file);

    printf("sum of the numbers from the file %s: %.2f\n", filename, sum);
    
    return 0;
}