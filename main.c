#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <Windows.h>

#include "ringbuffer.h"

int MAXRINGSIZE=10, MAXMSGCOUNT=1000;


typedef struct thread_arg
{
    struct ring_buffer* rb;
    int tid;
} thread_arg;

void* producer(void* _arg)
{
    thread_arg* arg= (thread_arg*)_arg;
    char buffer[1024];
    int x=0;
    while(x<MAXMSGCOUNT)
    {
        sprintf(buffer, "%d-Text%d", arg->tid, x);
        if(!rb_write(arg->rb, buffer))
        {
            printf("PRODUCER THREAD (%d): %s <-- h: %d  >> X: %d\n", arg->tid, buffer, arg->rb->head, x);
        }
        x++;
    }
    return 0;
}

void* consumer(void* _arg)
{
    thread_arg* arg= (thread_arg*)_arg;
    char buffer[MAXMSGLEN];
    int x=0;
    while(x<MAXMSGCOUNT)
    {
        if(!rb_read(arg->rb, buffer))
        {
            printf("CONSUMER THREAD (%d): %s --> t: %d  >> X:%d\n", arg->tid, buffer, arg->rb->tail, x);
        }
        x++;
    }
    return 0;
}

void simulate_single(ring_buffer* rb)
{
    pthread_t cons1, prod1;
    thread_arg *arg1, *arg2;
    arg1 = malloc(sizeof(thread_arg));
    arg1->rb=rb;
    arg1->tid=1;
    arg2 = malloc(sizeof(thread_arg));
    arg2->rb=rb;
    arg2->tid=2;
    pthread_create(&prod1, NULL, producer, (void*)arg1);
    pthread_create(&cons1, NULL, consumer, (void*)arg2);
    pthread_join(prod1, NULL);
    pthread_join(cons1, NULL);
}

int main()
{
    ring_buffer* rb;
    rb=malloc(sizeof(ring_buffer));
    rb_init(rb, MAXRINGSIZE);
    simulate(rb);
    return 0;
}
