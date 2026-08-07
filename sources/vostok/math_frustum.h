////////////////////////////////////////////////////////////////////////////
//	Created 	: 23.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MATH_FRUSTUM_H_INCLUDED
#define VOSTOK_MATH_FRUSTUM_H_INCLUDED

#include <vostok/math_aabb_plane.h>
#include <vostok/math_cuboid.h>

namespace vostok {
namespace math {

class VOSTOK_CORE_API frustum : public cuboid {
public:
	inline				frustum		( ) { }
	inline explicit		frustum		( plane const (&planes)[plane_count] ) : cuboid( planes ) { }
						frustum		( float4x4 const& view_multiplied_by_projection );
}; // class frustum

VOSTOK_CORE_API bool get_frustum_vertices( frustum const& f, float3 (&vertices)[8] );

} // namespace math
} // namespace vostok

#endif // #ifndef VOSTOK_MATH_FRUSTUM_H_INCLUDED
