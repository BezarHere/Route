#include "pch.h"
#include "Object.h"

namespace route
{
	
	void Object::update_2d( real_t deltatime ) {
		std::cout << deltatime << " updated 2d!\n";
	}

	void Object::update_3d( real_t deltatime ) {
		std::cout << deltatime << "updated 3d!\n";
	}

}
