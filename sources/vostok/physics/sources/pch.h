////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.11.2008
//	Author		: Dmitriy Iassenev
//	Description : precompiled header
////////////////////////////////////////////////////////////////////////////

#ifndef PCH_H_INCLUDED
#define PCH_H_INCLUDED

#define VOSTOK_PHYSICS_BUILDING

#ifndef VOSTOK_STATIC_LIBRARIES
#	define VOSTOK_ENGINE_BUILDING
#endif // #ifndef VOSTOK_STATIC_LIBRARIES

#define VOSTOK_LOG_MODULE_INITIATOR	"physics"
#include <vostok/extensions.h>

#include "physics_memory.h"

using namespace vostok;

namespace vostok {
namespace physics {

	enum axis
	{
		x_axis = 0,
		y_axis,
		z_axis,
		none_axis	
	};

}
}

using  vostok::physics::axis;
using  vostok::physics::x_axis;
using  vostok::physics::y_axis;
using  vostok::physics::z_axis;
using  vostok::physics::none_axis;
#endif // #ifndef PCH_H_INCLUDED