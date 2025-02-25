#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t read_cond;
    pthread_cond_t write_cond;
    int readers;   // Count of active readers
    int writing;   // Boolean indicating if a writer is currently writing
} rw_lock_t;

void rw_lock_init(rw_lock_t* lock) {
    pthread_mutex_init(&lock->mutex, NULL);
    pthread_cond_init(&lock->read_cond, NULL);
    pthread_cond_init(&lock->write_cond, NULL);
    lock->readers = 0;
    lock->writing = 0;
}

void rw_lock_destroy(rw_lock_t* lock) {
    pthread_mutex_destroy(&lock->mutex);
    pthread_cond_destroy(&lock->read_cond);
    pthread_cond_destroy(&lock->write_cond);
}

void rw_lock_reader_acquire(rw_lock_t* lock) {
    pthread_mutex_lock(&lock->mutex);

    while (lock->writing) {
        pthread_cond_wait(&lock->read_cond, &lock->mutex);
    }

    lock->readers++;
    pthread_mutex_unlock(&lock->mutex);
}

void rw_lock_reader_release(rw_lock_t* lock) {
    pthread_mutex_lock(&lock->mutex);

    lock->readers--; 
    if (lock->readers == 0) {
        pthread_cond_signal(&lock->write_cond); 
    }

    pthread_mutex_unlock(&lock->mutex);
}

void rw_lock_writer_acquire(rw_lock_t* lock) {
    pthread_mutex_lock(&lock->mutex);

    while (lock->writing || lock->readers > 0) {
        pthread_cond_wait(&lock->write_cond, &lock->mutex);
    }

    lock->writing = 1; 
    pthread_mutex_unlock(&lock->mutex);
}

void rw_lock_writer_release(rw_lock_t* lock) {
    pthread_mutex_lock(&lock->mutex);

    lock->writing = 0;
    
    pthread_cond_signal(&lock->read_cond);
    pthread_cond_signal(&lock->write_cond);

    pthread_mutex_unlock(&lock->mutex);
}

void* reader(void* arg) {
    rw_lock_t* lock = (rw_lock_t*)arg;
    rw_lock_reader_acquire(lock);

    printf("Reader is reading...\n");

    // 50% sleep chance
    srand(time(NULL));
    if (rand() % 2 == 1) {
        printf("Going to sleep...zzz...\n");
        sleep(1);
    }
    
    rw_lock_reader_release(lock);

    return NULL;
}

void* writer(void* arg) {
    rw_lock_t* lock = (rw_lock_t*)arg;
    
    rw_lock_writer_acquire(lock);
    printf("Writer is writing...\n");
    rw_lock_writer_release(lock);

    return NULL;
}

int main() {
    pthread_t readers[5], writers[2];
    rw_lock_t lock;

    rw_lock_init(&lock);

    for (int i = 0; i < 5; i++) {
        pthread_create(&readers[i], NULL, reader, &lock);
    }

    for (int i = 0; i < 2; i++) {
        pthread_create(&writers[i], NULL, writer, &lock);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(writers[i], NULL);
    }

    rw_lock_destroy(&lock);
    return 0;
}
