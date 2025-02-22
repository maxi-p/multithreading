#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

pthread_mutex_t lock;
pthread_cond_t cv;

// produce: 2 * 5 * 16 = 10 * 16 = 160
// consume: 4 * 40 = 160

// fuels the gas variable (adds numbers)
void *fuelStation(void* arg) {
  for (int i = 0; i < 5; i++) {
    pthread_mutex_lock(&lock);

    printf("Adding 16 gallons... ");
    *(int*)arg += 16;
    printf("Now gas: %d\n", *(int*)arg);
    pthread_cond_broadcast(&cv);

    pthread_mutex_unlock(&lock);
    sleep(1);
  }

  return NULL;
}

// consumes gas from the gas variable (subtracts)
void *carThread(void* arg) {
  pthread_mutex_lock(&lock);

  while (*(int*)arg < 40) {
    printf("Waiting for enough fuel\n");
    pthread_cond_wait(&cv, &lock);
  }
  printf("Consuming the 40 gallons... ");
  *(int*)arg -= 40;
  printf("%d remaining\n", *(int*)arg);

  pthread_mutex_unlock(&lock);
  return NULL;
}

int main() {
  pthread_t station[2];
  pthread_t car[4];
  int* gas = NULL;
  int res;

  gas = (int*) malloc(sizeof(int));
  if (gas == NULL) { return 2; }
  
  res = pthread_mutex_init(&lock, NULL);
  if (res != 0) { return 1; }
  res = pthread_cond_init(&cv, NULL);
  if (res != 0) { return 1; }
  
  for (int i = 0; i < 2; i++) {
    res = pthread_create(&station[i], NULL, &fuelStation, gas);
    if (res != 0) { return 1; }
  }

  for (int i = 0; i < 4; i++) {
    res = pthread_create(&car[i], NULL, &carThread, gas);
    if (res != 0) { return 1; }
  }

  for (int i = 0; i < 2; i++) {
    res = pthread_join(station[i], NULL);
    if (res != 0) { return 1; }
  }

  for (int i = 0; i < 4; i++) {
    res = pthread_join(car[i], NULL);
    if (res != 0) { return 1; }
  }

  printf("End.. Remaining gas: %d\n", *gas);
  free(gas);
  return 0;
}
