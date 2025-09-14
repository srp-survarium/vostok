////////////////////////////////////////////////////////////////////////////
//	Created 	: 23.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_MATH_CYLINDER_H_INCLUDED
#define VOSTOK_MATH_CYLINDER_H_INCLUDED

namespace vostok {
namespace math {

class VOSTOK_CORE_API cylinder {
public:
	float3				center;
	float				radius;
	float3				direction;
	float				height;

public:
	inline				cylinder	( );

	#ifndef MASTER_GOLD
	inline	bool		valid		( ) const;
	#endif

public:
	intersection		intersect	( float3 const& position, float3 const& direction, float& distance ) const;

private:
	u32					intersect	( float3 const& position, float3 const& direction, float ( &t )[ 2 ] ) const;
}; // class cylinder

} // namespace math
} // namespace vostok

#include <vostok/math_cylinder_inline.h>

#endif // #ifndef VOSTOK_MATH_CYLINDER_H_INCLUDED