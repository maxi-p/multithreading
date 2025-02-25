#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

int val = 0;

typedef struct __Zem_t {
    int value;
    pthread_cond_t  cond;
    pthread_mutex_t lock;
} Zem_t;

Zem_t sem;

void Zem_init(Zem_t *z, int value) {
    z->value = value;
    pthread_cond_init(&z->cond, NULL);
    pthread_mutex_init(&z->lock, NULL);
}

void Zem_wait(Zem_t *z) {
    pthread_mutex_lock(&z->lock);
    while (z->value <= 0)
	pthread_cond_wait(&z->cond, &z->lock);
    z->value--;
    pthread_mutex_unlock(&z->lock);
}

void Zem_post(Zem_t *z) {
    pthread_mutex_lock(&z->lock);
    z->value++;
    pthread_cond_signal(&z->cond);
    pthread_mutex_unlock(&z->lock);
}

void Zem_destroy(Zem_t *z) {
    pthread_cond_destroy(&z->cond);
    pthread_mutex_destroy(&z->lock);
}

void *routine() {
    for (int i = 0; i < 10000000; i++) {
        Zem_wait(&sem);
        val++;
        Zem_post(&sem);
    }
    return NULL;
}

int main() {
    Zem_init(&sem, 1);  // Initialize semaphore (binary semaphore, initial value = 1)
    
    pthread_t t1, t2;

    pthread_create(&t1, NULL, &routine, NULL);  // Create thread 1
    pthread_create(&t2, NULL, &routine, NULL);  // Create thread 2

    pthread_join(t1, NULL);  // Wait for thread 1 to finish
    pthread_join(t2, NULL);  // Wait for thread 2 to finish

    printf("end value: %d\n", val);  // Print the final value of val

    Zem_destroy(&sem);

    return 0;
}
