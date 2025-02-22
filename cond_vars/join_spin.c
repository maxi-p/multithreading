#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

volatile int done = 0;

void *routine() {
    printf("Started thread\n");
    sleep(5);
    done = 1;
    return NULL;
}

int main() {

    pthread_t t1;

    int res = pthread_create(&t1, NULL, &routine, NULL);
    if (res != 0) { return 1; }

    while (done == 0)
        ; // spin

    printf("Ended. But wasted a lot of CPU cycles\n");
    return 0;
}