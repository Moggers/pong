#ifndef DEFINED_DEBUG
#ifdef DEBUG
#include <stdio.h>
#define debug_printf( ... ) fprintf( stdout, __VA_ARGS__ )
#else
#define debug_printf( ... )
#endif // DEBUG
#endif // DEFINED_DEBUG
