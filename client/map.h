typedef struct map
{
	float left_paddle;
	float right_paddle;
	float ball_x;
	float ball_y;
} map;

void map_init( void );

void map_draw( struct map * );

