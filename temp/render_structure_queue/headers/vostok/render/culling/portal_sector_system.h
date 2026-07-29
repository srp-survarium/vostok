////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_CULLING_PORTAL_SECTOR_SYSTEM_H_INCLUDED
#define RENDER_CULLING_PORTAL_SECTOR_SYSTEM_H_INCLUDED

/* INCLUDES */
class vostok::buffer_vector<unsigned char>;
class vostok::buffer_vector<vostok::math::float4>;
class vostok::collision::geometry;
class vostok::render::culling::sector_double_query_preventer;
class vostok::render::vector<vostok::render::culling::portal_sector_system::quad>;
class vostok::resources::unmanaged_resource;
typedef vostok::resources::resource_ptr<vostok::render::culling::portal_sector_structure,vostok::resources::unmanaged_intrusive_base>
	vostok::render::culling::portal_sector_structure_ptr;
class vostok::render::culling::portal_sector_structure;

/* FORWARD REFS */
class vostok::buffer_vector<float>;
class vostok::buffer_vector<vostok::render::culling::aab_rect>;
class vostok::collision::space_partitioning_tree;
class vostok::configs::binary_config_value;
class vostok::math::frustum;
class vostok::math::plane;
class vostok::render::culling::aab_rect;
class vostok::render::system_renderer;
class vostok::render::vector<vostok::render::render_surface_instance *>;

namespace vostok {
namespace render {
namespace culling {

class portal_sector_system : public resources::unmanaged_resource {
public:
			explicit	portal_sector_system			( portal_sector_structure_ptr structure );
	virtual				~portal_sector_system			( );

			void		render							( system_renderer& r, float3 const& view_pos, float4x4 const& __formal );

	inline	void		load							( configs::binary_config_value* arg_0 ) { /* no source */ }

			void		test_action						( );

			void		select_models					(
							collision::space_partitioning_tree*		tree,
							float3 const&							view_pos,
							float4x4 const&							mat_vp,
							vector< render_surface_instance* >&		selection
						);

	inline	u32			get_portals_count				( ) const { /* no source */ }
			void		get_portals_occlusion_bounds	( float4* bounds );

			void		update_portals_occlusion_culling( pcbyte occlusion_results );

private:
			void		draw_quads						( system_renderer& r );
			void		draw_portals					( system_renderer& r, const u32 active_sector_id );

			void		process_sector					(
							u32						sector_id,
							u32						input_portal_id,
							float3 const&			view_pos,
							math::frustum const&	f
						);
			void		process_sector					(
							u32									sector_id,
							u32									input_portal_id,
							buffer_vector< aab_rect > const&	portals_rects,
							float3 const&						view_pos,
							math::plane const&					far_plane,
							float4x4 const&						mat_vp,
							float4x4 const&						inv_mat_vp,
							aab_rect const&						rect
						);
			void		process_portal_by_frustum_intersection(
							u32						portal_id,
							math::frustum const&	f,
							u32						sector_id,
							float3 const&			view_pos
						);

			void		perform_frustum_culling_and_sectors_test(
							collision::space_partitioning_tree*		tree,
							u32										active_sector_id,
							math::frustum const&					f,
							vector< render_surface_instance* >&		visible_surfaces,
							float3 const&							view_pos,
							float4x4 const&							mat_vp
						);

			void		initialize_portals_occlusion_bounds_and_results( );

			void		make_frustum_images				( float3 const& view_dir );

			void		process_portal_in_screen_space	(
							u32									portal_id,
							u32									sector_id,
							buffer_vector< aab_rect > const&	portals_rects,
							float3 const&						view_pos,
							math::plane const&					far_plane,
							float4x4 const&						mat_vp,
							float4x4 const&						inv_mat_vp,
							aab_rect const&						limiting_rect
						);
			void		sort_portals_and_calculate_rects_in_screen_space( float4x4 const& mat_vp, float min_z, buffer_vector< aab_rect >& rects );
			void		calculate_portal_rects_in_screen_space(
							float4x4 const&					mat_vp,
							float							min_z,
							buffer_vector< aab_rect >&		rects,
							buffer_vector< float >&			distances
						);

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	portal_sector_structure_ptr			m_structure;
	/* 0x010c */	vector< portal_sector_system::quad >	m_quads;
	/* 0x0118 */	bool								m_test_action;
	/* 0x011c */	sector_double_query_preventer*		m_preventer;
	/* 0x0120 */	void*								m_occlusion_bounds_buffer;
	/* 0x0124 */	buffer_vector< float4 >				m_occlusion_bounds;
	/* 0x012c */	void*								m_occlusion_results_buffer;
	/* 0x0130 */	buffer_vector< u8 >					m_occlusion_results;
	/* 0x0138 */	collision::geometry*				m_portals_geometry;
}; // class portal_sector_system

STATIC_SIZE_ASSERT(portal_sector_system, 0x140);

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_CULLING_PORTAL_SECTOR_SYSTEM_H_INCLUDED
