#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

int g_nums[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

void* routine(void* arg) {
    sleep(1);
    int index = *(int*)arg;
    int num = g_nums[index];
    *(int*)arg = num;
    return (void*) arg;
}

int main() {
    pthread_t th[10];
    int* arg_ret;

    for (int i = 0; i < 10; i++) {
        arg_ret = (int*) malloc(sizeof(int));
        *arg_ret = i;
        if (pthread_create(&th[i], NULL, &routine, arg_ret) != 0) {
            printf("Error creating the thread #%d\n", i);
            return 1;
        }
    }

    for (int i = 0; i < 10; i++) {
        arg_ret = NULL;
        if (pthread_join(th[i], (void**) &arg_ret) != 0) {
            printf("Error ending the thread #%d\n", i);
            return 2;
        }
        if (arg_ret != NULL) { 
            printf("%d ", *(int*)arg_ret);
            free(arg_ret); 
        }
    }

    return 0;
}