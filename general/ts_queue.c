#include "ts_queue.h"
#include "debug.h"

ts_queue * ts_queue_create( void )
{
	struct ts_queue * new_queue = malloc( sizeof( ts_queue ) );
	new_queue->head = NULL;
	new_queue->tail = NULL;
	new_queue->lock = malloc( sizeof( pthread_mutex_t ) );
	pthread_mutex_init( new_queue->lock, NULL );

	return new_queue;
}

void ts_queue_push( ts_queue * targ, void * data )
{
	pthread_mutex_lock( targ->lock );

	ts_queue_node * node = malloc( sizeof( ts_queue_node ) );
	node->data = data;
	node->next = NULL;

	if( targ->tail == NULL )
	{
		targ->tail = node;
		targ->head = node;
	}
	else
	{
		targ->tail->next = node;
		targ->tail = node;
	}

	pthread_mutex_unlock( targ->lock );
}
	
void * ts_queue_pop( ts_queue * targ ) 
{
	pthread_mutex_lock( targ->lock );

	if( targ->head == NULL )
		exit( 1 );
	if( targ->head == targ->tail )
		targ->tail = NULL;
	void * data = targ->head->data;
	ts_queue_node * node = targ->head;
	targ->head = targ->head->next;
	free( node );

	pthread_mutex_unlock( targ->lock );
	return data;
}

void * ts_queue_try_pop( ts_queue * targ ) 
{
	pthread_mutex_lock( targ->lock );

	if( targ->head == NULL )
	{
		pthread_mutex_unlock( targ->lock );
		return NULL;
	}
	if( targ->head == targ->tail )
		targ->tail = NULL;
	void * data = targ->head->data;
	ts_queue_node * node = targ->head;
	targ->head = targ->head->next;
	free( node );

	pthread_mutex_unlock( targ->lock );
	return data;
}

void * ts_queue_block_pop( ts_queue * targ ) 
{
	for( ;; ) // Mem: This could hog the mutex, blocking other threads with congestion
	{
		pthread_mutex_lock( targ->lock );
		if( targ->head != NULL ) break;
		pthread_mutex_unlock( targ->lock );
	}

	if( targ->head == targ->tail )
		targ->tail = NULL;
	void * data = targ->head->data;
	ts_queue_node * node = targ->head;
	targ->head = targ->head->next;
	free( node );

	pthread_mutex_unlock( targ->lock );
	return data;
}
