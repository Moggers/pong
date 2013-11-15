#ifndef DEFINED_PLAYER
#define DEFINED_PLAYER

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "../general/ts_queue.h"
#include "../general/net_structs.h"

typedef struct generic_packet generic_packet; // Forward declare solves circular dependancy

typedef struct player
	{
		pthread_t thread;
		char name[32];
		int sockfd;
		struct player * next;
		float y;
	} player;

typedef struct player_queue
{
	player * head; // This goes up as we add players
	player * tail; // This goes up as we pop players
} player_queue;

typedef struct player_thread_args
{
	int sockfd;
	ts_queue * queue;
} player_thread_args;

player_queue * player_queue_create( void );

void player_queue_push( player_queue *, player * );

player * player_queue_pop( player_queue * );

player * player_queue_search_by_fd( player_queue *, int );

void player_queue_remove_by_fd( player_queue *, int );

player * player_create( ts_queue *, int );

void * player_thread_callback( void * );

void send_to_player( player * ply, const generic_packet * );

void send_to_all_players( player_queue *, const generic_packet * );

#endif // DEFINED_PLAYER
