////////////////////////////////////////////////////////////////////////////
//	Created		: 03.08.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "curve_line.h"

namespace vostok {
namespace particle {

float get_tangent_from_2d_vector(vostok::math::float2 const& vec)
{
	float t = vec.y / vostok::math::max(vec.x, vostok::math::epsilon_5);
	return t;
}

} // namespace particle
} // namespace vostok
