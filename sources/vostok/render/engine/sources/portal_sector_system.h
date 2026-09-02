// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_ENGINE_PORTAL_SECTOR_SYSTEM_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_PORTAL_SECTOR_SYSTEM_H_INCLUDED
#include <vostok/buffer_vector.h>
#include <vostok/math_float3.h>
#include <vostok/math_float4.h>
#include <vostok/math_float4x4.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/culling/aab_rect.h>
#include <vostok/render/culling/portal_sector_structure.h>
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {

namespace collision {

class geometry;
struct space_partitioning_tree;

} // namespace collision

namespace configs {

class binary_config_value;

} // namespace configs

namespace math {

class frustum;
class plane;

} // namespace math

namespace render {

struct render_surface_instance;
class system_renderer;

namespace culling {

class sector_double_query_preventer;

class portal_sector_system : public resources::unmanaged_resource {
public:
	explicit portal_sector_system( portal_sector_structure_ptr structure );
	virtual ~portal_sector_system( );

	void render( system_renderer& renderer, float3 const& view_pos, float4x4 const& matrix );

	void load( configs::binary_config_value* )
	{
		// STATE[STUB]
	}

	void test_action( );

	void select_models(
		collision::space_partitioning_tree* tree,
		float3 const& view_pos,
		float4x4 const& mat_vp,
		vector<render_surface_instance*>& selection
	);

	u32 get_portals_count( ) const
	{
		return m_structure->get_portals( ).size( );
	}

	void get_portals_occlusion_bounds( float4* bounds );
	void update_portals_occlusion_culling( pcbyte occlusion_results );

private:
	typedef buffer_vector<spatial_sector> sectors_type;
	typedef buffer_vector<portal> portals_type;

	void draw_quads( system_renderer& renderer );
	void draw_portals( system_renderer& renderer, u32 const active_sector_id );

	void process_sector(
		u32 sector_id,
		u32 input_portal_id,
		float3 const& view_pos,
		math::frustum const& frustum
	);

	void process_sector(
		u32 sector_id,
		u32 input_portal_id,
		buffer_vector<aab_rect> const& portals_rects,
		float3 const& view_pos,
		math::plane const& far_plane,
		float4x4 const& mat_vp,
		float4x4 const& inv_mat_vp,
		aab_rect const& rect
	);

	void process_portal_by_frustum_intersection(
		u32 portal_id,
		math::frustum const& frustum,
		u32 sector_id,
		float3 const& view_pos
	);

	void perform_frustum_culling_and_sectors_test(
		collision::space_partitioning_tree* tree,
		u32 active_sector_id,
		math::frustum const& frustum,
		vector<render_surface_instance*>& visible_surfaces,
		float3 const& view_pos,
		float4x4 const& mat_vp
	);

	void initialize_portals_occlusion_bounds_and_results( );
	void make_frustum_images( float3 const& view_dir );

	typedef buffer_vector<aab_rect> aab_rects_buffer_type;

	void process_portal_in_screen_space(
		u32 portal_id,
		u32 sector_id,
		aab_rects_buffer_type const& portals_rects,
		float3 const& view_pos,
		math::plane const& far_plane,
		float4x4 const& mat_vp,
		float4x4 const& inv_mat_vp,
		aab_rect const& limiting_rect
	);

	typedef buffer_vector<float> float_buffer_type;

	void sort_portals_and_calculate_rects_in_screen_space(
		float4x4 const& mat_vp,
		float min_z,
		aab_rects_buffer_type& rects
	);

	void calculate_portal_rects_in_screen_space(
		float4x4 const& mat_vp,
		float min_z,
		aab_rects_buffer_type& rects,
		float_buffer_type& distances
	);

private:
	portal_sector_structure_ptr	m_structure;

	struct quad {
		float3 vertices[4];
	};

	STATIC_SIZE_ASSERT( quad, 0x30 );

	typedef vector<quad> quads_type;
	quads_type					m_quads;
	bool						m_test_action;
	sector_double_query_preventer*	m_preventer;
	typedef buffer_vector<float4> occlusion_bounds_buffer_type;
	void*						m_occlusion_bounds_buffer;
	occlusion_bounds_buffer_type	m_occlusion_bounds;
	typedef buffer_vector<u8> bytes_buffer_type;
	void*						m_occlusion_results_buffer;
	bytes_buffer_type				m_occlusion_results;
	collision::geometry*			m_portals_geometry;
};

STATIC_SIZE_ASSERT( portal_sector_system, 0x140 );

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_PORTAL_SECTOR_SYSTEM_H_INCLUDED
