#ifndef DEFINED_FD_QUEUE
#define DEFINED_FD_QUEUE

#include <pthread.h>
#include <stdlib.h>

typedef struct ts_queue_node
{
	void * data;
	struct ts_queue_node * next;
} ts_queue_node;

typedef struct ts_queue
{
	struct ts_queue_node * head;
	struct ts_queue_node * tail;
	pthread_mutex_t * lock;
} ts_queue;

ts_queue * ts_queue_create( void );

void ts_queue_push( ts_queue *, void * );
	
void * ts_queue_pop( ts_queue * );

void * ts_queue_try_pop( ts_queue * );

void * ts_queue_block_pop( ts_queue * );

#endif // DEFINED_FD_QUEUE
