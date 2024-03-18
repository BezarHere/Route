#include <iostream>
#include <Route.h>

#pragma comment(lib, "Route.lib")

int main() {
	route::Window window{ "hello", route::Rect2i( 100, 100, 200, 200 ) };
	while (window)
	{
		window.poll();
	}
}
