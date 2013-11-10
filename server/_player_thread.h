#ifndef DEFINED_PLAYER_THREAD
#define DEFINED_PLAYER_THREAD

#include "ts_queue.h"

typedef struct player_thread_args
{
	ts_queue * queue;
	int sockfd;
	int ply_id;
} player_thread_args;

void * player_thread_callback( void * args );

#endif // DEFINED_PLAYER_THREAD
