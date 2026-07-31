#ifndef VOSTOK_RENDER_ENGINE_GRASS_PATCH_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_GRASS_PATCH_H_INCLUDED

#include <vostok/math_aabb.h>
#include <vostok/render/core/untyped_buffer.h>
#include <vostok/render/facade/render_stage_types.h>
#include "grass_instance.h"
#include "grass_template.h"
#include "render_target.h"
#include "res_geometry.h"
#include "res_texture.h"

namespace vostok {

namespace collision {

class geometry_instance;
class object;
struct space_partitioning_tree;

} // namespace collision

namespace render {

struct grass_world;
class renderer;
class renderer_context;
class res_effect;
struct trample_desc;

struct grass_patch : public boost::noncopyable {
	struct sort_info {
		sort_info( ) :
			position( 0.f, 0.f, 0.f ),
			index_offset( 0 ),
			num_indices( 0 )
		{
		}

		float3 position;
		u32 index_offset;
		u32 num_indices;
	};

	STATIC_SIZE_ASSERT( sort_info, 0x14 );

	grass_patch(
		collision::space_partitioning_tree* in_collision_tree,
		grass_template* templ,
		float3 const& in_origin,
		float in_size
	);
	~grass_patch( );

	void merge_instances( );
	void sort_instances( float3 const& view_position );
	void init_collision( );
	u32 get_stream_1_stride( ) const;
	u32 get_index_count( ) const;

	math::aabb const& get_aabb( ) const
	{
		return m_aabb;
	}

	bool is_occluded( ) const;
	void remove_trample( );

	void render(
		grass_world* in_grass_world,
		renderer_context* context,
		float3 const& viewer_position,
		enum_render_stage_type stage_type,
		u32 tech_index,
		float draw_distance,
		res_effect* debug_effect,
		u32 cascade_index
	);

	u32 get_valid_lod_index( u32 lod_index ) const;

	void update_movement_texture( renderer_context* )
	{
		// claude@NOTE: no legacy ancestor - no movement-texture token anywhere in the legacy corpus; matcher-phase work.
		// STATE[STUB]
	}

	void try_accumulate_trample(
		trample_desc& desc,
		grass_world* in_grass_world,
		renderer* in_renderer,
		renderer_context* in_context
	);

private:
	float m_movement_data[64][64];
	render_target_ptr m_movement_rt;
	res_texture_ptr m_movement_texture;
	float3 m_prev_view_pos;
	math::aabb m_aabb;
	float3 m_origin;
	float m_size;
	u32 m_occlusion_info_index;
	u32 m_current_lod_index;
	u32 m_num_avaliable_lods;
	vector<grass_instance*> m_instances;
	res_geometry_ptr m_geometry[3];
	untyped_buffer_ptr m_vb_stream_1[3];
	u16* m_merged_indices[3];
	sort_info* m_sort_info[3];
	u32 m_num_merged_vertices[3];
	u32 m_num_merged_indices[3];
	grass_template* m_template;
	collision::space_partitioning_tree* const m_collision_tree;
	collision::geometry_instance* m_collision_geometry;
	collision::object* m_collision_object;
	bool m_visible;
	bool m_occluded;
};

STATIC_SIZE_ASSERT( grass_patch, 0x40B0 );

bool has_surface_by_lod( grass_render_model_ptr model, u32 lod_index );
grass_render_surface* surface_by_lod( grass_render_model_ptr model, u32 lod_index );
math::color transform_packed_normal( float4x4 const& transform_matrix, math::color const& packed_normal );

} // namespace render
} // namespace vostok

#endif // VOSTOK_RENDER_ENGINE_GRASS_PATCH_H_INCLUDED
