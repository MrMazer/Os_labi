#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/time.h>

pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;

struct ThreadData{
    int* data;
    int size;
    int thread_id;
    int number_threads;
    int* minimum;
    int* maximun;
};

void* findMinimunMaximun(void* arg){
    struct ThreadData* thread_data = (struct ThreadData*)arg;
    int minimum = thread_data->data[0];
    int maximun = thread_data->data[0];

    for (int index = thread_data->thread_id; index < thread_data->size; index += thread_data-> number_threads){
        if(thread_data-> data[index]< minimum){
            minimum = thread_data -> data[index];
        }
        if(thread_data -> data[index] > maximun){
            maximun = thread_data->data[index];
        }
    }
    
    
    pthread_mutex_lock(&global_mutex);

    if(minimum < *(thread_data->minimum)){
        *(thread_data->minimum) = minimum;
    }

    if(maximun > *(thread_data->maximun)){
        *(thread_data->maximun) = maximun;
    }

    pthread_mutex_unlock(&global_mutex);

    pthread_exit(NULL);
}   


int main(int argc, char* argv[]){
    if(argc != 4){
        printf("Usage: %s <num_threads> <data_file> <array_size>\n", argv[0]);
        return 1;
    }

    int number_threads = atoi(argv[1]);
    const char* filename = argv[2];
    int size = atoi(argv[3]);

    int* data = (int*)malloc(sizeof(int) * size);

    FILE* file = fopen(filename, "r");

    if(file == NULL){
        perror("Error openning file");
        return 1;
    }

    for(int index = 0; index < size; index ++){
        fscanf(file, "%d", &data[index]);
    }
    fclose(file);

    int minimum = data[0];
    int maximum = data[0];

    pthread_t threads[number_threads];

    struct  ThreadData thread_data[number_threads];
   
    struct  timeval start, end;

    gettimeofday(&start, NULL);

    for(int index_of_thread = 0; index_of_thread < number_threads; index_of_thread++){
        thread_data[index_of_thread].data = data;
        thread_data[index_of_thread].size = size;
        thread_data[index_of_thread].thread_id = index_of_thread;
        thread_data[index_of_thread].number_threads = number_threads;
        thread_data[index_of_thread].minimum = &minimum;
        thread_data[index_of_thread].maximun = &maximum;
        pthread_create(&threads[index_of_thread], NULL, findMinimunMaximun, &thread_data[index_of_thread]);
    }
    
    for(int index_of_thread = 0; index_of_thread < number_threads; index_of_thread++){
        pthread_join(threads[index_of_thread], NULL);
    }

    gettimeofday(&end, NULL);
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e5;
   
   printf("Mininum: %d\n", minimum);
   printf("Maximum %d\n", maximum);
   printf("Execution time: %f seconds\n", time_taken);

   free(data);
   return 0;
}