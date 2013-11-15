#ifndef DEFINED_CONNECT
#define DEFINED_CONNECT

#include "../general/ts_queue.h"
#include "../general/net_structs.h"

int g_server_sockfd;
int connect_to_server( char * , char *, ts_queue * );
void send_to_server( generic_packet* );

#endif // DEFINED_CONNECT
