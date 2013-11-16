#include "net_structs.h"

generic_packet * create_client_request( int sockfd )
{
	generic_packet * packet = malloc( sizeof( generic_packet ) );
	memset( packet, 0, sizeof( generic_packet ) );
	packet->packet_type = PACKET_CLIENT_REQUEST;
	packet->cl_request.sockfd = sockfd;
	
	return packet;
}

void read_client_request( struct generic_packet * packet, int * sockfd )
{
	*sockfd = packet->cl_request.sockfd;
}

generic_packet * create_client_info( int sockfd, char * name )
{
	generic_packet * packet = malloc( sizeof( generic_packet ) );
	memset( packet, 0, sizeof( generic_packet ) );
	packet->packet_type = PACKET_CLIENT_INFO;
	strcpy( packet->cl_info.name, name );
	packet->cl_info.sockfd = sockfd;

	return packet;
}

void read_client_info( struct generic_packet * packet, int * sockfd, char * name )
{
	strcpy( name, packet->cl_info.name );
	*sockfd = packet->cl_info.sockfd;
}

generic_packet * create_client_info_request( int sockfd )
{
	generic_packet * packet = malloc( sizeof( generic_packet ) );
	memset( packet, 0, sizeof( generic_packet ) );
	packet->packet_type = PACKET_CLIENT_INFO_REQUEST;
	packet->cl_in_request.ply_id = sockfd;

	return packet;
}

generic_packet * create_client_disconnect( int sockfd )
{
	generic_packet * packet = malloc( sizeof( generic_packet ) );
	memset( packet, 0, sizeof( generic_packet ) );
	packet->packet_type = PACKET_CLIENT_DISCONNECT;
	packet->cl_disconnect.sockfd = sockfd;

	return packet;
}

generic_packet * create_client_start_message( char * name0, char * name1 )
{
	generic_packet * packet = malloc( sizeof( generic_packet ) );
	memset( packet, 0, sizeof( generic_packet ) );
	packet->packet_type = PACKET_CLIENT_START_MESSAGE;
	strcpy( packet->cl_message.ply_one, name0 );
	strcpy( packet->cl_message.ply_two, name1 );

	return packet;
}

generic_packet * create_client_game_state( float ply0_pad, float ply1_pad, float bx, float by )
{
	generic_packet * packet = malloc( sizeof( generic_packet ) );
	memset( packet, 0, sizeof( generic_packet ) );
	packet->packet_type = PACKET_CLIENT_GAME_STATE;
	packet->cl_ga_state.ply0_pad = ply0_pad;
	packet->cl_ga_state.ply1_pad = ply1_pad;
	packet->cl_ga_state.bx = bx;
	packet->cl_ga_state.by = by;

	return packet;
}

generic_packet * create_client_move_update( int sockfd, float y )
{
	generic_packet * packet = malloc( sizeof( generic_packet ) );
	packet->packet_type = PACKET_CLIENT_MOVE_UPDATE;
	packet->cl_mv_update.sockfd = sockfd;
	packet->cl_mv_update.y = y;

	return packet;
}

generic_packet * create_client_start_countdown( int n )
{
	generic_packet * packet = malloc( sizeof( generic_packet ) );
	packet->packet_type = PACKET_CLIENT_START_COUNTDOWN;
	packet->cl_countdown.timer = n;

	return packet;
}

char * packet_type_name( int type )
{
	switch( type )
	{
		case PACKET_CLIENT_REQUEST:
			return "packet client request";
		case PACKET_CLIENT_INFO:
			return "packet client info";
		case PACKET_CLIENT_INFO_REQUEST:
			return "packet client info request";
		case PACKET_CLIENT_DISCONNECT:
			return "packet client disconnect";
		case PACKET_CLIENT_START_MESSAGE:
			return "packet client start message";
		case PACKET_CLIENT_GAME_STATE:
			return "packet client game state";
		case PACKET_CLIENT_MOVE_UPDATE:
			return "packet client move update";
		case PACKET_CLIENT_START_COUNTDOWN:
			return "packet client start countdown";
		default:
			return "unrecognized, probably unallocated memory\n";
	}
}
