#include "pch.h"
#include <vostok/render/facade/light_props.h>
#include <vostok/render/facade/sources/scene_renderer.h>

namespace vostok {
namespace render {

// STATE[STUB]
scene_renderer::scene_renderer(
	one_way_render_channel&		channel,
	memory::base_allocator&		allocator,
	engine::world&				render_engine_world,
	math::frustum*				frustum_listener
)
	: m_render_engine_world( render_engine_world ),
	  m_channel( channel ),
	  m_allocator( allocator ),
	  m_frustum_listener( frustum_listener )
{
	// FUNCTION BODY[0x6e0020]: 2
	// <0x6e0093>|0x073|+0x006:'47'
	// <0x6e0099>|0x079|+0x026:'48'
	// ******
}

// STATE[STUB]
void scene_renderer::set_view_matrix( base_scene_view_ptr const& scene_view, float4x4 const& view_and_culling_matrix )
{
	// CALL SITE INFO
	// <0x6e1887> -> < unknown >
	// ******

	// FUNCTION BODY[0x6e1720]: 16
	// <0>
	// <1>
	// <0x6e172f>|0x00f|+0x023:'55'
	// <0x6e1752>|0x032|+0x025:'56'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x6e1777>|0x057|+0x115:'68'
	// ******
}

// STATE[STUB]
void scene_renderer::add_vegetation_trample( base_scene_ptr const& scene, trample_desc const& desc )
{
	// CALL SITE INFO
	// <0x6e0800> -> < unknown >
	// ******

	// FUNCTION BODY[0x6e06f0]: 12
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x6e06f6>|0x006|+0x10f:'84'
	// ******
}

// STATE[STUB]
void scene_renderer::set_projection_matrix( base_scene_view_ptr const& scene_view, float4x4 const& projection )
{
	// CALL SITE INFO
	// <0x6e1709> -> < unknown >
	// ******

	// FUNCTION BODY[0x6e15a0]: 15
	// <0>
	// <1>
	// <0x6e15af>|0x00f|+0x023:'123'
	// <0x6e15d2>|0x032|+0x027:'124'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x6e15f9>|0x059|+0x115:'135'
	// ******
}

// STATE[STUB]
void scene_renderer::add_model(
	base_scene_ptr const&				scene,
	render_model_instance_ptr const&	v,
	float4x4 const&						transform
)
{
	// CALL SITE INFO
	// <0x6e1592> -> < unknown >
	// ******

	// FUNCTION BODY[0x6e1460]: 2
	// <0>
	// <0x6e146e>|0x00e|+0x129:'141'
	// ******
}

// STATE[STUB]
void scene_renderer::remove_model( base_scene_ptr const& scene, render_model_instance_ptr const& model )
{
	// FUNCTION BODY[0x6e26c0]: 2
	// <0>
	// <0x6e26cb>|0x00b|+0x14a:'147'
	// ******
}

// STATE[STUB]
void scene_renderer::update_model(
	base_scene_ptr const&				scene,
	render_model_instance_ptr const&	render_model,
	float4x4 const&						model_to_world
)
{
	// CALL SITE INFO
	// <0x6e26b1> -> < unknown >
	// ******

	// FUNCTION BODY[0x6e2580]: 2
	// <0>
	// <0x6e2589>|0x009|+0x12d:'153'
	// ******
}

// STATE[STUB]
void scene_renderer::update_skeleton( render_model_instance_ptr const& v, float4x4 const* matrices, u32 count )
{
	// FUNCTION BODY[0x6e0200]: 1
	// <0x6e0204>|0x004|+0x07d:'158'
	// ******
}

// STATE[STUB]
void scene_renderer::set_gamma_correction_factor( const float value )
{
	// FUNCTION BODY[0x6dfee0]: 1
	// <0x6dfeec>|0x00c|+0x11d:'163'
	// ******
}

// STATE[STUB]
void scene_renderer::play_particle_system(
	base_scene_ptr const&		scene,
	const resources::unmanaged_resource_ptr	arg_1 /* resources::unmanaged_resource_ptr in_instance */,
	float4x4 const&				transform
)
{
	// CALL SITE INFO
	// <0x6e142b> -> < unknown >
	// ******

	// FUNCTION BODY[0x6e1300]: 15
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <0x6e130e>|0x00e|+0x122:'182'
	// ******
}

// STATE[STUB]
void scene_renderer::remove_particle_system_instance(
	base_scene_ptr const&		scene,
	resources::unmanaged_resource_ptr const&	in_instance
)
{
	// FUNCTION BODY[0x6e0590]: 2
	// <0>
	// <0x6e059b>|0x00b|+0x13f:'194'
	// ******
}

// STATE[STUB]
void scene_renderer::update_particle_system_instance(
	base_scene_ptr const&		scene,
	resources::unmanaged_resource_ptr const&	instance,
	float4x4 const&				transform,
	bool						visible,
	bool						paused
)
{
	// CALL SITE INFO
	// <0x6e12e7> -> < unknown >
	// ******

	// FUNCTION BODY[0x6e11c0]: 2
	// <0>
	// <0x6e11c9>|0x009|+0x123:'200'
	// ******
}

// STATE[STUB]
bool scene_renderer::is_playing( resources::unmanaged_resource_ptr const& instance )
{
	return false;

	// FUNCTION BODY[0x6dfc90]: 1
	// <0x6dfc90>|0x000|+0x009:'205'
	// ******
}

// STATE[STUB]
void scene_renderer::update_decal( base_scene_ptr const& scene, u32 id, decal_properties const& properties )
{
	// CALL SITE INFO
	// <0x6e11aa> -> < unknown >
	// ******

	// FUNCTION BODY[0x6e1090]: 13
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x6e109b>|0x00b|+0x114:'244'
	// ******
}

// STATE[STUB]
void scene_renderer::remove_decal( base_scene_ptr const& scene, u32 id )
{
	// FUNCTION BODY[0x6e2440]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x6e244b>|0x00b|+0x128:'259'
	// ******
}

// STATE[STUB]
void scene_renderer::update_environment_probe(
	base_scene_ptr const&					scene,
	u32										id,
	environment_probe_properties const&		properties
)
{
	// CALL SITE INFO
	// <0x6e2cc0> -> < unknown >
	// ******

	// FUNCTION BODY[0x6e2bb0]: 13
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x6e2bb9>|0x009|+0x10c:'276'
	// ******
}

// STATE[STUB]
void scene_renderer::remove_environment_probe( base_scene_ptr const& scene, u32 id )
{
	// FUNCTION BODY[0x6e2300]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x6e230b>|0x00b|+0x128:'291'
	// ******
}

// STATE[STUB]
void scene_renderer::update_sky_ambient_occlusion(
	base_scene_ptr const&		scene,
	u32							id,
	sky_ambient_occlusion_properties const&	properties
)
{
	// CALL SITE INFO
	// <0x6e2b9a> -> < unknown >
	// ******

	// FUNCTION BODY[0x6e2a80]: 13
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x6e2a8b>|0x00b|+0x114:'308'
	// ******
}

// STATE[STUB]
void scene_renderer::remove_sky_ambient_occlusion( base_scene_ptr const& scene, u32 id )
{
	// FUNCTION BODY[0x6e21c0]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x6e21cb>|0x00b|+0x128:'323'
	// ******
}

// STATE[STUB]
void scene_renderer::update_ambient_volume( base_scene_ptr const& scene, u32 id, ambient_volume_properties const& properties )
{
	// CALL SITE INFO
	// <0x6e1080> -> < unknown >
	// ******

	// FUNCTION BODY[0x6e0f70]: 13
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x6e0f79>|0x009|+0x10c:'341'
	// ******
}

// STATE[STUB]
void scene_renderer::remove_ambient_volume( base_scene_ptr const& scene, u32 id )
{
	// FUNCTION BODY[0x6e2080]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x6e208b>|0x00b|+0x128:'356'
	// ******
}

// STATE[STUB]
void scene_renderer::update_lpv_occluder( base_scene_ptr const& scene, u32 id, float4x4 const& transform )
{
	// CALL SITE INFO
	// <0x6e0f5a> -> < unknown >
	// ******

	// FUNCTION BODY[0x6e0e40]: 13
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x6e0e4b>|0x00b|+0x114:'373'
	// ******
}

// STATE[STUB]
void scene_renderer::remove_lpv_occluder( base_scene_ptr const& scene, u32 id )
{
	// FUNCTION BODY[0x6e1f40]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x6e1f4b>|0x00b|+0x128:'388'
	// ******
}

// STATE[STUB]
void scene_renderer::update_volume_fog( base_scene_ptr const& scene, u32 id, volume_fog_parameters const& in_parameters )
{
	// CALL SITE INFO
	// <0x6e0e30> -> < unknown >
	// ******

	// FUNCTION BODY[0x6e0d20]: 13
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x6e0d29>|0x009|+0x10c:'422'
	// ******
}

// STATE[STUB]
void scene_renderer::remove_volume_fog( base_scene_ptr const& scene, u32 id )
{
	// FUNCTION BODY[0x6e1e00]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x6e1e0b>|0x00b|+0x128:'437'
	// ******
}

// STATE[STUB]
void scene_renderer::add_tracer(
	base_scene_ptr const&				scene,
	tracer_model_instance_ptr const&	instance,
	float4x4 const&						initialize_transform
)
{
	// CALL SITE INFO
	// <0x6e0d06> -> < unknown >
	// ******

	// FUNCTION BODY[0x6e0bd0]: 13
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x6e0bd9>|0x009|+0x132:'454'
	// ******
}

// STATE[STUB]
void scene_renderer::update_tracer(
	base_scene_ptr const&				scene,
	tracer_model_instance_ptr const&	instance,
	float4x4 const&						new_transform
)
{
	// CALL SITE INFO
	// <0x6e0bb6> -> < unknown >
	// ******

	// FUNCTION BODY[0x6e0a80]: 13
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <0x6e0a89>|0x009|+0x132:'471'
	// ******
}

// STATE[STUB]
void scene_renderer::remove_tracer( base_scene_ptr const& scene, tracer_model_instance_ptr const& instance )
{
	// FUNCTION BODY[0x6e1ca0]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x6e1ca7>|0x007|+0x142:'486'
	// ******
}

// STATE[STUB]
void scene_renderer::set_portal_system( base_scene_ptr const& scene, resources::unmanaged_resource_ptr const& pss_ptr )
{
	// FUNCTION BODY[0x6e1b40]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x6e1b4b>|0x00b|+0x13d:'623'
	// ******
}

// STATE[STUB]
void scene_renderer::add_light( base_scene_ptr const& scene, u32 id, light_props* props )
{
	// FUNCTION BODY[0x6e2f40]: 12
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x6e2f4b>|0x00b|+0x133:'654'
	// ******
}

// STATE[STUB]
void scene_renderer::update_light( base_scene_ptr const& scene, u32 id, light_props* props )
{
	// FUNCTION BODY[0x6e2e00]: 12
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <0x6e2e09>|0x009|+0x122:'670'
	// ******
}

// STATE[STUB]
void scene_renderer::remove_light( base_scene_ptr const& scene, u32 id )
{
	// FUNCTION BODY[0x6e1a00]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <0x6e1a0b>|0x00b|+0x128:'685'
	// ******
}

// STATE[STUB]
void scene_renderer::set_post_process(
	base_scene_view_ptr const&		scene_view,
	resources::unmanaged_resource_ptr const&	post_process_resource
)
{
	// FUNCTION BODY[0x6e2930]: 2
	// <0>
	// <0x6e293b>|0x00b|+0x138:'697'
	// ******
}

// STATE[STUB]
void scene_renderer::reload_shaders( )
{
	// FUNCTION BODY[0x6dfdd0]: 1
	// <0x6dfddf>|0x00f|+0x0f7:'702'
	// ******
}

// STATE[STUB]
void scene_renderer::reload_modified_textures( )
{
	// FUNCTION BODY[0x6e2820]: 1
	// <0x6e282f>|0x00f|+0x0f7:'707'
	// ******
}

// STATE[STUB]
void scene_renderer::set_sky_material( base_scene_ptr const& scene, resources::unmanaged_resource_ptr const& mtl_ptr )
{
	// FUNCTION BODY[0x6e18a0]: 1
	// <0x6e18ab>|0x00b|+0x13d:'720'
	// ******
}

// STATE[STUB]
void scene_renderer::set_model_visible( render_model_instance_ptr const& v, u32 surface_id, u32 flags )
{
	// FUNCTION BODY[0x6e0950]: 1
	// <0x6e0959>|0x009|+0x11d:'744'
	// ******
}

// STATE[STUB]
void scene_renderer::build_lpv_geometry( base_scene_ptr const& scene )
{
	// FUNCTION BODY[0x6e0810]: 1
	// <0x6e081b>|0x00b|+0x123:'749'
	// ******
}

// STATE[STUB]
void scene_renderer::set_grass( resources::unmanaged_resource_ptr grass, base_scene_ptr const& scene )
{
	// FUNCTION BODY[0x6e0410]: 1
	// <0x6e041c>|0x00c|+0x140:'811'
	// ******
}

// STATE[STUB]
void scene_renderer::reset_grass( resources::unmanaged_resource_ptr grass, base_scene_ptr const& scene )
{
	// FUNCTION BODY[0x6e0290]: 1
	// <0x6e029c>|0x00c|+0x140:'816'
	// ******
}

// STATE[STUB]
particle::world& scene_renderer::particle_world( base_scene_ptr const& scene )
{
	// FUNCTION BODY[0x6e01c0]: 2
	// <0>
	// <0x6e01c1>|0x001|+0x038:'833'
	// ******
	return *(particle::world*)0;
}

// STATE[STUB]
void scene_renderer::begin_render_options_changing( long volatile* waiting_for )
{
	// FUNCTION BODY[0x6dfca0]: 1
	// <0x6dfca8>|0x008|+0x110:'863'
	// ******
}

// STATE[STUB]
void scene_renderer::end_render_options_changing(
	base_scene_ptr const&		scene,
	base_output_window_ptr		output_window,
	bool						reload_all_materials,
	bool						shaders_recompile,
	long volatile*				waiting_for
)
{
	// FUNCTION BODY[0x6e3090]: 1
	// <0x6e3097>|0x007|+0x161:'868'
	// ******
}

// STATE[STUB]
void scene_renderer::reset_renderer( )
{
	// FUNCTION BODY[0x6e2cd0]: 1
	// <0x6e2cd8>|0x008|+0x115:'873'
	// ******
}

// STATE[STUB]
light_props::light_props( )
{
	// FUNCTION BODY[0x6e00d0]: 6
	// <0x6e018b>|0x0bb|+0x006:'923'
	// <0x6e0191>|0x0c1|+0x006:'924'
	// <0x6e0197>|0x0c7|+0x006:'925'
	// <0x6e019d>|0x0cd|+0x006:'926'
	// <0x6e01a3>|0x0d3|+0x006:'927'
	// <0x6e01a9>|0x0d9|+0x006:'928'
	// ******
}

// STATE[STUB]
void light_props::load_light_props( configs::binary_config_value const& cfg )
{
	// FUNCTION BODY[0x6e01b0]: 2
	// <0>
	// <0x6e01b1>|0x001|+0x00b:'1014'
	// ******
}

} // namespace render
} // namespace vostok
