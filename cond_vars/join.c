#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

pthread_mutex_t lock;
pthread_cond_t cond;
int done = 0;

void *routine() {
  printf("Child started\n");
  pthread_mutex_lock(&lock);
  
  printf("Signaling to parent\n");
  done = 1;
  pthread_cond_signal(&cond);

  pthread_mutex_unlock(&lock);
  
  sleep(2);
  printf("Thread is done\n");
  return NULL;
}

int main() {
  // initialization
  pthread_t t1;

  int res = pthread_mutex_init(&lock, NULL);
  if (res != 0) { return 1; }

  res = pthread_cond_init(&cond, NULL);
  if (res != 0) { return 1; }

  res = pthread_create(&t1, NULL, &routine, NULL);
  if (res != 0) { return 1; }

  pthread_mutex_lock(&lock);
  while (done == 0) {
    printf("Waiting for thread to be done\n");
    pthread_cond_wait(&cond, &lock);
  }
  pthread_mutex_unlock(&lock);

  printf("Parent is done\n");
  return 0;
}
