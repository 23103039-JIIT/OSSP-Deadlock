#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#define NUM_PHILOSOPHERS 5

sem_t forks[NUM_PHILOSOPHERS]; 
sem_t mutex;

void *philosopher(void *num) {
    int id = *(int *)num;

    while (1) {
        printf("Philosopher %d is thinking.\n", id);
        sleep(rand() % 3 + 1);

        printf("Philosopher %d is hungry.\n", id);

        sem_wait(&mutex);

        sem_wait(&forks[id]);
        printf("Philosopher %d picked up LEFT fork %d.\n", id, id);

        sem_wait(&forks[(id + 1) % NUM_PHILOSOPHERS]);
        printf("Philosopher %d picked up RIGHT fork %d.\n", id, (id + 1) % NUM_PHILOSOPHERS);

        sem_post(&mutex);

        printf("Philosopher %d is eating.\n", id);
        sleep(rand() % 2 + 1);

        sem_post(&forks[id]);
        sem_post(&forks[(id + 1) % NUM_PHILOSOPHERS]);

        printf("Philosopher %d finished eating and puts down both forks.\n", id);
        sleep(rand() % 2 + 1);
    }
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];
    srand(time(NULL));

    sem_init(&mutex, 0, 1);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) sem_init(&forks[i], 0, 1);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }
    
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) pthread_join(threads[i], NULL);

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) sem_destroy(&forks[i]);
    sem_destroy(&mutex);

    return 0;
}