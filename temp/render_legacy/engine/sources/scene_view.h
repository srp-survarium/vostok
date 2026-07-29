////////////////////////////////////////////////////////////////////////////
//	Created		: 03.09.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_SCENE_VIEW_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SCENE_VIEW_H_INCLUDED

#include <vostok/render/engine/base_classes.h>
#include <vostok/render/core/memory.h>
#include <vostok/render/core/resource_intrusive_base.h>
#include <vostok/scaleform/sources/flash_movie_resource.h>
#include <vostok/vectora.h>
#include "post_process_parameters.h"
#include <vostok/render/facade/scene_view_mode.h>
#include <vostok/render/facade/particles.h>
#include "camera.h"
#include "light.h"

namespace survarium {
	struct flash_text_manager;
}

namespace vostok {

namespace particle {
	enum enum_particle_render_mode;
	struct render_particle_emitter_instance;
}

namespace render {

class ambient_volume;
struct culling_result;
struct decal_instance;
class environment_probe;
struct grass_patch;
struct render_surface_instance;

typedef intrusive_ptr<
	light,
	resource_intrusive_base,
	threading::single_threading_policy
> light_ptr;

class scene_view: public base_scene_view {
public:
									scene_view				( );
	virtual							~scene_view				( );
	inline	void					camera_set_view			( float4x4 const& transform )	{ m_camera.set_view_transform( transform ); }
#ifndef MASTER_GOLD
	inline	void					camera_set_view_only	( float4x4 const& transform )	{ m_camera.set_view_transform_only( transform ); }
#endif // #ifndef MASTER_GOLD
	inline	void					camera_set_projection	( float4x4 const& transform )	{ m_camera.set_projection_transform( transform ); }
	inline	render::camera const&	camera					( ) const						{ return m_camera; }
	
	inline	render::post_process_parameters&		post_process_parameters	( )				{ return m_post_process_parameters; }
	inline	render::post_process_parameters const&  post_process_parameters	( ) const		{ return m_post_process_parameters; }
			
	void			set_view_mode	(scene_view_mode view_mode) { m_view_mode = view_mode; }
	scene_view_mode get_view_mode	() const { return m_view_mode; }
	
	vostok::math::float4 const& get_luminance_parameters	() const { return m_frame_luminance_parameters; }
	vostok::math::float4 const& get_prev_luminance_parameters	() const { return m_prev_frame_luminance_parameters; }
	
	void					  set_luminance_parameters	(vostok::math::float4 const& avrg_min_max_luminance) 
														{m_frame_luminance_parameters = avrg_min_max_luminance;}
	
	void					  set_prev_luminance_parameters	(vostok::math::float4 const& avrg_min_max_luminance) 
															{m_prev_frame_luminance_parameters = avrg_min_max_luminance;}
	
	u32					get_render_frame_index	() const { return m_render_frame_index; }

	void set_use_post_process		(bool use_post_process) { m_use_post_process = use_post_process; }
	bool is_use_post_process		() const { return m_use_post_process; }

	void										add_movie						( survarium::flash_movie_resource_ptr& movie );
	void										remove_movie					( survarium::flash_movie_resource_ptr& movie );
	render::vector< survarium::flash_movie_resource_ptr > const&	flash_movies		( ) const { return m_flash_movies; }

	void										add_text_manager				( survarium::flash_text_manager* tm );
	void										remove_text_manager				( survarium::flash_text_manager* tm );
	survarium::flash_text_manager*				flash_text_manager				( ) const { return m_flash_text_manager; }

	void										set_particles_render_mode	(vostok::particle::enum_particle_render_mode render_mode) { m_particles_render_mode = render_mode; }
	vostok::particle::enum_particle_render_mode   get_particles_render_mode	() const { return m_particles_render_mode; }

	render::vector< render_surface_instance* >&	get_visible_models				( ) { return m_visible_models; }
	render::vector< render_surface_instance* >&	get_visible_opaque_models		( ) { return m_visible_opaque_models; }
	render::vector< render_surface_instance* >&	get_visible_moved_opaque_models	( ) { return m_visible_moved_models; }
	render::vector< light_ptr >&					get_visible_lights				( ) { return m_visible_lights; }
	vectora< particle::render_particle_emitter_instance* >&	get_visible_particle_instances	( ) { return m_visible_particle_instances; }
	render::vector< decal_instance* >&			get_visible_decals				( ) { return m_visible_decals; }
	render::vector< environment_probe* >&		get_visible_environment_probes	( ) { return m_visible_environment_probes; }
	render::vector< grass_patch* >&				get_visible_grass_patches		( ) { return m_visible_grass_patches; }
	render::vector< ambient_volume* >&			get_visible_ambient_volumes		( ) { return m_visible_ambient_volumes; }

	void get_histogram_values( float* values )
	{
		memory::copy( values, sizeof( m_histogram_values ), m_histogram_values, sizeof( m_histogram_values ) );
	}

	void set_histogram_values( float* values )
	{
		memory::copy( m_histogram_values, sizeof( m_histogram_values ), values, sizeof( m_histogram_values ) );
	}

	render::post_process_parameters				m_post_process_parameters;
	u32											luminance_pass_index;
	bool										need_recalc_atmosphere;

private:
	friend class renderer;
	void inc_render_frame_index	() { m_render_frame_index++; }
	render::camera								m_camera;
	render::scene_view_mode						m_view_mode;
	vostok::particle::enum_particle_render_mode	m_particles_render_mode;
	bool										m_use_post_process;
	u32											m_render_frame_index;
	vostok::math::float4							m_frame_luminance_parameters;
	vostok::math::float4							m_prev_frame_luminance_parameters;
	float										m_histogram_values[16];
	render::vector< render_surface_instance* >	m_visible_moved_models;
	render::vector< render_surface_instance* >	m_visible_models;
	render::vector< render_surface_instance* >	m_visible_opaque_models;
	render::vector< light_ptr >					m_visible_lights;
	render::vector< decal_instance* >			m_visible_decals;
	render::vector< environment_probe* >		m_visible_environment_probes;
	vectora< particle::render_particle_emitter_instance* >	m_visible_particle_instances;
	render::vector< grass_patch* >				m_visible_grass_patches;
	render::vector< ambient_volume* >			m_visible_ambient_volumes;
	render::vector< survarium::flash_movie_resource_ptr >	m_flash_movies;
	survarium::flash_text_manager*				m_flash_text_manager;
	u32											m_num_frustum_culled_surfaces;
	u32											m_num_frustum_culled_lights;
	u32											m_num_frustum_culled_grass_patches;
	u32											m_num_frustum_culled_particle_instances;
	u32											m_num_frustum_culled_decals;
	u32											m_num_occlusion_culled_surfaces;
	u32											m_num_occlusion_culled_lights;
	u32											m_num_occlusion_culled_grass_patches;
	u32											m_num_occlusion_culled_particle_instances;
	u32											m_num_occlusion_culled_decals;
	pbyte										m_occlusion_culling_results;
	u32											m_num_occlusion_culling_results;
	culling_result*								m_culling_results;
	float4*										m_culling_bounds;
	u32											m_atmosphere_prev_targets_size;
}; // class scene_view

STATIC_SIZE_ASSERT( scene_view, 0x5E0 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SCENE_VIEW_H_INCLUDED
