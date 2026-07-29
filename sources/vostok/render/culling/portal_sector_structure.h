////////////////////////////////////////////////////////////////////////////
//	Created		: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_CULLING_PORTAL_SECTOR_STRUCTURE_H_INCLUDED
#define VOSTOK_RENDER_CULLING_PORTAL_SECTOR_STRUCTURE_H_INCLUDED

#include <vostok/buffer_vector.h>
#include <vostok/math_plane.h>
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {

namespace collision {
class geometry;
struct space_partitioning_tree;
} // namespace collision

namespace memory {
class base_allocator;
} // namespace memory

namespace render {
namespace culling {

class spatial_sector;

class portal
{
public:
	inline	u32 const		( &get_sectors	( ) const )[2]	{ return m_sectors; }
	inline	float3 const	( &get_points	( ) const )[4]	{ return m_points; }
	inline	math::plane const&	get_plane	( ) const		{ return m_plane; }

private:
	math::plane			m_plane;
	u32					m_sectors[2];
	float3				m_points[4];
	bool				m_visible;
}; // class portal

STATIC_SIZE_ASSERT( portal, 0x4C );

class portal_sector_structure : public resources::unmanaged_resource
{
public:
								portal_sector_structure	(
									memory::base_allocator*	allocator,
									u32						sectors_count,
									u32						portals_count
								);
	virtual						~portal_sector_structure	( );

	inline	buffer_vector< portal > const&			get_portals	( ) const { return m_portals; }
	inline	buffer_vector< spatial_sector > const&	get_sectors	( ) const { return m_sectors; }

private:
	memory::base_allocator*					m_allocator;
	pvoid									m_portals_buffer;
	buffer_vector< portal >					m_portals;
	u32*									m_portal_ids_buffer;
	pvoid									m_sectors_buffer;
	buffer_vector< spatial_sector >			m_sectors;
	collision::space_partitioning_tree*		m_sectors_spatial_tree;
	collision::space_partitioning_tree*		m_portals_spatial_tree;
	collision::geometry*					m_portals_geometry;
}; // class portal_sector_structure

STATIC_SIZE_ASSERT( portal_sector_structure, 0x138 );

typedef resources::resource_ptr<
	portal_sector_structure,
	resources::unmanaged_intrusive_base
> portal_sector_structure_ptr;

} // namespace culling
} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_CULLING_PORTAL_SECTOR_STRUCTURE_H_INCLUDED
