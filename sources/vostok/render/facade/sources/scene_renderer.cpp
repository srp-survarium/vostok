#include "pch.h"
#include <vostok/render/facade/light_props.h>
#include <vostok/render/facade/sources/scene_renderer.h>
#include <vostok/render/engine/world.h>
#include <vostok/render/facade/ambient_volume_properties.h>
#include <vostok/render/facade/decal_properties.h>
#include <vostok/render/facade/volume_fog_parameters.h>
#include <vostok/render/facade/one_way_render_channel.h>
#include <vostok/render/facade/sources/functor_command.h>
#include <vostok/render/facade/sources/functor_with_big_buffer_to_copy_command.h>
#include <vostok/render/facade/sources/update_skeleton_command.h>

namespace vostok {
namespace render {

scene_renderer::scene_renderer(
	one_way_render_channel&		channel,
	memory::base_allocator&		allocator,
	engine::world&				render_engine_world,
	math::frustum*				frustum_listener
)
	: m_render_engine_world( render_engine_world ),
	  m_channel( channel ),
	  m_allocator( allocator ),
	  m_frustum_listener( frustum_listener ),
	  m_view( math::float4x4().identity() ),
	  m_projection( math::create_perspective_projection( math::pi_d2, 4.f/3.f, .1f, 1000.f ) )
{
	// FUNCTION BODY[0x6e0020]: 2
	// <0x6e0093>|0x073|+0x006:'47'
	// <0x6e0099>|0x079|+0x026:'48'
	// ******

	if ( m_frustum_listener )
		*m_frustum_listener	= math::frustum( mul4x4(m_view, m_projection) );
}

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

	R_ASSERT				( scene_view );
	m_view					= view_and_culling_matrix;
	if ( m_frustum_listener )
		*m_frustum_listener	= math::frustum( mul4x4( m_view, m_projection ) );

	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) (
			boost::bind(
				&vostok::render::engine::world::set_view_matrix,
				&m_render_engine_world,
				scene_view,
				_1
			),
			view_and_culling_matrix
		)
	);
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

	R_ASSERT	( scene_view );
	m_projection = projection;
	if ( m_frustum_listener )
		*m_frustum_listener	= math::frustum( mul4x4( m_view, m_projection ) );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) (
			boost::bind(
				&vostok::render::engine::world::set_projection_matrix,
				&m_render_engine_world,
				scene_view,
				_1
			),
			projection
		)
	);
}

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

	R_ASSERT	( scene );
	m_channel.owner_push_back		( VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) ( boost::bind( &vostok::render::engine::world::add_model, &m_render_engine_world, scene, v, _1, true), transform ) );
}

void scene_renderer::remove_model( base_scene_ptr const& scene, render_model_instance_ptr const& model )
{
	// FUNCTION BODY[0x6e26c0]: 2
	// <0>
	// <0x6e26cb>|0x00b|+0x14a:'147'
	// ******

	R_ASSERT	( scene );
	m_channel.owner_push_back		( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &vostok::render::engine::world::remove_model, &m_render_engine_world, scene, model)));
}

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

	R_ASSERT	( scene );
	m_channel.owner_push_back		( VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) ( boost::bind( &vostok::render::engine::world::update_model, &m_render_engine_world, scene, render_model, _1), model_to_world));
}

void scene_renderer::update_skeleton( render_model_instance_ptr const& v, float4x4 const* matrices, u32 count )
{
	// FUNCTION BODY[0x6e0200]: 1
	// <0x6e0204>|0x004|+0x07d:'158'
	// ******

	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, update_skeleton_command ) ( m_render_engine_world, v, matrices, count ) );
}

// STATE[STUB]
void scene_renderer::set_gamma_correction_factor( const float value )
{
	// FUNCTION BODY[0x6dfee0]: 1
	// <0x6dfeec>|0x00c|+0x11d:'163'
	// ******
}

void scene_renderer::play_particle_system(
	base_scene_ptr const&		scene,
	const resources::unmanaged_resource_ptr	in_instance,
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

	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) (
			boost::bind(
				&vostok::render::engine::world::play_particle_system,
				&m_render_engine_world,
				scene,
				in_instance,
				true,
				false,
				_1
			),
			transform
		)
	);
}

