////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CONVEX_VOLUME_H_INCLUDED
#define RENDER_CONVEX_VOLUME_H_INCLUDED

/* INCLUDES */
class vostok::math::plane;
class vostok::math::intersection;

/* FORWARD REFS */
class vostok::math::sphere;

namespace vostok {
namespace render {

class convex_volume {
public:
	inline	void					clear				( ) { /* no source */ }

	inline	void					add					( math::plane& arg_0 ) { /* no source */ }
	inline	void					add					( float3 const& arg_0, float3 const& arg_1, float3 const& arg_2 ) { /* no source */ }

	inline	void					create_from_matrix	( float4x4 const& arg_0 ) { /* no source */ }
	inline	void					create_from_planes	( math::plane const* arg_0, u32 arg_1 ) { /* no source */ }

	inline	math::intersection		test_sphere			( float3 const& arg_0, float arg_1 ) const { /* no source */ }

	inline	math::intersection		test				( math::sphere const& arg_0 ) const { /* no source */ }

	inline							convex_volume		( ) { /* no source */ }

	/* 0x0000 */	math::plane		planes[12];
	/* 0x00c0 */	u32				count;
}; // class convex_volume

STATIC_SIZE_ASSERT(convex_volume, 0xC4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CONVEX_VOLUME_H_INCLUDED
