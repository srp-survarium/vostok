////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_WORLD_H_INCLUDED
#define RENDER_GRASS_WORLD_H_INCLUDED

/* INCLUDES */
class vostok::render::shader_constant_host;
class vostok::render::vector<vostok::render::grass_patch *>;
class vostok::render::vector<vostok::render::grass_template *>;
class vostok::render::vector<vostok::render::trample_desc>;
class vostok::resources::unmanaged_resource;
struct vostok::collision::space_partitioning_tree;
class vostok::render::enum_render_stage_type;
class vostok::render::grass_render_model;

/* FORWARD REFS */
class vostok::math::color;
class vostok::render::grass_layer_data;
class vostok::render::grass_layer_desc;
class vostok::render::grass_patch;
class vostok::render::grass_template;
class vostok::render::renderer;
class vostok::render::renderer_context;
class vostok::render::res_effect;
class vostok::render::trample_desc;
class vostok::render::vector<unsigned int>;
class vostok::resources::queries_result;

namespace vostok {
namespace render {

struct grass_world : public resources::unmanaged_resource {
										grass_world					( );
	virtual								~grass_world				( );

			void						render						(
											renderer_context*			context,
											float3 const&				viewer_position,
											enum_render_stage_type		stage_type,
											const u32					tech_index,
											const float					draw_distance,
											bool						stencil_mask,
											res_effect*					debug_effect,
											bool						shadow_pass,
											const u32					cascade_index
										);

			void						accumulate_trample			( renderer* in_renderer, renderer_context* in_context );

			void						process_culling				( renderer_context* context, const float first_lod_distance );
			void						process_sorting				( float3 const& viewer_position, bool sort_instances );

			void						render_debug				( renderer_context* context );

	inline	grass_patch*				find_patch					( float3 const& arg_0, grass_template* arg_1 ) { /* no source */ }
			grass_patch*				find_patch					( float3 const& point );
	inline	u32							find_patches				( grass_template* arg_0, vector< grass_patch* >& arg_1 ) { /* no source */ }
	inline	vector< grass_patch* >&		get_visible_patches			( ) { /* no source */ }

			void						add_grass_layer				(
											grass_layer_desc*		desc,
											grass_layer_data*		data,
											bool					populate,
											bool					from_cook
										);
			void						update_grass_layer			(
											grass_layer_desc*		desc,
											grass_layer_data*		layer_data,
											bool					is_set,
											bool					do_populate,
											bool					from_cook
										);
			void						remove_grass_layer			( u8 id, bool do_populate );
			void						remove_trample				( );

			void						populate					( const float patch_size_ground );

			void						clear						( );

			void						add_trample					( trample_desc const& desc );

private:
	inline	void						on_test_sub_resources_loaded( resources::queries_result& arg_0 ) { /* no source */ }

			void						grass_layer_resources_ready	(
											resources::queries_result&		data,
											grass_layer_desc*				desc,
											grass_layer_data*				layer_data,
											bool							do_populate
										);
			void						grass_layer_resources_ready_from_cook(
											resources::queries_result&		data,
											grass_layer_desc*				desc,
											grass_layer_data*				layer_data,
											bool							do_populate
										);

			void						remove_layer_instances		( u8 id, float2 const& cell_lt, float2 const& cell_rb );

	inline	void						create_patch_render_buffers	( ) { /* no source */ }

			void						merge_patches				( );

			u32							add_template				( grass_render_model_ptr const& in_render_model );
	inline	void						remove_template				( const u32 arg_0 ) { /* no source */ }

			u32							add_instance				(
											const u32				in_template_id,
											math::color const&		in_color,
											float4x4 const&			in_transform,
											const u8				in_layer,
											const float				in_wind_scale
										);
			void						remove_instance				( const u32 in_id );
			void						remove_instances			( vector< u32 > const& v );
			void						remove_patches				( );

			grass_template*				id_to_template				( const u32 id ) const;
			grass_template*				find_template				( grass_render_model_ptr const& model ) const;

public:
			void						set_patch_parameters		( grass_patch* patch );
			void						set_wind_parameters			( float2 const& dir, const float strength );
			void						set_trample_parameters		( trample_desc& desc );
			void						set_shadow_parameters		( const u32 cascade_index );

	/* 0x0000 */	/* resources::unmanaged_resource */
private:
	/* 0x0108 */	vector< trample_desc >					m_trample_array;
	/* 0x0114 */	vector< grass_template* >				m_templates;
	/* 0x0120 */	vector< grass_patch* >					m_patches;
	/* 0x012c */	vector< grass_patch* >					m_visible_patches;
	/* 0x0138 */	collision::space_partitioning_tree*		m_patches_tree;
	/* 0x013c */	shader_constant_host*					m_ambient_color;
	/* 0x0140 */	shader_constant_host*					m_c_environment_skylight_upper_color;
	/* 0x0144 */	shader_constant_host*					m_c_environment_skylight_lower_color;
	/* 0x0148 */	shader_constant_host*					m_c_environment_skylight_parameters;
	/* 0x014c */	shader_constant_host*					m_c_sun_direction;
	/* 0x0150 */	shader_constant_host*					m_c_sun_color;
	/* 0x0154 */	shader_constant_host*					m_patch_parameters;
	/* 0x0158 */	shader_constant_host*					m_trample_parameters;
	/* 0x015c */	shader_constant_host*					m_shadow_cascade_index_parameter;
	/* 0x0160 */	shader_constant_host*					m_wind_info_parameters;
public:
	/* 0x0164 */	bool									m_need_populate;
}; // struct grass_world

STATIC_SIZE_ASSERT(grass_world, 0x168);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_WORLD_H_INCLUDED
