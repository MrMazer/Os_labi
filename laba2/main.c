#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include <limits.h>

#define MAX_THREADS 16
#define MAX_ARRAY_SIZE 20000000

int data[MAX_ARRAY_SIZE];
int array_size = 0;
int min_result = INT_MAX;
int max_result = INT_MIN;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int thread_count = 0;

void* find_min_max(void *arg){
    int thread_id = *((int*)arg);

    int chunk_size = array_size / thread_count;
    int start_index = thread_id * chunk_size;
    int end_index = (thread_id == (thread_count - 1)) ? array_size : (start_index+ chunk_size);

    int local_min = INT_MAX;
    int local_max = INT_MIN;

    for(int arr_index = start_index; arr_index < end_index; arr_index++){
        
        if(data[arr_index] < local_min){
            local_min = data[arr_index];
        }

        if(data[arr_index] > local_max){
            local_max = data[arr_index];
        }
    }
    pthread_mutex_lock(&mutex);

    if(local_min < min_result){
        min_result = local_min;
    }
    if(local_max > max_result){
        max_result = local_max;
    }
    
    

    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

int main(int argc, char* argv[]){
    if(argc != 3){
        printf("Using %s<filename> <number of threads>\n", argv[0]);
        return 1;
    }

    int number_of_threads = atoi(argv[2]);
    
    if(number_of_threads <= 0 || number_of_threads > MAX_THREADS){
        printf("incorrect numbers of threads. allowed value : from 1 to %d\n", MAX_THREADS);
    }

    clock_t start_time = clock();

    FILE* file = fopen(argv[1], "r");
    if(file == NULL){
        perror("error file open");
        return 1;
    }

    while (fscanf(file, "%d", &data[array_size]) != EOF && array_size < MAX_ARRAY_SIZE){
        array_size++;
    }

    fclose(file);

    pthread_t threads[MAX_THREADS];
    int thread_ids[MAX_THREADS];

    thread_count = number_of_threads;

    for(int thread_index = 0; thread_index < number_of_threads; thread_index++){
        thread_ids[thread_index] = thread_index;
        pthread_create(&threads[thread_index], NULL, find_min_max, &thread_ids[thread_index]);
    }
    
    for(int thread_index = 0; thread_index < number_of_threads; thread_index++){
        pthread_join(threads[thread_index], NULL);
    }

    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("minumum value: %d\n", min_result);
    printf("max value: %d\n", max_result);
    printf("Number of threads used: %d\n", thread_count);
    printf("Program execution time: %.2f s\n", execution_time);
    
    return 0;
}