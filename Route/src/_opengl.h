#pragma once



class OpenGL
{
public:
	static constexpr struct {
		unsigned major, minor;
	} ContextVersion = { 4, 4 };

	static errno_t init();
	static void close();

private:


private:

};
