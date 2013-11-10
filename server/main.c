#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "ts_queue.h"
#include "debug.h"
#include "net_structs.h"
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
	float left_paddle, right_paddle;
	float ballx, bally;
	float bvx, bvy;
	srand( clock() );
	int win;
	int match;

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
			}

			if( packet->packet_type == PACKET_CLIENT_INFO )
			{
				player * ply = player_queue_search_by_fd( players, packet->cl_info.sockfd );
				strcpy( ply->name, packet->cl_info.name );
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
				ply->y = packet->cl_mv_update.y;				
			}
		}

		if( players->tail != NULL && players->tail->next != NULL )
		{
			if( game_state == -1 )
			{
				win = 0;
				match = 0;
				game_state = 0;
				generic_packet * packet = create_client_start_message( players->tail, players->tail->next );
				send_to_all_players( players, packet );
				free( packet );
			}

			if( game_state == 0 )
			{
				if( match == 2 )
				{
					// Win condition
				}
				game_state = 1;
				left_paddle = 0.f;
				right_paddle = 0.f;
				ballx = 0.f;
				bally = 0.f;
				bvx = (float)( rand() * 10000 % 20000 ) / 10000.f;
				bvy = (float)( rand() * 10000 % 20000 ) / 10000.f;
				float h = pow( pow( bvx, 2 ) + pow( bvy, 2 ), 0.5 );
				bvx /= h;
				bvy /= h;
				usleep( 100000 );
			}

			ballx += bvx;
			bally += bvy;

			if( bally >= 50 || bally <= -50 )
				bvy = -bvy;

			if( ballx > 50 )
			{
				if( right_paddle < ( bally + 5 ) && right_paddle > ( bally - 5 ) )
					bvx = -bvx;
				else
				{
					win--;
					match++;
					game_state = 0;
				}
			}


			if( ballx < -50 )
			{
				if( left_paddle < ( bally + 5 ) && left_paddle > ( bally - 5 ) )
					bvx = -bvx;
				else
				{
					win++;
					match++;
					game_state = 0;
				}
			}

			packet = create_client_game_state( left_paddle, right_paddle, ballx, bally );
			send_to_all_players( players, packet );
			free( packet );
		}
	}

	return 0;
}
