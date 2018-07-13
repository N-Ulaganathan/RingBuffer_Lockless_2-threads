#ifndef RINGBUFFER_H_INCLUDED
#define RINGBUFFER_H_INCLUDED

#define MAXMSGLEN 1024

typedef struct message
{
    char buffer[MAXMSGLEN];
    int read_flag;
} message;

typedef struct ring_buffer
{
    struct message* messages;
    int max_size;
    int size;
    int sequence;
    int head;
    int tail;
} ring_buffer;

void rb_init(ring_buffer* rb, size_t max_size)
{
    if(!rb)
    {
        printf("alloc memory for ring buffer first\n");
        exit(-1);
    }
    rb->messages = (message*)malloc(max_size * sizeof(message));
    if(rb->messages == NULL)
    {
        printf("Unable to alloc memory for ring buffer messages\n");
        exit(-1);
    }
    rb->max_size=max_size;
    rb->size = rb->sequence = 0;
    rb->head = rb->tail = -1;
}

void rb_write(ring_buffer* rb, const char* buffer)
{
    rb->head = (rb->head+1) % rb->max_size;
    while(rb->messages[rb->head].read_flag==1)
        rb->head = (rb->head+1) % rb->max_size;
    sprintf(rb->messages[rb->head].buffer, "%s", buffer);
    rb->messages[rb->head].read_flag=1;
    rb->size++;
    rb->sequence++;
}

void rb_read(ring_buffer* rb, char* buffer)
{
    rb->tail = (rb->tail+1) % rb->max_size;
    while(rb->messages[rb->tail].read_flag!=1)
        rb->tail = (rb->tail+1) % rb->max_size;
    sprintf(buffer, "%s", rb->messages[rb->tail].buffer);
    rb->messages[rb->tail].read_flag=2;
    rb->size--;
}

#endif // RINGBUFFER_H_INCLUDED
