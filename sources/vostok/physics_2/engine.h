////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_2_ENGINE_H_INCLUDED
#define VOSTOK_PHYSICS_2_ENGINE_H_INCLUDED

namespace vostok {
namespace physics_2 {

/* vostok::physics::engine */

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

struct engine  {
public:

private:
}; // struct engine

namespace {
	typedef char size_assert[
		sizeof(engine) == 0x1 ? 1 : -1
	];
}

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_2_ENGINE_H_INCLUDED
