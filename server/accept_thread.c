#include "accept_thread.h"

accept_thread_args * create_accept_thread_args( ts_queue * queue, char * port )
{
	accept_thread_args * ptr = malloc( sizeof( accept_thread_args ) );
	ptr->port = port;
	ptr->queue = queue;

	return ptr;
}

void * accept_thread_callback( void * args )
{
	debug_printf( "Enter listen thread\n" );
	struct addrinfo * res;
	struct addrinfo hints;
	memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	int status;
	status = getaddrinfo( NULL, ((accept_thread_args*)args)->port, &hints, &res );
	if( status != 0 )
	{
		debug_printf( "Failure in getaddrinfo: %s\n", gai_strerror( status ) );
		pthread_exit( NULL );
	}

	int sockfd;
	if( ( sockfd = socket( res->ai_family, res->ai_socktype, res->ai_protocol ) ) == -1 )
	{
		debug_printf( "Failure in socket: %s\n", gai_strerror( errno ) );
		pthread_exit( NULL );
	}

	if( bind( sockfd, res->ai_addr, res->ai_addrlen ) == -1 )
	{
		debug_printf( "Failure in bind: %s\n", gai_strerror( errno ) );
		pthread_exit( NULL );
	}
	if( listen( sockfd, 5 ) == -1 )
	{
		debug_printf( "Failure in listen %s\n", gai_strerror( errno ) );
		pthread_exit( NULL );
	}

	debug_printf( "Created listen socket\n" );
	for( ;; )
	{
		struct addrinfo their_address;
		memset( &their_address, 0, sizeof( their_address ) );
		debug_printf( "Waiting for connection\n" );
		int new_fd = accept( sockfd, their_address.ai_addr, &their_address.ai_addrlen );
		debug_printf( "Received a connection\n" );
		generic_packet * packet = create_client_request( new_fd );
		ts_queue_push( ((accept_thread_args*)args)->queue, packet );
		debug_printf( "Completed connection accept\n" );
	}
} 

pthread_t * create_accept_thread( struct ts_queue * queue, char * port )
{
	pthread_t * ptr = malloc( sizeof( pthread_t ) );
	pthread_create( ptr, NULL, &accept_thread_callback, create_accept_thread_args( queue, port ) );

	return ptr;
}
