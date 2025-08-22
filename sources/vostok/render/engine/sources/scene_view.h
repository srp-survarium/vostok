////////////////////////////////////////////////////////////////////////////
//	Created		: 03.09.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_SCENE_VIEW_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_SCENE_VIEW_H_INCLUDED

#include <vostok/render/engine/base_classes.h>
#include "post_process_parameters.h"
#include <vostok/render/facade/scene_view_mode.h>
#include <vostok/render/facade/particles.h>
#include "camera.h"

namespace vostok {

namespace particle {
	enum enum_particle_render_mode;
}

namespace render {

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
	
	// TODO: Make per emitter render mode. Remove from scene_view?
	void										set_particles_render_mode	(vostok::particle::enum_particle_render_mode render_mode) { m_particles_render_mode = render_mode; }
	vostok::particle::enum_particle_render_mode   get_particles_render_mode	() const { return m_particles_render_mode; }
	
	render::post_process_parameters				m_post_process_parameters;
	
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
}; // class scene_view


} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_SCENE_VIEW_H_INCLUDED
