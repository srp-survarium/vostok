////////////////////////////////////////////////////////////////////////////
//	Created		: 26.09.2011
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_BULLET_UTILS_H_INCLUDED
#define VOSTOK_PHYSICS_BULLET_UTILS_H_INCLUDED

#include <LinearMath/btTransformUtil.h>

namespace vostok {
namespace physics {

inline btVector3	from_vostok(float3 const& v){ return btVector3(v.x, v.y, -v.z); }
inline float3		from_bullet(btVector3 const& v){ return float3(v.x(), v.y(), -v.z()); }

btTransform			from_vostok(float4x4 const& m);
float4x4			from_bullet(btTransform const& m);

inline math::quaternion from_bullet( btQuaternion const& from )
{ 
	return math::quaternion( from_bullet(from.getAxis()), from.getAngle());
}
inline btQuaternion from_vostok( math::quaternion const& from )
{ 
	float3 axe;
	float angle;
	from.get_axis_and_angle(axe, angle);
	return btQuaternion( from_vostok(axe), angle);
}

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_BULLET_UTILS_H_INCLUDED