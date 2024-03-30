#pragma once
#include "Bases.h"


namespace route
{
	class Resource
	{
	public:
		inline refc_t change_counter() const {
			return m_change_counter;
		}

	protected:
		inline void inc_change_counter() {
			++m_change_counter;
		};
	private:
		refc_t m_change_counter;
	};
}
