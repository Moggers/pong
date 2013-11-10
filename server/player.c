#include "player.h"

player_queue * player_queue_create( void )
{
	player_queue * ptr = malloc( sizeof( player_queue ) );
	ptr->head = NULL;
	ptr->tail = NULL;
	return ptr;
}

void player_queue_push( player_queue * targ, player * src )
{
	if( targ->head == NULL )
	{
		targ->head = src;
		targ->tail = src;
		return;
	}

	targ->head->next = src;
	targ->head = src;
}

player * player_queue_pop( player_queue * src )
{
	player * ptr;
	ptr = src->tail;
	if( src->tail == src->head )
	{
		src->tail = NULL;
		src->head = NULL;
		return ptr;
	}
	src->tail = src->tail->next;
	return ptr;
}

player * player_queue_search_by_fd( player_queue * queue, int key )
{
	player * cur = queue->tail;
	while( cur->next != NULL && cur->sockfd != key )
		cur = cur->next;

	if( cur->sockfd == key )
		return cur;
	return NULL;
}

void player_queue_remove_by_fd( player_queue * queue, int key )
{
	player * cur = queue->tail;
	player * prev = NULL;
	while( cur->next != NULL && cur->sockfd != key )
		prev = cur;
		cur = cur->next;

	if( cur->sockfd == key )
	{
		if( prev != NULL ) prev->next = cur->next;
		free( cur );
	}
}

player * player_create( ts_queue * queue, int sockfd )
{
	player * ply = malloc( sizeof( player ) );
	memset( ply->name, 0, sizeof( char ) * 32 );
	ply->sockfd = sockfd;
	ply->next = NULL;

	player_thread_args * args;
	args->sockfd = ply->sockfd;
	args->queue = queue;
	pthread_create( &ply->thread, NULL, &player_thread_callback, (void*)&args );

	return ply;
}

void * player_thread_callback( void * args )
{
	generic_packet * pkt;
	int size;
	for( ;; )
	{
		pkt = malloc( sizeof( player_thread_args ) );
		int size = recv( ((player_thread_args*)args)->sockfd, pkt, sizeof( generic_packet ), 0 );

		switch( size )
		{
			case 0:
				// Disconnect
			case -1:
				// send/recv error
			default:
				ts_queue_push( ((player_thread_args*)args)->queue, pkt );
		}


	}
}

void send_to_player( player * ply, const generic_packet * pkt )
{
	send( ply->sockfd, pkt, sizeof( generic_packet ), 0 );
}

void send_to_all_players( player_queue * players, const generic_packet * packet )
{
	player * ply = players->tail;
	while( ply != NULL )
	{
		send_to_player( ply, packet );
		ply = ply->next;
	}
}
