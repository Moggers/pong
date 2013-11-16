#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../general/ts_queue.h"
#include "../general/debug.h"
#include "../general/net_structs.h"
#include "accept_thread.h"
#include "player.h"
#include <math.h>

int main( int argc, char * argv[] )
{
	if( argc != 2 )
	{
		fprintf( stdout, "Usage: server port\n" );
		return 0;
	}
	debug_printf( "Start server\n" );

	ts_queue * queue = ts_queue_create();

	pthread_t * accept_thread = create_accept_thread( queue, argv[1] );

	player_queue * players = player_queue_create();

	int game_state = -1;
	srand( clock() );

	float paddle_0, paddle_1;

	for( ;; )
	{
		generic_packet * packet;
		while( ( packet = ts_queue_try_pop( queue ) ) != NULL )
		{
			if( packet->packet_type == PACKET_CLIENT_REQUEST )
			{
				player * ply = player_create( queue, packet->cl_request.sockfd );
				player_queue_push( players, ply );
				generic_packet * pkt = create_client_info_request( ply->sockfd );
				send_to_player( ply, pkt );
				debug_printf( "Sent info request packet to client\n" );
			}

			if( packet->packet_type == PACKET_CLIENT_INFO )
			{
				player * ply = player_queue_search_by_fd( players, packet->cl_info.sockfd );
				strcpy( ply->name, packet->cl_info.name );
				debug_printf( "%d told us that their name is %s\n", ply->sockfd, ply->name );
			}
			if( packet->packet_type == PACKET_CLIENT_DISCONNECT )
			{
				player_queue_remove_by_fd( players, packet->cl_disconnect.sockfd );
			}
			if( packet->packet_type == PACKET_CLIENT_MOVE_UPDATE )
			{
				int sockfd;
				float y;
				player * ply;
				ply = player_queue_search_by_fd( players, packet->cl_mv_update.sockfd );
				ply->y += packet->cl_mv_update.y * 0.001;				
				debug_printf( "Moved %d to %f, moving by %f unit\n", ply->sockfd, ply->y, packet->cl_mv_update.y );
				if( players->tail == ply )
				{
					debug_printf( "That was player one, so we're sending out the position updates\n" );
					send_to_all_players( players, create_client_game_state( players->tail->y, players->tail->next->y, 0, 0 ) );
				}
				if( players->tail->next == ply )
				{
					debug_printf( "That was player two, so we're sending out the position updates\n" );
					send_to_all_players( players, create_client_game_state( players->tail->y, players->tail->next->y, 0, 0 ) );
				}
			}
			free( packet );
		}

		if( players->tail != NULL && players->tail->next != NULL )
		{
		}
	}

	return 0;
}
