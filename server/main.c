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
	float ball_x, ball_y;
	float bvx, bvy;
	float tick;

	srand( clock() );


	for( ;; )
	{
		usleep( 10000 );
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
					player * ply;
					ply = player_queue_search_by_fd( players, packet->cl_mv_update.sockfd );
				if( ( players->tail == ply || players->tail->next == ply ) && game_state == 1 )
				{
					int sockfd;
					float y;
					ply->y += packet->cl_mv_update.y * 0.01;
				}
			}
			free( packet );
		}

		if( players->tail != NULL && players->tail->next != NULL )
		{
			if( game_state == 0 )
			{
				if( players->tail->next->name[0] == '\0' || players->tail->name[0] == '\0' )
				{
					game_state = 0;
				}
				else
				{
					send_to_all_players( players, create_client_start_message( players->tail->name, players->tail->next->name ) );
					game_state = 1;
					players->tail->y = 0;
					players->tail->next->y = 0;
					ball_x = 0;
					ball_y = 0;
					bvx = -0.01;
					bvy = 0;
					tick = 0;
				}
			}
			if( !( (int)tick % 100 ) )
				send_to_all_players( players, create_client_start_countdown( 5 - (tick / 100) ) );
			if( tick > 400 ) 
			{
				if( abs( ball_y + bvy ) >= 1 )
					bvy *= -1;

				if( ball_x <= -0.85 )
				{
					if( ball_y > ( players->tail->y + 0.35 ) || ball_y < ( players->tail->y - 0.35 ) )
					{
						player_queue_first_to_last( players );
						game_state = 0;
					}
					else
					{
						bvx = 0.07;
						bvy = ball_y - players->tail->y;
						float h = pow( ( pow( bvx, 2 ) + pow( bvy, 2 ) ), 0.5 ) * 100;
						bvx /= h;
						bvy /= h;
					}
				}
				if( ball_x >= 0.85 )
				{
					if( ball_y > ( players->tail->next->y + 0.35 ) || ball_y < ( players->tail->next->y - 0.35 ) )
					{
						player_queue_second_to_last( players );
						game_state = 0;
					}
					else
					{
						bvx = -0.07;
						bvy = ball_y - players->tail->next->y;
						float h = pow( ( pow( bvx, 2 ) + pow( bvy, 2 ) ), 0.5 ) * 100;
						bvx /= h;
						bvy /= h;
					}
				}

				ball_x += bvx;
				ball_y += bvy;
			}
			else
			{
				tick++;
			}
			send_to_all_players( players, create_client_game_state( players->tail->y, players->tail->next->y, ball_x, ball_y ) );

		}
		else
		{
			game_state = 0;
		}
	}

	return 0;
}
