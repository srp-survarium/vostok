#ifndef VOSTOK_RENDER_FACADE_SOURCES_SCENE_RENDERER_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_SCENE_RENDERER_H_INCLUDED

#include <vostok/detail_noncopyable.h>
#include <vostok/math_float4x4.h>
#include <vostok/math_frustum.h>
#include <vostok/math_uint2.h>
#include <vostok/render/engine/base_classes.h>
#include <vostok/render/facade/model.h>
#include <vostok/render/facade/render_stage_types.h>
#include <vostok/render/facade/scene_view_mode.h>
#include <vostok/resources_unmanaged_resource.h>
#include <vostok/vectora.h>

namespace vostok {

namespace fs_new {
class virtual_path_string;
}

namespace memory {
class base_allocator;
}

namespace particle {
enum enum_particle_render_mode;
struct world;

typedef resources::resource_ptr<
	resources::unmanaged_resource,
	resources::unmanaged_intrusive_base
> particle_system_instance_ptr;
}

namespace render {

namespace engine {
class world;
}

namespace game {
class renderer;
}

class one_way_render_channel;
class speedtree_instance;
struct tracer_model_instance;

struct ambient_volume_properties;
struct buffer_fragment;
struct cloud_key_parameters;
struct cloud_parameters;
struct decal_properties;
struct environment_probe_properties;
struct grass_layer_data;
struct grass_layer_desc;
struct light_props;
struct sky_ambient_occlusion_properties;
struct trample_desc;
struct volume_fog_parameters;

typedef resources::resource_ptr<
	speedtree_instance,
	resources::unmanaged_intrusive_base
> speedtree_instance_ptr;

typedef resources::resource_ptr<
	tracer_model_instance,
	resources::unmanaged_intrusive_base
> tracer_model_instance_ptr;

class scene_renderer : private core::noncopyable {
private:
	friend class game::renderer;

	scene_renderer(
		one_way_render_channel& channel,
		memory::base_allocator& allocator,
		engine::world& render_engine_world,
		math::frustum* frustum_listener
	);

public:
	void reload_shaders( );
	void reload_modified_textures( );

	math::uint2 window_client_size( base_output_window_ptr const& )
	{
		// no source
		return math::uint2( 0, 0 );
	}

	void set_view_matrix( base_scene_view_ptr const& scene_view, float4x4 const& view_and_culling_matrix );
	void set_projection_matrix( base_scene_view_ptr const& scene_view, float4x4 const& projection );

	void begin_render_options_changing( long volatile* waiting_for );
	void end_render_options_changing(
		base_scene_ptr const& scene,
		base_output_window_ptr output_window,
		bool reload_all_materials,
		bool shaders_recompile,
		long volatile* waiting_for
	);
	void reset_renderer( );

	void add_model( base_scene_ptr const& scene, render_model_instance_ptr const& model, float4x4 const& transform );
	void update_model( base_scene_ptr const& scene, render_model_instance_ptr const& model, float4x4 const& model_to_world );
	void remove_model( base_scene_ptr const& scene, render_model_instance_ptr const& model );
	inline void update_model_vertex_buffer(
		render_model_instance_ptr const& object,
		vectora< buffer_fragment > const& fragments
	);
	void update_skeleton( render_model_instance_ptr const& model, float4x4 const* matrices, u32 count );
	void set_model_ghost_mode( render_model_instance_ptr const&, bool ) { /* no source */ }
	inline void set_speedtree_instance_material(
		speedtree_instance_ptr const&,
		fs_new::virtual_path_string const&,
		resources::unmanaged_resource_ptr const&
	);
	inline void set_model_material(
		render_model_instance_ptr const&,
		fs_new::virtual_path_string const&,
		resources::unmanaged_resource_ptr const&
	);
	inline void set_model_visible(
		render_model_instance_ptr const&,
		fs_new::virtual_path_string const&,
		u32
	);
	void set_model_visible( render_model_instance_ptr const& model, u32 surface_id, u32 flags );
	inline void set_model_lod_params( render_model_instance_ptr const& model, u8 type, bool use_default, float p0, float p1, float p2 );

	void build_lpv_geometry( base_scene_ptr const& scene );
	inline void remove_unused_environment_cubemaps( base_scene_ptr const& scene );

	void add_vegetation_trample( base_scene_ptr const& scene, trample_desc const& desc );
	inline void add_speedtree_instance(
		base_scene_ptr const&,
		speedtree_instance_ptr const&,
		float4x4 const&,
		bool
	);
	inline void remove_speedtree_instance( base_scene_ptr const&, speedtree_instance_ptr const&, bool );
	inline void update_speedtree_instance(
		base_scene_ptr const&,
		speedtree_instance_ptr const&,
		float4x4 const&,
		bool
	);
	inline void populate_speedtree_forest( base_scene_ptr const& );

