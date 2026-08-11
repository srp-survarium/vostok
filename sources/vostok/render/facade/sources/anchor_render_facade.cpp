#include "pch.h"

// Render-facade /OPT:REF reachability anchor for the scene_renderer entries the
// base build's call graph does not reach yet. The shipped game drives them from
// object_light / object_vegetation / the bullet manager / the options tab; those
// game consumers are still carcasses here, so /OPT:REF strips every method whose
// only reference would come from them - and stripping the facade method strips
// its whole engine::world -> scene cone with it.
//
// Every pin is an ADDRESS-TAKE, never a call: taking the address keeps the
// out-of-line body (so its bound &engine::world::* references survive) without
// handing LTCG a call site whose fabricated constant arguments would specialise
// the body away from the target's bytes.
//
// Dispatched from survarium::IncludeAll::IncludeAll() (game_core/sources/anchor.cpp)
// like every other module anchor - a self-dispatching file-scope initializer does
// not work here, because render_facade is a static library and the linker never
// extracts an .obj that resolves no undefined symbol.
//
// Retire pin by pin as each real game consumer is matched: a pin whose removal
// leaves the method paired is redundant and must go.

#include <vostok/render/facade/scene_renderer.h>
#include <vostok/render/facade/sources/debug_renderer.h>
#include <vostok/render/facade/environment_probe_properties.h>
#include <vostok/render/facade/sky_ambient_occlusion_properties.h>
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
	s_sink	= *( pcvoid const* )&m_draw_ellipsoid;
}

} // namespace render
} // namespace vostok
