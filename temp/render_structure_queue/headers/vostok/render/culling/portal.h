////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CULLING_PORTAL_H_INCLUDED
#define RENDER_CULLING_PORTAL_H_INCLUDED

/* INCLUDES */
class vostok::math::plane;

/* FORWARD REFS */
class const vostok::math::float3[4];

namespace vostok {
namespace render {
namespace culling {

class portal {
public:
	inline							portal		(
										u32					arg_0,
										u32					arg_1,
										float3 const&		arg_2,
										float3 const&		arg_3,
										float3 const&		arg_4,
										float3 const&		arg_5
									) { /* no source */ }

	inline	const u32[2]&			get_sectors	( ) const { /* no source */ }
	inline	const float3[4]&		get_points	( ) const { /* no source */ }
	inline	math::plane const&		get_plane	( ) const { /* no source */ }

	inline	void					swap_sectors( ) { /* no source */ }

	inline	void					set_visible	( bool arg_0 ) { /* no source */ }
	inline	bool					is_visible	( ) const { /* no source */ }

private:
	/* 0x0000 */	math::plane		m_plane;
	/* 0x0010 */	u32				m_sectors[2];
	/* 0x0018 */	float3			m_points[4];
	/* 0x0048 */	bool			m_visible;
}; // class portal

STATIC_SIZE_ASSERT(portal, 0x4C);

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CULLING_PORTAL_H_INCLUDED
