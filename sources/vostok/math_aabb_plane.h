////////////////////////////////////////////////////////////////////////////
//	Created		: 18.02.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MATH_AABB_PLANE_H_INCLUDED
#define VOSTOK_MATH_AABB_PLANE_H_INCLUDED

namespace vostok {
namespace math {

class VOSTOK_CORE_API aabb_plane {
public:
	void				transform	( float4x4 const& transform );
	intersection		test		( aabb const& aabb ) const;
	void				normalize	( );

private:
	void				setup_lut_id( );

public:
	plane				plane;

private:
	u32					m_lut_id;
}; // struct aabb_plane

} // namespace math
} // namespace vostok

#endif // #ifndef VOSTOK_MATH_AABB_PLANE_H_INCLUDED