	void play_particle_system(
		base_scene_ptr const& scene,
		const resources::unmanaged_resource_ptr instance,
		float4x4 const& transform
	);
	void stop_particle_system( base_scene_ptr const&, resources::unmanaged_resource_ptr const& ) { /* no source */ }
	void remove_particle_system_instance( base_scene_ptr const& scene, resources::unmanaged_resource_ptr const& instance );
	void update_particle_system_instance(
		base_scene_ptr const& scene,
		resources::unmanaged_resource_ptr const& instance,
		float4x4 const& transform,
		bool visible = true,
		bool paused = false
	);
	bool is_playing( resources::unmanaged_resource_ptr const& instance );
	void set_particles_render_mode( base_scene_view_ptr const&, particle::enum_particle_render_mode ) { /* no source */ }
	void set_slomo( base_scene_ptr const&, float ) { /* no source */ }
	void set_gamma_correction_factor( const float value );

	void add_light( base_scene_ptr const& scene, u32 id, light_props* props );
	void update_light( base_scene_ptr const& scene, u32 id, light_props* props );
	void remove_light( base_scene_ptr const& scene, u32 id );

	void add_decal( base_scene_ptr const&, u32, decal_properties const& ) { /* no source */ }
	void update_decal( base_scene_ptr const& scene, u32 id, decal_properties const& properties );
	void remove_decal( base_scene_ptr const& scene, u32 id );

	void update_environment_probe( base_scene_ptr const& scene, u32 id, environment_probe_properties const& properties );
	void remove_environment_probe( base_scene_ptr const& scene, u32 id );
	void update_sky_ambient_occlusion( base_scene_ptr const& scene, u32 id, sky_ambient_occlusion_properties const& properties );
	void remove_sky_ambient_occlusion( base_scene_ptr const& scene, u32 id );
	void update_ambient_volume( base_scene_ptr const& scene, u32 id, ambient_volume_properties const& properties );
	void remove_ambient_volume( base_scene_ptr const& scene, u32 id );

	void add_tracer( base_scene_ptr const& scene, tracer_model_instance_ptr const& instance, float4x4 const& transform );
	void update_tracer( base_scene_ptr const& scene, tracer_model_instance_ptr const& instance, float4x4 const& transform );
	void remove_tracer( base_scene_ptr const& scene, tracer_model_instance_ptr const& instance );

	void update_lpv_occluder( base_scene_ptr const& scene, u32 id, float4x4 const& transform );
	void remove_lpv_occluder( base_scene_ptr const& scene, u32 id );
	inline void add_volume_fog( base_scene_ptr const& scene, u32 id, volume_fog_parameters const& parameters );
	void update_volume_fog( base_scene_ptr const& scene, u32 id, volume_fog_parameters const& parameters );
	void remove_volume_fog( base_scene_ptr const& scene, u32 id );

	inline void add_clouds( base_scene_ptr const& scene, cloud_parameters const& parameters );
	inline void update_clouds( base_scene_ptr const& scene, cloud_parameters const& parameters );
	inline void remove_clouds( base_scene_ptr const& scene );
	inline void set_num_clouds_keys( base_scene_ptr const& scene, const u32 num_keys );
	inline void apply_clouds_changes( base_scene_ptr const& scene );
	inline void set_clouds_key( base_scene_ptr const& scene, const u32 index, cloud_key_parameters const& parameters );
	inline void set_clouds_time( base_scene_ptr const& scene, const float time );
	inline void set_editor_mode( base_scene_ptr const& scene, bool is_editor_mode );

	void set_post_process( base_scene_view_ptr const& scene_view, resources::unmanaged_resource_ptr const& resource );
	void enable_post_process( base_scene_view_ptr const&, bool ) { /* no source */ }
	void set_view_mode( base_scene_view_ptr const&, scene_view_mode ) { /* no source */ }
	void toggle_render_stage( enum_render_stage_type, bool ) { /* no source */ }

	particle::world& particle_world( base_scene_ptr const& scene );
	void set_sky_material( base_scene_ptr const& scene, resources::unmanaged_resource_ptr const& material );
	void set_portal_system( base_scene_ptr const& scene, resources::unmanaged_resource_ptr const& portal_system );
	void test_action_portal_system( base_scene_ptr const& ) { /* no source */ }

	inline void clear_grass( base_scene_ptr const& scene );
	inline void populate_grass( base_scene_ptr const& scene );
	inline void add_grass_layer( grass_layer_desc* desc, grass_layer_data* data, base_scene_ptr const& scene );
	inline void update_grass_layer( grass_layer_desc* desc, grass_layer_data* data, base_scene_ptr const& scene, bool is_set );
	inline void remove_grass_layer( u8 id, base_scene_ptr const& scene );
	void set_grass( resources::unmanaged_resource_ptr grass, base_scene_ptr const& scene );
	void reset_grass( resources::unmanaged_resource_ptr grass, base_scene_ptr const& scene );

	void set_fixed_lod( u32, base_scene& ) { /* no source */ }
	u32 get_fixed_lod( base_scene const& ) { /* no source */ return 0; }

private:
	void draw_present_impl( ) { /* no source */ }

private:
	engine::world& m_render_engine_world;
	one_way_render_channel& m_channel;
	memory::base_allocator& m_allocator;
	math::frustum* const m_frustum_listener;
	float4x4 m_view;
	float4x4 m_projection;
};

STATIC_SIZE_ASSERT( scene_renderer, 0x90 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_SCENE_RENDERER_H_INCLUDED
