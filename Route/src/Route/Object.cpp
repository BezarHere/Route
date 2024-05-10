#include "pch.h"
#include "Object.h"
#include "Component.h"
#include <iostream>

namespace route
{


	Object::Object( const Object &obj )
		: type{ obj.type }, m_active{ obj.m_active } {
	}

	Object &Object::operator=( const Object &obj ) {
		// TODO: insert return statement here
		return *this;
	}

	void Object::update_2d( real_t deltatime ) {
		std::cout << deltatime << " updated 2d!\n";
	}

	void Object::update_3d( real_t deltatime ) {
		std::cout << deltatime << "updated 3d!\n";
	}

	void Object::update( real_t deltatime ) {
		for (const auto &comp : m_components)
		{
			comp->update( deltatime );
		}

		if (type == ObjectType::Object2D)
		{
			return update_2d( deltatime );
		}
		return update_3d( deltatime );
	}

}
