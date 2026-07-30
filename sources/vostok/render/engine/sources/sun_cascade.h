#ifndef VOSTOK_RENDER_ENGINE_SUN_CASCADE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SUN_CASCADE_H_INCLUDED

#include <vostok/fixed_vector.h>
#include <vostok/math_float4x4.h>

#include "ray.h"

namespace vostok {
namespace render {

struct sun_cascade {
	enum { num_max_sun_shadow_cascades = 4 };

	sun_cascade( ) :
		reset_chain( false )
	{
	}

	~sun_cascade( ) { }

	float4x4				transform;
	fixed_vector< ray, 8 >	rays;
	float					size;
	float					bias;
	bool					reset_chain;
};

STATIC_SIZE_ASSERT( sun_cascade, 0x114 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SUN_CASCADE_H_INCLUDED
