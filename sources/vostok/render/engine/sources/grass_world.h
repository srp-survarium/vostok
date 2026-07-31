#ifndef VOSTOK_RENDER_ENGINE_GRASS_WORLD_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_GRASS_WORLD_H_INCLUDED

#include <vostok/math_float2.h>
#include <vostok/math_float3.h>
#include <vostok/math_float4x4.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/facade/render_stage_types.h>
#include <vostok/resources_resource_ptr.h>
#include <vostok/resources_unmanaged_resource.h>

#include "grass_template.h"
#include "trample_desc.h"

namespace vostok {

namespace collision {

struct space_partitioning_tree;

} // namespace collision

namespace math {

class color;

} // namespace math

namespace resources {

class queries_result;

} // namespace resources

namespace render {

struct grass_layer_data;
struct grass_layer_desc;
class grass_patch;
class grass_render_model;
class grass_template;
class renderer;
class renderer_context;
class res_effect;
class shader_constant_host;

typedef resources::resource_ptr<
	grass_render_model,
	resources::unmanaged_intrusive_base
> grass_render_model_ptr;

struct sort_grass_patch_predicate {
	explicit sort_grass_patch_predicate( float3 const& view_position ) :
		m_view_pos( view_position )
	{
	}

	bool operator()( grass_patch const* left, grass_patch const* right ) const;

private:
	float3 m_view_pos;
};

STATIC_SIZE_ASSERT( sort_grass_patch_predicate, 0xC );

struct grass_world : public resources::unmanaged_resource {
	grass_world( );
	virtual ~grass_world( );

	void render(
		renderer_context* context,
		float3 const& viewer_position,
		enum_render_stage_type stage_type,
		u32 tech_index,
		float draw_distance,
		bool stencil_mask,
		res_effect* debug_effect,
		bool shadow_pass,
		u32 cascade_index
	);

	void accumulate_trample( renderer* in_renderer, renderer_context* in_context );
	void process_culling( renderer_context* context, float first_lod_distance );
	void process_sorting( float3 const& viewer_position, bool sort_instances );
	void render_debug( renderer_context* context );

	grass_patch* find_patch( float3 const&, grass_template* )
	{
		// claude@NOTE: no legacy ancestor - declaration-only in the legacy header; the body was consumed into the ported point-only overload; matcher-phase work.
		// STATE[STUB]
		return 0;
	}

	grass_patch* find_patch( float3 const& point );

	u32 find_patches( grass_template*, vector<grass_patch*>& )
	{
		// claude@NOTE: legacy body reads grass_patch::m_template directly - public in the legacy struct, but private in the canonical grass_patch with no friend/accessor for grass_world; not portable without inventing a friend; matcher-phase work.
		// STATE[STUB]
		return 0;
	}

	vector<grass_patch*>& get_visible_patches( )
	{
		return m_visible_patches;
	}

	void add_grass_layer(
		grass_layer_desc* desc,
		grass_layer_data* data,
		bool populate,
		bool from_cook
	);

	void update_grass_layer(
		grass_layer_desc* desc,
		grass_layer_data* layer_data,
		bool is_set,
		bool do_populate,
		bool from_cook
	);

	void remove_grass_layer( u8 id, bool do_populate );
	void remove_trample( );
	void populate( float patch_size_ground );
	void clear( );
	void add_trample( trample_desc const& desc );

private:
	void on_test_sub_resources_loaded( resources::queries_result& )
	{
		// claude@NOTE: legacy body diverged - legacy on_sub_resources_loaded calls the retired add_template/add_instance/populate signatures; matcher-phase work.
		// STATE[STUB]
	}

	void grass_layer_resources_ready(
		resources::queries_result& data,
		grass_layer_desc* desc,
		grass_layer_data* layer_data,
		bool do_populate
	);

	void grass_layer_resources_ready_from_cook(
		resources::queries_result& data,
		grass_layer_desc* desc,
		grass_layer_data* layer_data,
		bool do_populate
	);

	void remove_layer_instances( u8 id, float2 const& cell_lt, float2 const& cell_rb );

	void create_patch_render_buffers( )
	{
		// claude@NOTE: legacy body diverged - legacy loops grass_patch::create_render_buffer, a method the canonical patch does not have; matcher-phase work.
		// STATE[STUB]
	}

	void merge_patches( );
	u32 add_template( grass_render_model_ptr const& render_model );

	void remove_template( u32 in_id )
	{
		vector<grass_template*>::iterator	it_t			=	m_templates.begin();
		vector<grass_template*>::iterator	end_t			=	m_templates.end();

		for (; it_t != end_t; ++it_t)
		{
			grass_template* templ						=	*it_t;
			if (templ->m_index == in_id)
			{
				DELETE									(templ);
				m_templates.erase						(it_t);

#ifndef MASTER_GOLD
//				find_patches							(templ, m_patches_to_remove);
#endif // #ifndef MASTER_GOLD

				break;
			}
		}
	}

	u32 add_instance(
		u32 template_id,
		math::color const& color,
		float4x4 const& transform,
		u8 layer,
		float wind_scale
	);

	void remove_instance( u32 id );
	void remove_instances( vector<u32> const& ids );
	void remove_patches( );
	grass_template* id_to_template( u32 id ) const;
	grass_template* find_template( grass_render_model_ptr const& model ) const;

public:
	void set_patch_parameters( grass_patch* patch );
	void set_wind_parameters( float2 const& direction, float strength );
	void set_trample_parameters( trample_desc& desc );
	void set_shadow_parameters( u32 cascade_index );

private:
	vector<trample_desc>				m_trample_array;
	vector<grass_template*>			m_templates;
	vector<grass_patch*>				m_patches;
	vector<grass_patch*>				m_visible_patches;
	collision::space_partitioning_tree*	m_patches_tree;
	shader_constant_host*				m_ambient_color;
	shader_constant_host*				m_c_environment_skylight_upper_color;
	shader_constant_host*				m_c_environment_skylight_lower_color;
	shader_constant_host*				m_c_environment_skylight_parameters;
	shader_constant_host*				m_c_sun_direction;
	shader_constant_host*				m_c_sun_color;
	shader_constant_host*				m_patch_parameters;
	shader_constant_host*				m_trample_parameters;
	shader_constant_host*				m_shadow_cascade_index_parameter;
	shader_constant_host*				m_wind_info_parameters;

public:
	bool m_need_populate;
};

STATIC_SIZE_ASSERT( grass_world, 0x168 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_GRASS_WORLD_H_INCLUDED