void scene_renderer::remove_particle_system_instance(
	base_scene_ptr const&		scene,
	resources::unmanaged_resource_ptr const&	in_instance
)
{
	// FUNCTION BODY[0x6e0590]: 2
	// <0>
	// <0x6e059b>|0x00b|+0x13f:'194'
	// ******

	R_ASSERT	( scene );
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::remove_particle_system_instance, &m_render_engine_world, in_instance, scene) ));
}

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

	R_ASSERT	( scene );
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< math::float4x4 > ) ( boost::bind( &engine::world::update_particle_system_instance, &m_render_engine_world, instance, scene, _1, visible, paused), transform ) );
}

// STATE[STUB]
bool scene_renderer::is_playing( resources::unmanaged_resource_ptr const& instance )
{
	return false;

	// FUNCTION BODY[0x6dfc90]: 1
	// <0x6dfc90>|0x000|+0x009:'205'
	// ******
}

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

	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< decal_properties > ) (
			boost::bind(
				&vostok::render::engine::world::update_decal,
				&m_render_engine_world,
				scene,
				id,
				_1
			),
			properties
		)
	);
}

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

	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::remove_decal,
				&m_render_engine_world,
				scene,
				id
			)
		)
	);
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

	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< ambient_volume_properties > ) (
			boost::bind(
				&vostok::render::engine::world::update_ambient_volume,
				&m_render_engine_world,
				scene,
				id,
				_1
			),
			properties
		)
	);
}

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

	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::remove_ambient_volume,
				&m_render_engine_world,
				scene,
				id
			)
		)
	);
}

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

	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) (
			boost::bind(
				&vostok::render::engine::world::update_lpv_occluder,
				&m_render_engine_world,
				scene,
				id,
				_1
			),
			transform
		)
	);
}

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

	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::remove_lpv_occluder,
				&m_render_engine_world,
				scene,
				id
			)
		)
	);
}

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

	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< volume_fog_parameters > ) (
			boost::bind(
				&vostok::render::engine::world::update_volume_fog,
				&m_render_engine_world,
				scene,
				id,
				_1
			),
			in_parameters
		)
	);
}

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

	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::remove_volume_fog,
				&m_render_engine_world,
				scene,
				id
			)
		)
	);
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

	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::add_light,
				&m_render_engine_world,
				scene,
				id,
				props
			)
		)
	);
}

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

	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::update_light,
				&m_render_engine_world,
				scene,
				id,
				props
			)
		)
	);
}

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

	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::remove_light,
				&m_render_engine_world,
				scene,
				id
			)
		)
	);
}

void scene_renderer::set_post_process(
	base_scene_view_ptr const&		scene_view,
	resources::unmanaged_resource_ptr const&	post_process_resource
)
{
	// FUNCTION BODY[0x6e2930]: 2
	// <0>
	// <0x6e293b>|0x00b|+0x138:'697'
	// ******

	R_ASSERT	( scene_view );
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::set_post_process, &m_render_engine_world, scene_view, post_process_resource) ));
}

void scene_renderer::reload_shaders( )
{
	// FUNCTION BODY[0x6dfdd0]: 1
	// <0x6dfddf>|0x00f|+0x0f7:'702'
	// ******

	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &vostok::render::engine::world::reload_shaders, &m_render_engine_world) ) );
}

void scene_renderer::reload_modified_textures( )
{
	// FUNCTION BODY[0x6e2820]: 1
	// <0x6e282f>|0x00f|+0x0f7:'707'
	// ******

	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &vostok::render::engine::world::reload_modified_textures, &m_render_engine_world) ) );
}

void scene_renderer::set_sky_material( base_scene_ptr const& scene, resources::unmanaged_resource_ptr const& mtl_ptr )
{
	// FUNCTION BODY[0x6e18a0]: 1
	// <0x6e18ab>|0x00b|+0x13d:'720'
	// ******

	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::set_sky_material, &m_render_engine_world, scene, mtl_ptr) ));
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

particle::world& scene_renderer::particle_world( base_scene_ptr const& scene )
{
	// FUNCTION BODY[0x6e01c0]: 2
	// <0>
	// <0x6e01c1>|0x001|+0x038:'833'
	// ******

	R_ASSERT	( scene );
	return 		m_render_engine_world.particle_world( scene );
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
