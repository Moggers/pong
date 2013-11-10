#ifndef DEFINED_ACCEPT_THREAD
#define DEFINED_ACCEPT_THREAD

#include <pthread.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include "debug.h"
#include "ts_queue.h"
#include "net_structs.h"

typedef struct accept_thread_args
{
	struct ts_queue * queue;
	char * port;
} accept_thread_args;

accept_thread_args * create_accept_thread_args( struct ts_queue *, char * );

pthread_t * create_accept_thread( struct ts_queue *, char * );

void * accept_thread_callback( void * );

#endif // DEFINED_ACCEPT_THREAD
