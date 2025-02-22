#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

int done = 0;
pthread_mutex_t lock;
pthread_cond_t cond;

void *routine() {
    printf("Thread started\n");
    sleep(1);
    done = 1;
    printf("Child signal\n");
    pthread_cond_signal(&cond);
    return NULL;
}

int main() {
    pthread_t t1;

    int res = pthread_create(&t1, NULL, &routine, NULL);
    if (res != 0) { return 1; }
    
    res = pthread_mutex_init(&lock, NULL);
    if (res != 0) { return 1; }

    res = pthread_cond_init(&cond, NULL);
    if (res != 0) { return 1; }

    pthread_mutex_lock(&lock);
    while (done == 0) {
        printf("Parent wait to be signalled...\n");
        sleep(2);
        pthread_cond_wait(&cond, &lock);
    }
    pthread_mutex_unlock(&lock);

    printf("Parent ended but wasted a lot of cycles\n");
    return 0;
}