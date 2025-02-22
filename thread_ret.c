#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>

void* routine() {
    srand(time(NULL));
    int num = rand() % 5 + 1;
    int* res = malloc(sizeof(int));
    *res = num;
    printf("The random number is: %d\n", num);
    return (void*) res;
}

int main() {
    pthread_t t1;
    int* ret;

    int res = pthread_create(&t1, NULL, &routine, NULL);
    if (res != 0) { return 1; }

    res = pthread_join(t1, (void**) &ret);
    if (res != 0) { return 2; }

    printf("The result is: %d\n", *ret);
    if (ret != NULL) { free(ret); }

    return 0;
}