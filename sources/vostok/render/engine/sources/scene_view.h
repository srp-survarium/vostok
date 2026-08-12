#ifndef VOSTOK_RENDER_ENGINE_SCENE_VIEW_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SCENE_VIEW_H_INCLUDED

#include <vostok/render/core/memory.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/render/engine/base_classes.h>
#include <vostok/render/facade/particles.h>
#include <vostok/render/facade/scene_view_mode.h>
#include <vostok/scaleform/sources/flash_movie_resource.h>
#include <vostok/vectora.h>

#include "camera.h"
#include "light.h"
#include "post_process_parameters.h"

namespace survarium {
struct flash_text_manager;
}

namespace vostok {

namespace particle {
struct render_particle_emitter_instance;
}

namespace render {

struct ambient_volume;
struct environment_probe;
struct decal_instance;
struct grass_patch;
struct render_surface_instance;

struct culling_result {
	void* user_data;
};

class scene_view : public base_scene_view {
public:
	scene_view( );
	virtual ~scene_view( );

	void camera_set_view( float4x4 const& transform )
	{
		m_camera.set_view_transform( transform );
	}

	void camera_set_projection( float4x4 const& transform )
	{
		m_camera.set_projection_transform( transform );
	}

	render::camera const& camera( ) const { return m_camera; }

	render::post_process_parameters& post_process_parameters( )
	{
		return m_post_process_parameters;
	}

	render::post_process_parameters const& post_process_parameters( ) const
	{
		return m_post_process_parameters;
	}

	void add_movie( survarium::flash_movie_resource_ptr& movie );
	void remove_movie( survarium::flash_movie_resource_ptr& movie );
	void add_text_manager( survarium::flash_text_manager* tm );
	void remove_text_manager( survarium::flash_text_manager* tm );

	vector< survarium::flash_movie_resource_ptr > const& flash_movies( ) const
	{
		return m_flash_movies;
	}

	survarium::flash_text_manager* flash_text_manager( ) const
	{
		return m_flash_text_manager;
	}

	void set_view_mode( scene_view_mode view_mode ) { m_view_mode = view_mode; }
	scene_view_mode get_view_mode( ) const { return m_view_mode; }

	float4 const& get_luminance_parameters( ) const
	{
		return m_frame_luminance_parameters;
	}

	float4 const& get_prev_luminance_parameters( ) const
	{
		return m_prev_frame_luminance_parameters;
	}

	void set_luminance_parameters( float4 const& parameters )
	{
		m_frame_luminance_parameters = parameters;
	}

	void set_prev_luminance_parameters( float4 const& parameters )
	{
		m_prev_frame_luminance_parameters = parameters;
	}

	u32 get_render_frame_index( ) const { return m_render_frame_index; }

	void set_use_post_process( bool value ) { m_use_post_process = value; }
	bool is_use_post_process( ) const { return m_use_post_process; }

	void set_particles_render_mode( particle::enum_particle_render_mode mode )
	{
		m_particles_render_mode = mode;
	}

	particle::enum_particle_render_mode get_particles_render_mode( ) const
	{
		return m_particles_render_mode;
	}

	render::post_process_parameters m_post_process_parameters;

	vector< render_surface_instance* >& get_visible_models( )
	{
		return m_visible_models;
	}

	vector< render_surface_instance* >& get_visible_opaque_models( )
	{
		return m_visible_opaque_models;
	}

	vector< render_surface_instance* >& get_visible_moved_opaque_models( )
	{
		return m_visible_moved_models;
	}

	vector< light_ptr >& get_visible_lights( ) { return m_visible_lights; }

	vectora< particle::render_particle_emitter_instance* >&
	get_visible_particle_instances( )
	{
		return m_visible_particle_instances;
	}

	vector< decal_instance* >& get_visible_decals( ) { return m_visible_decals; }
	vector< environment_probe* >& get_visible_environment_probes( )
	{
		return m_visible_environment_probes;
	}
	vector< grass_patch* >& get_visible_grass_patches( )
	{
		return m_visible_grass_patches;
	}
	vector< ambient_volume* >& get_visible_ambient_volumes( )
	{
		return m_visible_ambient_volumes;
	}

	void get_histogram_values( float* values )
	{
		memory::copy(
			values,
			sizeof( m_histogram_values ),
			m_histogram_values,
			sizeof( m_histogram_values )
		);
	}

	void set_histogram_values( float* values )
	{
		memory::copy(
			m_histogram_values,
			sizeof( m_histogram_values ),
			values,
			sizeof( m_histogram_values )
		);
	}

public:
	u32 luminance_pass_index;
	bool need_recalc_atmosphere;

private:
	friend class renderer;
	friend class stage_atmosphere;

	void inc_render_frame_index( ) { ++m_render_frame_index; }

	render::camera m_camera;
	scene_view_mode m_view_mode;
	particle::enum_particle_render_mode m_particles_render_mode;
	bool m_use_post_process;
	u32 m_render_frame_index;
	float4 m_frame_luminance_parameters;
	float4 m_prev_frame_luminance_parameters;
	float m_histogram_values[16];
	vector< render_surface_instance* > m_visible_moved_models;
	vector< render_surface_instance* > m_visible_models;
	vector< render_surface_instance* > m_visible_opaque_models;
	vector< light_ptr > m_visible_lights;
	vector< decal_instance* > m_visible_decals;
	vector< environment_probe* > m_visible_environment_probes;
	vectora< particle::render_particle_emitter_instance* >
		m_visible_particle_instances;
	vector< grass_patch* > m_visible_grass_patches;
	vector< ambient_volume* > m_visible_ambient_volumes;
	vector< survarium::flash_movie_resource_ptr > m_flash_movies;
	survarium::flash_text_manager* m_flash_text_manager;
	u32 m_num_frustum_culled_surfaces;
	u32 m_num_frustum_culled_lights;
	u32 m_num_frustum_culled_grass_patches;
	u32 m_num_frustum_culled_particle_instances;
	u32 m_num_frustum_culled_decals;
	u32 m_num_occlusion_culled_surfaces;
	u32 m_num_occlusion_culled_lights;
	u32 m_num_occlusion_culled_grass_patches;
	u32 m_num_occlusion_culled_particle_instances;
	u32 m_num_occlusion_culled_decals;
	pbyte m_occlusion_culling_results;
	u32 m_num_occlusion_culling_results;
	culling_result* m_culling_results;
	float4* m_culling_bounds;
	u32 m_atmosphere_prev_targets_size;
};

STATIC_SIZE_ASSERT( scene_view, 0x5E0 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SCENE_VIEW_H_INCLUDED
