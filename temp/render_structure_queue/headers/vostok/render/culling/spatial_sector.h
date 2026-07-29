////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CULLING_SPATIAL_SECTOR_H_INCLUDED
#define RENDER_CULLING_SPATIAL_SECTOR_H_INCLUDED

/* INCLUDES */
class vostok::math::aabb;

namespace vostok {
namespace render {
namespace culling {

class spatial_sector {
public:
	inline							spatial_sector		( u32* arg_0, u32 arg_1, math::aabb const& arg_2 ) { /* no source */ }

	inline	u32 const*				get_portals			( ) const { /* no source */ }
	inline	u32						get_portals_count	( ) const { /* no source */ }
	inline	math::aabb const&		get_aabb			( ) const { /* no source */ }

	inline	void					sort_portal_ids		( float const* arg_0 ) { /* no source */ }

private:
	/* 0x0000 */	math::aabb		m_aabb;
	/* 0x0018 */	u32*			m_portal_ids;
	/* 0x001c */	u32				m_portals_count;
}; // class spatial_sector

STATIC_SIZE_ASSERT(spatial_sector, 0x20);

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CULLING_SPATIAL_SECTOR_H_INCLUDED
