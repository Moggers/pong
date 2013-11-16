#include "connect.h"
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include "../general/ts_queue.h"
#include "../general/debug.h"
#include "../general/net_structs.h"
#include <errno.h>

typedef struct connect_thread_args
{
	char * address;
	char * service;
	ts_queue * upstream;
} connect_thread_args;

void * connect_thread_callback( void * args )
{
	debug_printf( "Enter server connect thread\n" );
	struct addrinfo* sv;
	struct addrinfo hints;
	memset( &hints, 0, sizeof( hints ) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	int status;
	status = getaddrinfo( ((connect_thread_args*)args)->address, ((connect_thread_args*)args)->service, &hints, &sv );
	if( status != 0 )
	{
		debug_printf( "Failure in getaddrinfo: %s\n", gai_strerror( status ) );
		pthread_exit( NULL );
	}

	if( (g_server_sockfd = socket( sv->ai_family, sv->ai_socktype, sv->ai_protocol ) ) == -1 )
	{
		debug_printf( "Failure in socket: %s\n", gai_strerror( errno ) );
		pthread_exit( NULL );
	}

	if( connect( g_server_sockfd, sv->ai_addr, sv->ai_addrlen ) == -1 )
	{
		debug_printf( "Failure in connect: %s\n", gai_strerror( errno ) );
		pthread_exit( NULL );
	}
	
	debug_printf( "Completed network initialization, now blocking\n" );
	

	for( ;; )
	{
		generic_packet * packet;
		packet = malloc( sizeof( generic_packet ) );
		memset( packet, 0, sizeof( generic_packet ) );
		int status = recv( g_server_sockfd, packet, sizeof( generic_packet ), 0 ); 
		if( status == -1 )
		{
			debug_printf( "Failed to receive packet: %s\n", gai_strerror( errno ) );
			continue;
		}
		if( status == 0 )
		{
			debug_printf( "Disconnected from serber\n" );
			pthread_exit( 0 );
		}
		ts_queue_push( ((connect_thread_args*)args)->upstream, packet );
	}
}

int connect_to_server( char * address, char * service, ts_queue * upstream )
{
	debug_printf( "Connecting to server\n" );
	connect_thread_args * args = malloc( sizeof( connect_thread_args ) );
	args->address = address;
	args->service = service;
	args->upstream = upstream;
	pthread_t c_thread;
	pthread_create( &c_thread, 0, &connect_thread_callback, (void*)args );
}

void send_to_server( generic_packet * packet )
{
	if( send( g_server_sockfd, packet, sizeof( generic_packet ), 0 ) == -1 )
	{
		debug_printf( "Failed to send packet: %s\n", gai_strerror( errno ) );
	}
}
