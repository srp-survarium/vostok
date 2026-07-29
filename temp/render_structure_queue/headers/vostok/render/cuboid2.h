////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CUBOID2_H_INCLUDED
#define RENDER_CUBOID2_H_INCLUDED

/* INCLUDES */
class vostok::math::aabb_plane;
class vostok::math::intersection;

/* FORWARD REFS */
class vostok::math::aabb;
class vostok::math::plane;

namespace vostok {
namespace render {

class cuboid2 {
public:
	inline							cuboid2		( ) { /* no source */ }
	inline							cuboid2		( math::plane* arg_0, const u32 arg_1 ) { /* no source */ }

	inline	math::intersection		test_inexact( math::aabb const& arg_0 ) const { /* no source */ }

protected:
	/* 0x0000 */	math::aabb_plane	m_planes[16];
	/* 0x0140 */	u32					m_num_planes;
}; // class cuboid2

STATIC_SIZE_ASSERT(cuboid2, 0x144);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CUBOID2_H_INCLUDED
