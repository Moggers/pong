#include "map.h"
#include <GLFW/glfw3.h>

void map_init( void )
{
	glfwInit();
	GLFWwindow * win = glfwCreateWindow( 512, 512, "Pong", NULL, NULL ); 
}
