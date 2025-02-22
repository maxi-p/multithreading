#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/syscall.h>

void* routine() {
    pthread_t th = pthread_self();
    printf("th value: %ul\n", th);
    printf("gettid (OS thread id): %d\n", (pid_t) syscall(SYS_gettid)); // Macos doesn't expose this
    return NULL;
}

int main() {
    pthread_t t1;
    int res;

    res = pthread_create(&t1, NULL, &routine, NULL);
    if (res != 0) { return 1; }

    res = pthread_join(t1, NULL);
    if (res != 0) { return 1; }
    return 0;
}