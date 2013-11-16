#include "map.h"
#include <GLFW/glfw3.h>
#include "../general/debug.h"
#include <stdlib.h>

GLFWwindow * g_win;
GLuint g_shader_program;
GLuint g_pad0_buffer;
GLuint g_pad1_buffer;

GLuint create_vertex_shader( void )
{
	GLuint shader = glCreateShader( GL_VERTEX_SHADER );

	char * vertex_shader = 
	{
		"#version 330\n\
		 layout(location = 0) in vec4 position;\n\
		 void main()\n\
		 {\n\
			 gl_Position = position;\n\
		 }"
	};

	glShaderSource( shader, 1, &vertex_shader, NULL );

	glCompileShader( shader );
	GLint status;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
	if( status == GL_FALSE )
	{
		debug_printf( "Failed to compile\n" );
		GLint info_log_length;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &info_log_length );
		GLchar *str_info_log = malloc( info_log_length + 1 );
		glGetShaderInfoLog( shader, info_log_length, NULL, str_info_log );

		debug_printf( "%s\n", str_info_log );
		return 0;
	}
	
	return shader;
}

GLuint create_fragment_shader( void )
{
	GLuint shader = glCreateShader( GL_FRAGMENT_SHADER );
	char * fragment_shader =
	{
		"#version 330\n\
		 out vec4 outputColor;\n\
		 void main()\n\
		 {\n\
			 outputColor = vec4( 1.0f, 1.0f, 1.0f, 1.0f );\n\
		 }"
	};

	glShaderSource( shader, 1, &fragment_shader, NULL );
	glCompileShader( shader );
	GLint status;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
	if( status == GL_FALSE )
	{
		debug_printf( "Failed to compile\n" );
		GLint info_log_length;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &info_log_length );
		GLchar *str_info_log = malloc( info_log_length + 1 );
		glGetShaderInfoLog( shader, info_log_length, NULL, str_info_log );

		debug_printf( "%s\n", str_info_log );
		return 0;
	}

	return shader;
}

GLuint create_program( void  )
{
	GLuint vertex = create_vertex_shader();
	GLuint fragment = create_fragment_shader();
	GLuint program = glCreateProgram();
	glAttachShader( program, vertex );
	glAttachShader( program, fragment );

	glLinkProgram( program );

	GLint status;
	glGetProgramiv( program, GL_LINK_STATUS, &status );
	if( status == GL_FALSE )
	{
		debug_printf( "Failed to link shaders\n" );
		GLint info_log_length;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &info_log_length );

		GLchar * str_info_log = malloc( info_log_length + 1 );
		glGetProgramInfoLog( program, info_log_length, NULL, str_info_log );
		debug_printf( "%s\n", str_info_log );
	}

	glDetachShader( program, vertex );
	glDetachShader( program, fragment );
}

void window_size_callback( GLFWwindow * win, int x, int y )
{
	glfwSetWindowSize( win, x, y );
}

void window_framebuffer_callback( GLFWwindow * win, int x, int y )
{
	glViewport( 0, 0, x, y );
}

void window_key_callback( GLFWwindow * win, int key, int scancode, int action, int mods )
{
	if( action == GLFW_PRESS )
	{
		if( key == GLFW_KEY_UP )
		{
			g_key_pressing = 1;
		}
		if( key == GLFW_KEY_DOWN )
		{
			g_key_pressing = -1;
		}
	}
	if( action == GLFW_RELEASE )
	{
		if( key == GLFW_KEY_UP && g_key_pressing == 1 )
		{
			g_key_pressing = 0;
		}
		if( key == GLFW_KEY_DOWN && g_key_pressing == -1 )
		{
			g_key_pressing = 0;
		}
	}
}

void init_resources( void )
{
	glGenBuffers( 1, &g_pad0_buffer );
	glGenBuffers( 1, &g_pad1_buffer );
}

void move_pad0( float n )
{
	const float vertex_pos[] = {
		-0.9f, n + 0.3f, 0.f, 1.f,
		-0.85f, n + 0.3f, 0.f, 1.f,
		-0.9f, n - 0.3f, 0.f, 1.f,
		-0.9f, n - 0.3f, 0.f, 1.f,
		-0.85f, n - 0.3f, 0.f, 1.f,
		-0.85f, n + 0.3f, 0.f, 1.f };
	glBindBuffer( GL_ARRAY_BUFFER, g_pad0_buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertex_pos ), vertex_pos, GL_STATIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void move_pad1( float n )
{
	const float vertex_pos[] = {
		+0.9f, n + 0.3f, 0.f, 1.f,
		+0.85f, n + 0.3f, 0.f, 1.f,
		+0.9f, n - 0.3f, 0.f, 1.f,
		+0.9f, n - 0.3f, 0.f, 1.f,
		+0.85f, n - 0.3f, 0.f, 1.f,
		+0.85f, n + 0.3f, 0.f, 1.f };
	glBindBuffer( GL_ARRAY_BUFFER, g_pad1_buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertex_pos ), vertex_pos, GL_STATIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void draw_pad0( float n )
{
	move_pad0( n );
	glColor4f( 1.f, 0.f, 0.f, 1.f );
	glUseProgram( g_shader_program );
	glBindBuffer( GL_ARRAY_BUFFER, g_pad0_buffer );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
}

void draw_pad1( float n )
{
	move_pad1( n );
	glColor4f( 0.f, 0.f, 1.f, 1.f );
	glUseProgram( g_shader_program );
	glBindBuffer( GL_ARRAY_BUFFER, g_pad1_buffer );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
}

void map_init( void )
{
	g_key_pressing  = 0;
	debug_printf( "Initializing graphics\n" );
	debug_printf( "Intializing window\n" );
	glfwInit();
	g_win = glfwCreateWindow( 512, 512, "Pong", NULL, NULL ); 
	glfwMakeContextCurrent( g_win );
	glfwSetWindowSizeCallback( g_win, window_size_callback );
	glfwSetFramebufferSizeCallback( g_win, window_framebuffer_callback );
	glfwSetKeyCallback( g_win, window_key_callback );
	debug_printf( "Initializing shaders\n" );
	g_shader_program = create_program();
	debug_printf ("Initializing graphics resources\n" );
	init_resources();
	debug_printf( "Initialized graphics\n" );
	glfwPollEvents();
}

void map_draw( float pad0, float pad1 )
{
	glClearColor( .1f, .1f, .1f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT );
	draw_pad0( pad0 );
	draw_pad1( pad1 );
	glfwSwapBuffers( g_win );
	glfwPollEvents();
}
