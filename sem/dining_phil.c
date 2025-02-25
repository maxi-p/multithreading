#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct __Zem_t {
    int value;
    pthread_cond_t  cond;
    pthread_mutex_t lock;
} Zem_t;

Zem_t sem;

void Zem_init(Zem_t *z, int value) {
    z->value = value;
    pthread_cond_init(&z->cond, NULL);
    pthread_mutex_init(&z->lock, NULL);
}

void Zem_wait(Zem_t *z) {
    pthread_mutex_lock(&z->lock);
    while (z->value <= 0)
	pthread_cond_wait(&z->cond, &z->lock);
    z->value--;
    pthread_mutex_unlock(&z->lock);
}

void Zem_post(Zem_t *z) {
    pthread_mutex_lock(&z->lock);
    z->value++;
    pthread_cond_signal(&z->cond);
    pthread_mutex_unlock(&z->lock);
}

void Zem_destroy(Zem_t *z) {
    pthread_cond_destroy(&z->cond);
    pthread_mutex_destroy(&z->lock);
}


typedef struct {
    int num_loops;
    int thread_id;
} arg_t;

Zem_t forks[5];
Zem_t print_lock;

void space(int s) {
    Zem_wait(&print_lock);
    int i;
    for (i = 0; i < s * 10; i++)
	printf(" ");
}

void space_end() {
    Zem_post(&print_lock);
}

int left(int p)  {
    return p;
}

int right(int p) {
    return (p + 1) % 5;
}

void get_forks(int p) {
    if (p == 4) {
	space(p); printf("4 try %d\n", right(p)); space_end();
	Zem_wait(&forks[right(p)]);
	space(p); printf("4 try %d\n", left(p)); space_end();
	Zem_wait(&forks[left(p)]);
    } else {
	space(p); printf("try %d\n", left(p)); space_end();
	Zem_wait(&forks[left(p)]);
	space(p); printf("try %d\n", right(p)); space_end();
	Zem_wait(&forks[right(p)]);
    }
}

void put_forks(int p) {
    Zem_post(&forks[left(p)]);
    Zem_post(&forks[right(p)]);
}

void think() {
    return;
}

void eat() {
    return;
}

void *philosopher(void *arg) {
    arg_t *args = (arg_t *) arg;

    space(args->thread_id); printf("%d: start\n", args->thread_id); space_end();

    int i;
    for (i = 0; i < args->num_loops; i++) {
	space(args->thread_id); printf("%d: think\n", args->thread_id); space_end();
	think();
	get_forks(args->thread_id);
	space(args->thread_id); printf("%d: eat\n", args->thread_id); space_end();
	eat();
	put_forks(args->thread_id);
	space(args->thread_id); printf("%d: done\n", args->thread_id); space_end();
    }
    return NULL;
}
                                                                             
int main(int argc, char *argv[]) {
    if (argc != 2) {
	    fprintf(stderr, "usage: dining_philosophers <num_loops>\n");
	    exit(1);
    }
    printf("dining: started\n");
    
    int i;
    for (i = 0; i < 5; i++) 
	Zem_init(&forks[i], 1);
    Zem_init(&print_lock, 1);

    pthread_t p[5];
    arg_t a[5];
    for (i = 0; i < 5; i++) {
	a[i].num_loops = atoi(argv[1]);
	a[i].thread_id = i;
	pthread_create(&p[i], NULL, philosopher, &a[i]);
    }

    for (i = 0; i < 5; i++) 
	pthread_join(p[i], NULL); 

    printf("dining: finished\n");
    return 0;
}