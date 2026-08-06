#ifndef VOSTOK_RENDER_CULLING_PORTAL_SECTOR_STRUCTURE_H_INCLUDED
#define VOSTOK_RENDER_CULLING_PORTAL_SECTOR_STRUCTURE_H_INCLUDED

#include <vostok/buffer_vector.h>
#include <vostok/math_aabb.h>
#include <vostok/math_plane.h>
#include <vostok/resources_resource_ptr.h>
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {

namespace collision {

class geometry;
struct space_partitioning_tree;

} // namespace collision

namespace configs {

class binary_config_value;

} // namespace configs

namespace memory {

class base_allocator;

} // namespace memory

namespace math {

class frustum;

} // namespace math

namespace render {
namespace culling {

enum {
	sectors_volume_object_type	= 0x1,
	portal_object_type			= 0x2,
};

class portal {
public:
	portal(
		u32 sector0,
		u32 sector1,
		float3 const& point0,
		float3 const& point1,
		float3 const& point2,
		float3 const& point3
	) :
		m_plane( math::create_plane( point0, point1, point2 ) ),
		m_visible( true )
	{
		m_sectors[0] = sector0;
		m_sectors[1] = sector1;
		m_points[0] = point0;
		m_points[1] = point1;
		m_points[2] = point2;
		m_points[3] = point3;
	}

	u32 const ( &get_sectors( ) const )[2]
	{
		return m_sectors;
	}

	float3 const ( &get_points( ) const )[4]
	{
		return m_points;
	}

	math::plane const& get_plane( ) const
	{
		return m_plane;
	}

	void swap_sectors( )
	{
		u32 const sector = m_sectors[0];
		m_sectors[0] = m_sectors[1];
		m_sectors[1] = sector;
	}

	void set_visible( bool visible )
	{
		m_visible = visible;
	}

	bool is_visible( ) const
	{
		return m_visible;
	}

private:
	math::plane m_plane;
	u32 m_sectors[2];
	float3 m_points[4];
	bool m_visible;
};

STATIC_SIZE_ASSERT( portal, 0x4C );

class spatial_sector {
public:
	spatial_sector( u32* portal_ids, u32 portals_count, math::aabb const& bounds ) :
		m_aabb( bounds ),
		m_portal_ids( portal_ids ),
		m_portals_count( portals_count )
	{
	}

	u32 const* get_portals( ) const
	{
		return m_portal_ids;
	}

	u32 get_portals_count( ) const
	{
		return m_portals_count;
	}

	math::aabb const& get_aabb( ) const
	{
		return m_aabb;
	}

	void sort_portal_ids( float const* )
	{
		// claude@NOTE: no legacy ancestor - spatial_sector::sort_portal_ids postdates the legacy corpus (legacy culling header had no spatial_sector); matcher-phase work.
		// STATE[STUB]
	}

private:
	math::aabb m_aabb;
	u32* m_portal_ids;
	u32 m_portals_count;
};

STATIC_SIZE_ASSERT( spatial_sector, 0x20 );

class portal_sector_structure : public resources::unmanaged_resource {
public:
	typedef buffer_vector<portal> portals_type;
	typedef buffer_vector<spatial_sector> sectors_type;
	typedef collision::space_partitioning_tree* tree_ptr;

	portal_sector_structure(
		memory::base_allocator* allocator,
		u32 sectors_count,
		u32 portals_count
	);
	virtual ~portal_sector_structure( );

	u32 get_sector_id( memory::base_allocator& allocator, float3 const& pos ) const;

	portals_type const& get_portals( ) const
	{
		return m_portals;
	}

	sectors_type const& get_sectors( ) const
	{
		return m_sectors;
	}

	void load( configs::binary_config_value* value_ptr );
	void sort_portal_ids( float const* distances );

	void set_portal_visible( u32 portal_id, bool visible )
	{
		m_portals[portal_id].set_visible( visible );
	}

	void update_portals_visability( math::frustum const& f, pcbyte occlusion_results );

private:
	// claude@NOTE: the legacy ancestor (dx9/model_manager.cpp portal::create)
	// built each portal plane from its first three points
	// (create_plane(p[0],p[1],p[2]) - the averaged-normal variant was
	// commented out) and derived a bounding sphere from the point aabb; the
	// canonical portal ctor leaves m_plane default, so that plane build (and
	// any view-consistent reorientation via swap_sectors) belongs here or in
	// load - matcher-phase against the load body 0x75ebf0.
	void adjust_portals_orientation( )
	{
		// STATE[STUB]
	}

	void initialize_portals_geometry( );

private:
	memory::base_allocator* m_allocator;
	void* m_portals_buffer;
	portals_type m_portals;
	u32* m_portal_ids_buffer;
	void* m_sectors_buffer;
	sectors_type m_sectors;
	tree_ptr m_sectors_spatial_tree;
	tree_ptr m_portals_spatial_tree;
	collision::geometry* m_portals_geometry;
};

STATIC_SIZE_ASSERT( portal_sector_structure, 0x138 );

typedef resources::resource_ptr<
	portal_sector_structure,
	resources::unmanaged_intrusive_base
> portal_sector_structure_ptr;

} // namespace culling
} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_CULLING_PORTAL_SECTOR_STRUCTURE_H_INCLUDED
