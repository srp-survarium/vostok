#ifndef VOSTOK_RENDER_ENGINE_PORTAL_SECTOR_SYSTEM_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_PORTAL_SECTOR_SYSTEM_H_INCLUDED

// claude@NOTE: legacy-harvest disposition: load(config) and get_portals_count have no legacy ancestor (legacy model_manager loads via chunk_reader into heap arrays and exposes m_portals directly) - matcher-phase work.
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
	struct quad {
		quad( ) { }

		float3 vertices[4];
	};

	STATIC_SIZE_ASSERT( quad, 0x30 );

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
	void draw_quads( system_renderer& renderer );
	void draw_portals( system_renderer& renderer, u32 active_sector_id );

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

	void process_portal_in_screen_space(
		u32 portal_id,
		u32 sector_id,
		buffer_vector<aab_rect> const& portals_rects,
		float3 const& view_pos,
		math::plane const& far_plane,
		float4x4 const& mat_vp,
		float4x4 const& inv_mat_vp,
		aab_rect const& limiting_rect
	);

	void sort_portals_and_calculate_rects_in_screen_space(
		float4x4 const& mat_vp,
		float min_z,
		buffer_vector<aab_rect>& rects
	);

	void calculate_portal_rects_in_screen_space(
		float4x4 const& mat_vp,
		float min_z,
		buffer_vector<aab_rect>& rects,
		buffer_vector<float>& distances
	);

private:
	portal_sector_structure_ptr	m_structure;
	vector<quad>					m_quads;
	bool						m_test_action;
	sector_double_query_preventer*	m_preventer;
	void*						m_occlusion_bounds_buffer;
	buffer_vector<float4>			m_occlusion_bounds;
	void*						m_occlusion_results_buffer;
	buffer_vector<u8>				m_occlusion_results;
	collision::geometry*			m_portals_geometry;
};

STATIC_SIZE_ASSERT( portal_sector_system, 0x140 );

} // namespace culling
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_PORTAL_SECTOR_SYSTEM_H_INCLUDED
