#include "pch.h"

// Temporary /OPT:REF anchor for entries not yet retained by real game callers.
// Address-take only; retire each pin once its caller keeps it linked.

#include <vostok/render/facade/scene_renderer.h>
#include <vostok/render/facade/sources/debug_renderer.h>
#include <vostok/render/facade/environment_probe_properties.h>
#include <vostok/render/facade/sky_ambient_occlusion_properties.h>
#include <vostok/render/engine/world.h>
#include <vostok/render/engine/sources/trample_desc.h>

namespace vostok {
namespace render {

void anchor_render_facade( )
{
	static pcvoid volatile s_sink = 0;

	void ( scene_renderer::* const m_reload_shaders )( )										= &scene_renderer::reload_shaders;
	void ( scene_renderer::* const m_reload_modified_textures )( )								= &scene_renderer::reload_modified_textures;
	void ( scene_renderer::* const m_set_gamma_correction_factor )( float )						= &scene_renderer::set_gamma_correction_factor;
	bool ( scene_renderer::* const m_is_playing )( resources::unmanaged_resource_ptr const& )	= &scene_renderer::is_playing;
	void ( scene_renderer::* const m_add_vegetation_trample )( base_scene_ptr const&, trample_desc const& )
																								= &scene_renderer::add_vegetation_trample;
	void ( scene_renderer::* const m_build_lpv_geometry )( base_scene_ptr const& )				= &scene_renderer::build_lpv_geometry;
	void ( scene_renderer::* const m_set_model_visible )( render_model_instance_ptr const&, u32, u32 )
																								= &scene_renderer::set_model_visible;
	void ( scene_renderer::* const m_update_environment_probe )( base_scene_ptr const&, u32, environment_probe_properties const& )
																								= &scene_renderer::update_environment_probe;
	void ( scene_renderer::* const m_remove_environment_probe )( base_scene_ptr const&, u32 )	= &scene_renderer::remove_environment_probe;
	void ( scene_renderer::* const m_update_sky_ambient_occlusion )( base_scene_ptr const&, u32, sky_ambient_occlusion_properties const& )
																								= &scene_renderer::update_sky_ambient_occlusion;
	void ( scene_renderer::* const m_remove_sky_ambient_occlusion )( base_scene_ptr const&, u32 )
																								= &scene_renderer::remove_sky_ambient_occlusion;
	void ( scene_renderer::* const m_add_tracer )( base_scene_ptr const&, tracer_model_instance_ptr const&, float4x4 const& )
																								= &scene_renderer::add_tracer;
	void ( scene_renderer::* const m_update_tracer )( base_scene_ptr const&, tracer_model_instance_ptr const&, float4x4 const& )
																								= &scene_renderer::update_tracer;
	void ( scene_renderer::* const m_remove_tracer )( base_scene_ptr const&, tracer_model_instance_ptr const& )
																								= &scene_renderer::remove_tracer;
	void ( scene_renderer::* const m_set_portal_system )( base_scene_ptr const&, resources::unmanaged_resource_ptr const& )
																								= &scene_renderer::set_portal_system;
	void ( scene_renderer::* const m_set_grass )( resources::unmanaged_resource_ptr, base_scene_ptr const& )
																								= &scene_renderer::set_grass;
	void ( scene_renderer::* const m_reset_grass )( resources::unmanaged_resource_ptr, base_scene_ptr const& )
																								= &scene_renderer::reset_grass;
	void ( engine::world::* const m_clear_grass )( base_scene_ptr const& )
																								= &engine::world::clear_grass;
	void ( engine::world::* const m_populate_grass )( base_scene_ptr const& )
																								= &engine::world::populate_grass;
	void ( engine::world::* const m_add_grass_layer )( grass_layer_desc*, grass_layer_data*, base_scene_ptr const& )
																								= &engine::world::add_grass_layer;
	void ( engine::world::* const m_update_grass_layer )( grass_layer_desc*, grass_layer_data*, base_scene_ptr const&, bool )
																								= &engine::world::update_grass_layer;
	void ( engine::world::* const m_remove_grass_layer )( u8, base_scene_ptr const& )
																	= &engine::world::remove_grass_layer;
	void ( engine::world::* const m_resize_render_output_window )( base_output_window_ptr const&, u32, u32, bool )
																	= &engine::world::resize_render_output_window;
	void ( engine::world::* const m_toggle_render_stage )( enum_render_stage_type, bool )
																	= &engine::world::toggle_render_stage;
	void ( engine::world::* const m_set_slomo )( base_scene_ptr const&, float )
																								= &engine::world::set_slomo;
	void ( engine::world::* const m_test_action_portal_system )( base_scene_ptr const& )
																								= &engine::world::test_action_portal_system;
	void ( engine::world::* const m_set_view_mode )( base_scene_view_ptr, scene_view_mode )
																								= &engine::world::set_view_mode;
	void ( engine::world::* const m_set_particles_render_mode )( base_scene_view_ptr, particle::enum_particle_render_mode )
																								= &engine::world::set_particles_render_mode;
	void ( engine::world::* const m_enable_post_process )( base_scene_view_ptr, bool )
																								= &engine::world::enable_post_process;
	void ( debug::renderer::* const m_draw_ellipsoid )( base_scene_ptr const&, float4x4 const&, float3 const&, math::color const&, bool )
																								= &debug::renderer::draw_ellipsoid;

	s_sink	= *( pcvoid const* )&m_reload_shaders;
	s_sink	= *( pcvoid const* )&m_reload_modified_textures;
	s_sink	= *( pcvoid const* )&m_set_gamma_correction_factor;
	s_sink	= *( pcvoid const* )&m_is_playing;
	s_sink	= *( pcvoid const* )&m_add_vegetation_trample;
	s_sink	= *( pcvoid const* )&m_build_lpv_geometry;
	s_sink	= *( pcvoid const* )&m_set_model_visible;
	s_sink	= *( pcvoid const* )&m_update_environment_probe;
	s_sink	= *( pcvoid const* )&m_remove_environment_probe;
	s_sink	= *( pcvoid const* )&m_update_sky_ambient_occlusion;
	s_sink	= *( pcvoid const* )&m_remove_sky_ambient_occlusion;
	s_sink	= *( pcvoid const* )&m_add_tracer;
	s_sink	= *( pcvoid const* )&m_update_tracer;
	s_sink	= *( pcvoid const* )&m_remove_tracer;
	s_sink	= *( pcvoid const* )&m_set_portal_system;
	s_sink	= *( pcvoid const* )&m_set_grass;
	s_sink	= *( pcvoid const* )&m_reset_grass;
	s_sink	= *( pcvoid const* )&m_clear_grass;
	s_sink	= *( pcvoid const* )&m_populate_grass;
	s_sink	= *( pcvoid const* )&m_add_grass_layer;
	s_sink	= *( pcvoid const* )&m_update_grass_layer;
	s_sink	= *( pcvoid const* )&m_remove_grass_layer;
	s_sink	= *( pcvoid const* )&m_resize_render_output_window;
	s_sink	= *( pcvoid const* )&m_toggle_render_stage;
	s_sink	= *( pcvoid const* )&m_set_slomo;
	s_sink	= *( pcvoid const* )&m_test_action_portal_system;
	s_sink	= *( pcvoid const* )&m_set_view_mode;
	s_sink	= *( pcvoid const* )&m_set_particles_render_mode;
	s_sink	= *( pcvoid const* )&m_enable_post_process;
	s_sink	= *( pcvoid const* )&m_draw_ellipsoid;
}

} // namespace render
} // namespace vostok
