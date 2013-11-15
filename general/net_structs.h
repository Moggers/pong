#ifndef DECLARED_NET_STRUCTS
#define DECLARED_NET_STRUCTS

#include <string.h>
#include <stdlib.h>

typedef struct player player; // Fucking circular dependancies god damn it

#define PACKET_CLIENT_REQUEST 1
#define PACKET_CLIENT_INFO 			2
#define PACKET_CLIENT_INFO_REQUEST 	3
#define PACKET_CLIENT_DISCONNECT 	4
#define PACKET_CLIENT_START_MESSAGE 5
#define PACKET_CLIENT_GAME_STATE 	6
#define PACKET_CLIENT_MOVE_UPDATE 	7

typedef struct client_request
{
	int sockfd;
} client_request;

typedef struct client_info
{
	char name[32];
	int sockfd;
} client_info;

typedef struct client_info_request
{
	int ply_id;
} client_info_request;

typedef struct client_disconnect
{
	int sockfd;
} disconnect_packet;

typedef struct client_start_message
{
	char ply_one[32];
	char ply_two[32];
} client_start_message;

typedef struct client_game_state
{
	float ply0_pad;
	float ply1_pad;
	float bx;
	float by;
} client_game_state;

typedef struct client_move_update
{
	int sockfd;
	float y;
} move_update;

typedef struct generic_packet
{
	int packet_type;

	union
	{
		struct client_request cl_request;
		struct client_info cl_info;
		struct client_disconnect cl_disconnect;
		struct client_start_message cl_message;
		struct client_game_state cl_ga_state;
		struct client_info_request cl_in_request;
		struct client_move_update cl_mv_update;
	};
} generic_packet;

generic_packet * create_client_request( int );

generic_packet * create_client_info( int, char * );

generic_packet * create_client_info_request( int );

generic_packet * create_client_disconnect( int );

generic_packet * create_client_start_message( char *, char * );

generic_packet * create_client_game_state( float, float, float, float );

generic_packet * create_client_move_update( int, float );

char * packet_type_name( int );

#endif // DECLARED_NET_STRUCTS
