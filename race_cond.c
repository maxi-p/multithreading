#include<stdio.h>
#include<pthread.h>

int g_x = 0;

void* routine() {
    for(int i = 0; i < 1000000; i++) {
        g_x++;
    }
    return NULL;
}

int main() {
    pthread_t p1, p2;

    int res1 = pthread_create(&p1, NULL, &routine, NULL);
    int res2 = pthread_create(&p2, NULL, &routine, NULL);
    
    if (res1 != 0) { return 1; }
    if (res2 != 0) { return 1; }
    
    res1 = pthread_join(p1, NULL);
    res2 = pthread_join(p2, NULL);

    if (res1 != 0) { return 2; }
    if (res2 != 0) { return 2; }

    printf("The final value of x: %d\n", g_x);

    return 0;
}