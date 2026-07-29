////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CULLING_PORTAL_SECTOR_STRUCTURE_H_INCLUDED
#define RENDER_CULLING_PORTAL_SECTOR_STRUCTURE_H_INCLUDED

/* INCLUDES */
class vostok::buffer_vector<vostok::render::culling::portal>;
class vostok::buffer_vector<vostok::render::culling::spatial_sector>;
class vostok::collision::geometry;
class vostok::memory::base_allocator;
class vostok::resources::unmanaged_resource;
struct vostok::collision::space_partitioning_tree;

/* FORWARD REFS */
class vostok::configs::binary_config_value;
class vostok::math::frustum;

namespace vostok {
namespace render {
namespace culling {

class portal_sector_structure : public resources::unmanaged_resource {
public:
												portal_sector_structure		(
													memory::base_allocator*		arg_0 /* u32 portals_count */,
													u32							sectors_count,
													u32							arg_2
												);
	virtual										~portal_sector_structure	( );

			u32									get_sector_id				( memory::base_allocator& allocator, float3 const& pos ) const;
	inline	buffer_vector< portal > const&		get_portals					( ) const { /* no source */ }
	inline	buffer_vector< spatial_sector > const&	get_sectors					( ) const { /* no source */ }

			void								load						( configs::binary_config_value* value_ptr );

			void								sort_portal_ids				( float const* distances );

	inline	void								set_portal_visible			( u32 arg_0, bool arg_1 ) { /* no source */ }

			void								update_portals_visability	( math::frustum const& f, pcbyte oclusion_results );

private:
	inline	void								adjust_portals_orientation	( ) { /* no source */ }

			void								initialize_portals_geometry	( );

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	memory::base_allocator*					m_allocator;
	/* 0x010c */	void*									m_portals_buffer;
	/* 0x0110 */	buffer_vector< portal >					m_portals;
	/* 0x0118 */	u32*									m_portal_ids_buffer;
	/* 0x011c */	void*									m_sectors_buffer;
	/* 0x0120 */	buffer_vector< spatial_sector >			m_sectors;
	/* 0x0128 */	collision::space_partitioning_tree*		m_sectors_spatial_tree;
	/* 0x012c */	collision::space_partitioning_tree*		m_portals_spatial_tree;
	/* 0x0130 */	collision::geometry*					m_portals_geometry;
}; // class portal_sector_structure

STATIC_SIZE_ASSERT(portal_sector_structure, 0x138);

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CULLING_PORTAL_SECTOR_STRUCTURE_H_INCLUDED
