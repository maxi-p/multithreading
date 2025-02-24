#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>

#define BUF_SIZE 10

int buffer[BUF_SIZE];
int fill_ptr;
int use_ptr;
int count = 0;

pthread_mutex_t lock;
pthread_cond_t producer, consumer;

void put (int value) {
    buffer[fill_ptr] = value;
    fill_ptr = (fill_ptr + 1) % BUF_SIZE;
    count++;
}

int get() {
    int temp = buffer[use_ptr];
    use_ptr = (use_ptr + 1) % BUF_SIZE;
    count--;
    return temp;
}

void *producer_t(void *arg) {
    int i;
    int loops = *(int *) arg;
    for (i = 0; i < loops; i++) {
        pthread_mutex_lock(&lock);
        while (count == BUF_SIZE) {
            pthread_cond_wait(&producer, &lock);
        }
        put(i);
        pthread_cond_signal(&consumer);
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

void *consumer_t(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (count == 0) {
            pthread_cond_wait(&consumer, &lock);
        }
        int tmp = get();
        pthread_cond_signal(&producer);
        printf("consumed: %d\n", tmp);
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

void *thread() {
    return NULL;
}
int main() {
    int loops = 10;
    pthread_t prod, cons[2]; 

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&producer, NULL);
    pthread_cond_init(&consumer, NULL);

    if (pthread_create(&prod, NULL, &producer_t, &loops) != 0) { return 1; }
    for (int i = 0; i < 2; i++) {
        if (pthread_create(&cons[i], NULL, &consumer_t, NULL) != 0) { return 1; }
    }

    pthread_join(prod, NULL);
    for (int i = 0; i < 2; i++) {
        pthread_join(cons[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&producer);
    pthread_cond_destroy(&consumer);

    return 0;
}