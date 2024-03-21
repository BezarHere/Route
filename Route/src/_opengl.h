#pragma once
#include <GL/glew.h>

class OpenGL
{
public:
	static constexpr struct {
		unsigned major, minor;
	} ContextVersion = { 4, 4 };

	static errno_t init();
	static void close();

	static inline GLint query_int( const GLenum id ) {
		GLint value = 0;
		glGetIntegerv( id, &value );
		return value;
	}

	static inline GLfloat query_float( const GLenum id ) {
		GLfloat value = 0;
		glGetFloatv( id, &value );
		return value;
	}

private:


private:

};
