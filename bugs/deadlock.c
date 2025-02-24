#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock1;
pthread_mutex_t lock2;

void* threadA(void* arg) {
    printf("Thread A: Trying to acquire lock1...\n");
    pthread_mutex_lock(&lock1);
    printf("Thread A: Acquired lock1, now trying to acquire lock2...\n");

    printf("Thread A: Simulating a context switch (system interruption)\n");
    sleep(1);  

    pthread_mutex_lock(&lock2); 

    printf("Thread A: Acquired lock2, proceeding with task...\n");

    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);
    return NULL;
}

void* threadB(void* arg) {
    printf("Thread B: Trying to acquire lock2...\n");
    pthread_mutex_lock(&lock2);
    printf("Thread B: Acquired lock2, now trying to acquire lock1...\n");

    printf("Thread B: Simulating a context switch (system interruption)\n");
    sleep(1); 

    pthread_mutex_lock(&lock1); 

    printf("Thread B: Acquired lock1, proceeding with task...\n");

    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_create(&t1, NULL, threadA, NULL);
    pthread_create(&t2, NULL, threadB, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    return 0;
}
