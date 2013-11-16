#include "connect.h"
#include "../general/ts_queue.h"
#include <stdio.h>
#include <time.h>
#include "../general/debug.h"
#include "map.h"

int main( int argc, char * argv[] )
{
	if( argc != 4 )
	{
		fprintf( stdout, "Usage: client address port player_name\n" );
		return 0;
	}

	map_init();

	ts_queue * queue = ts_queue_create();
	connect_to_server( argv[1], argv[2], queue );
	int ply_id;
	float paddle_left = 0, paddle_right = 0;
	float ball_x, ball_y;

	for( ;; )
	{
		generic_packet * pkt;
		while ( ( pkt = ts_queue_try_pop( queue ) ) != NULL )
		{
			if( pkt->packet_type == PACKET_CLIENT_INFO_REQUEST )
			{
				ply_id = pkt->cl_in_request.ply_id;
				debug_printf( "We are client %d\n", ply_id);
				send_to_server( create_client_info( ply_id, argv[3] ) );
				debug_printf( "Told the server our name is %s\n", argv[3] );
			}
			if( pkt->packet_type == PACKET_CLIENT_GAME_STATE )
			{
				paddle_left = pkt->cl_ga_state.ply0_pad;
				paddle_right = pkt->cl_ga_state.ply1_pad;
				ball_x = pkt->cl_ga_state.bx;
				ball_y = pkt->cl_ga_state.by;
			}
				
			free( pkt );
		}
		if( g_key_pressing != 0 )
		{
			send_to_server( create_client_move_update( ply_id, g_key_pressing ) );
			debug_printf( "moved\n" );
		}
		map_draw( paddle_left, paddle_right );
	}
	return 0;
}
