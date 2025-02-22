#include<pthread.h>
#include<stdio.h>

int g_x = 0;
pthread_mutex_t lock;

void* routine () {
    for (int i = 0; i < 100000000; i++) {
        pthread_mutex_lock(&lock);
        g_x++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    int mut_res = pthread_mutex_init(&lock, NULL);

    if (mut_res != 0) { return 1; }

    int res1 = pthread_create(&t1, NULL, &routine, NULL);
    int res2 = pthread_create(&t2, NULL, &routine, NULL);

    if (res1 != 0) { return 2; }
    if (res2 != 0) { return 2; }

    res1 = pthread_join(t1, NULL);
    res2 = pthread_join(t2, NULL);

    if (res1 != 0) { return 3; }
    if (res2 != 0) { return 3; }

    mut_res = pthread_mutex_destroy(&lock);

    if (mut_res != 0) { return 4; }

    printf("The final value of the g_x: %d\n", g_x);

    return 0;
}