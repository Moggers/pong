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

	for( ;; )
	{
		generic_packet * pkt;
		while ( ( pkt = ts_queue_try_pop( queue ) ) != NULL )
		{
			debug_printf( "Received \'%s\' from the server\n", packet_type_name( pkt->packet_type ) );
			if( pkt->packet_type == PACKET_CLIENT_INFO_REQUEST )
			{
				ply_id = pkt->cl_in_request.ply_id;
				debug_printf( "We are client %d\n", ply_id);
				send_to_server( create_client_info( ply_id, argv[3] ) );
				debug_printf( "Told the server our name is %s\n", argv[3] );
			}
			free( pkt );
		}
	}
	return 0;
}
