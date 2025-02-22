#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

pthread_cond_t cond;
pthread_mutex_t lock;

void *routine() {
    printf("Thread begins\n");

    pthread_mutex_lock(&lock);

    printf("thead signals\n");
    pthread_cond_signal(&cond);

    pthread_mutex_unlock(&lock);
    
    return NULL;
}

int main() {
    pthread_t t1;
    int res = pthread_mutex_init(&lock, NULL);
    if (res != 0) { return 1; }

    res = pthread_cond_init(&cond, NULL);
    if (res != 0) { return 1; }

    res = pthread_create(&t1, NULL, &routine, NULL);
    if (res != 0) { return 1; }

    sleep(2);
    printf("Parent: wait to be signaled...\n");

    pthread_mutex_lock(&lock);
    pthread_cond_wait(&cond, &lock);
    pthread_mutex_unlock(&lock);

    printf("Parend end\n");
    return 0;
}