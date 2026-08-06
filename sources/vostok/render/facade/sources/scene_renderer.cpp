#include "pch.h"
#include <vostok/render/facade/light_props.h>
#include <vostok/render/facade/scene_renderer.h>
#include <vostok/render/engine/world.h>
#include <vostok/render/engine/sources/tracer_model_instance.h>
#include <vostok/render/engine/sources/trample_desc.h>
#include <vostok/render/facade/ambient_volume_properties.h>
#include <vostok/render/facade/decal_properties.h>
#include <vostok/render/facade/environment_probe_properties.h>
#include <vostok/render/facade/sky_ambient_occlusion_properties.h>
#include <vostok/render/facade/volume_fog_parameters.h>
#include <vostok/render/facade/one_way_render_channel.h>
#include <vostok/render/facade/sources/functor_command.h>
#include <vostok/render/facade/sources/functor_with_big_buffer_to_copy_command.h>
#include <vostok/render/facade/sources/update_skeleton_command.h>
#include <vostok/particle/world.h>
#include <vostok/math_color.h>

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
	if ( m_frustum_listener )
		*m_frustum_listener	= math::frustum( mul4x4(m_view, m_projection) );
}

void scene_renderer::set_view_matrix( base_scene_view_ptr const& scene_view, float4x4 const& view_and_culling_matrix )
{
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

void scene_renderer::add_vegetation_trample( base_scene_ptr const& scene, trample_desc const& desc )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< trample_desc > ) (
			boost::bind(
				&vostok::render::engine::world::add_vegetation_trample,
				&m_render_engine_world,
				scene,
				_1
			),
			desc
		)
	);
}

void scene_renderer::set_projection_matrix( base_scene_view_ptr const& scene_view, float4x4 const& projection )
{
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
	R_ASSERT	( scene );
	m_channel.owner_push_back		( VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) ( boost::bind( &vostok::render::engine::world::add_model, &m_render_engine_world, scene, v, _1, true), transform ) );
}

void scene_renderer::remove_model( base_scene_ptr const& scene, render_model_instance_ptr const& model )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back		( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &vostok::render::engine::world::remove_model, &m_render_engine_world, scene, model)));
}

void scene_renderer::update_model(
	base_scene_ptr const&				scene,
	render_model_instance_ptr const&	render_model,
	float4x4 const&						model_to_world
)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back		( VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) ( boost::bind( &vostok::render::engine::world::update_model, &m_render_engine_world, scene, render_model, _1), model_to_world));
}

void scene_renderer::update_skeleton( render_model_instance_ptr const& v, float4x4 const* matrices, u32 count )
{
	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, update_skeleton_command ) ( m_render_engine_world, v, matrices, count ) );
}

void scene_renderer::set_gamma_correction_factor( const float value )
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::set_gamma_correction_factor, &m_render_engine_world, value) ));
}

void scene_renderer::play_particle_system(
	base_scene_ptr const&		scene,
	const resources::unmanaged_resource_ptr	in_instance,
	float4x4 const&				transform
)
{
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
	R_ASSERT	( scene );
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< math::float4x4 > ) ( boost::bind( &engine::world::update_particle_system_instance, &m_render_engine_world, instance, scene, _1, visible, paused), transform ) );
}

bool scene_renderer::is_playing( resources::unmanaged_resource_ptr const& instance )
{
	return particle::is_playing( instance );
}

void scene_renderer::update_decal( base_scene_ptr const& scene, u32 id, decal_properties const& properties )
{
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

void scene_renderer::update_environment_probe(
	base_scene_ptr const&					scene,
	u32										id,
	environment_probe_properties const&		properties
)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< environment_probe_properties > ) (
			boost::bind(
				&vostok::render::engine::world::update_environment_probe,
				&m_render_engine_world,
				scene,
				id,
				_1
			),
			properties
		)
	);
}

void scene_renderer::remove_environment_probe( base_scene_ptr const& scene, u32 id )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::remove_environment_probe,
				&m_render_engine_world,
				scene,
				id
			)
		)
	);
}

void scene_renderer::update_sky_ambient_occlusion(
	base_scene_ptr const&		scene,
	u32							id,
	sky_ambient_occlusion_properties const&	properties
)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< sky_ambient_occlusion_properties > ) (
			boost::bind(
				&vostok::render::engine::world::update_sky_ambient_occlusion,
				&m_render_engine_world,
				scene,
				id,
				_1
			),
			properties
		)
	);
}

void scene_renderer::remove_sky_ambient_occlusion( base_scene_ptr const& scene, u32 id )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::remove_sky_ambient_occlusion,
				&m_render_engine_world,
				scene,
				id
			)
		)
	);
}

void scene_renderer::update_ambient_volume( base_scene_ptr const& scene, u32 id, ambient_volume_properties const& properties )
{
	R_ASSERT	( scene );
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
	R_ASSERT	( scene );
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
	R_ASSERT	( scene );
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
	R_ASSERT	( scene );
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

void scene_renderer::add_tracer(
	base_scene_ptr const&				scene,
	tracer_model_instance_ptr const&	instance,
	float4x4 const&						initialize_transform
)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) (
			boost::bind(
				&vostok::render::engine::world::add_tracer,
				&m_render_engine_world,
				scene,
				instance,
				_1
			),
			initialize_transform
		)
	);
}

void scene_renderer::update_tracer(
	base_scene_ptr const&				scene,
	tracer_model_instance_ptr const&	instance,
	float4x4 const&						new_transform
)
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_with_big_buffer_to_copy_command< float4x4 > ) (
			boost::bind(
				&vostok::render::engine::world::update_tracer,
				&m_render_engine_world,
				scene,
				instance,
				_1
			),
			new_transform
		)
	);
}

void scene_renderer::remove_tracer( base_scene_ptr const& scene, tracer_model_instance_ptr const& instance )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::remove_tracer,
				&m_render_engine_world,
				scene,
				instance
			)
		)
	);
}

void scene_renderer::set_portal_system( base_scene_ptr const& scene, resources::unmanaged_resource_ptr const& pss_ptr )
{
	R_ASSERT	( scene );
	m_channel.owner_push_back	(
		VOSTOK_NEW_IMPL( m_allocator, functor_command ) (
			boost::bind(
				&vostok::render::engine::world::set_portal_system,
				&m_render_engine_world,
				scene,
				pss_ptr
			)
		)
	);
}

void scene_renderer::add_light( base_scene_ptr const& scene, u32 id, light_props* props )
{
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
	R_ASSERT	( scene_view );
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::set_post_process, &m_render_engine_world, scene_view, post_process_resource) ));
}

void scene_renderer::reload_shaders( )
{
	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &vostok::render::engine::world::reload_shaders, &m_render_engine_world) ) );
}

void scene_renderer::reload_modified_textures( )
{
	m_channel.owner_push_back	( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &vostok::render::engine::world::reload_modified_textures, &m_render_engine_world) ) );
}

void scene_renderer::set_sky_material( base_scene_ptr const& scene, resources::unmanaged_resource_ptr const& mtl_ptr )
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::set_sky_material, &m_render_engine_world, scene, mtl_ptr) ));
}

// claude@MATCH: the target binds engine::world::set_model_visible_by_id here -
// world::set_model_visible takes the virtual_path_string overload, this facade
// entry takes the resolved (surface_id, flags) pair.
void scene_renderer::set_model_visible( render_model_instance_ptr const& v, u32 surface_id, u32 flags )
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::set_model_visible_by_id, &m_render_engine_world, v, surface_id, flags) ));
}

void scene_renderer::build_lpv_geometry( base_scene_ptr const& scene )
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::build_lpv_geometry, &m_render_engine_world, scene) ));
}

void scene_renderer::set_grass( resources::unmanaged_resource_ptr grass, base_scene_ptr const& scene )
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::set_grass, &m_render_engine_world, grass, scene) ));
}

void scene_renderer::reset_grass( resources::unmanaged_resource_ptr grass, base_scene_ptr const& scene )
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::reset_grass, &m_render_engine_world, grass, scene) ));
}

particle::world& scene_renderer::particle_world( base_scene_ptr const& scene )
{
	R_ASSERT	( scene );
	return 		m_render_engine_world.particle_world( scene );
}

void scene_renderer::begin_render_options_changing( long volatile* waiting_for )
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::begin_render_options_changing, &m_render_engine_world, waiting_for) ));
}

void scene_renderer::end_render_options_changing(
	base_scene_ptr const&		scene,
	base_output_window_ptr		output_window,
	bool						reload_all_materials,
	bool						shaders_recompile,
	long volatile*				waiting_for
)
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::end_render_options_changing, &m_render_engine_world, scene, output_window, reload_all_materials, shaders_recompile, waiting_for) ));
}

// claude@MATCH: the target passes the literal true for world::reset_renderer's
// async_effects argument (mov byte ptr [esp+24h], 1 at the bind site).
void scene_renderer::reset_renderer( )
{
	m_channel.owner_push_back  ( VOSTOK_NEW_IMPL( m_allocator, functor_command ) ( boost::bind( &engine::world::reset_renderer, &m_render_engine_world, true) ));
}

light_props::light_props( ) :
	attenuation_power			( 0.f ),
	intensity					( 0.f ),
	range						( 0.f ),
	spot_umbra_angle			( 0.f ),
	spot_penumbra_angle			( 0.f ),
	spot_falloff				( 0.f ),
	diffuse_influence_factor	( 0.f ),
	specular_influence_factor	( 0.f ),
	color						( 0 ),
	type						( light_type_point ),
	lighting_model				( 1 ),
	sun_shadow_map_size			( 0 ),
	num_sun_cascades			( 0 ),
	static_shadows				( false ),
	shadow_map_size				( 0 ),
	shadow_map_size_index		( 0 ),
	local_light_z_bias			( 0.f ),
	shadow_transparency			( 0.f ),
	enabled						( false ),
	does_cast_shadows			( false ),
	shadower					( false ),
	m_year						( 0 ),
	m_month						( 0 ),
	m_day						( 0 ),
	m_hours						( 0 ),
	m_minutes					( 0 ),
	m_use_auto_location			( false ),
	use_with_lpv				( false ),
	is_light_animated			( false ),
	light_animation_length		( 0.f )
{
	shadow_distribution_sides[0]	= false;
	shadow_distribution_sides[1]	= false;
	shadow_distribution_sides[2]	= false;
	shadow_distribution_sides[3]	= false;
	shadow_distribution_sides[4]	= false;
	shadow_distribution_sides[5]	= false;
}

template < typename config_value_type >
void load_props_impl( light_props& props, config_value_type const& cfg )
{
	float3 clr										= cfg["color"];

	if ( cfg.value_exists( "is_cast_shadow" ) )			props.does_cast_shadows				= cfg["is_cast_shadow"];
	if ( cfg.value_exists( "sun_shadow_map_size" ) )	props.sun_shadow_map_size			= cfg["sun_shadow_map_size"];
	if ( cfg.value_exists( "shadow_map_size_index" ) )	props.shadow_map_size_index			= cfg["shadow_map_size_index"];
	if ( cfg.value_exists( "num_sun_cascades" ) )		props.num_sun_cascades				= cfg["num_sun_cascades"];
	if ( cfg.value_exists( "is_cast_shadow_x" ) )		props.shadow_distribution_sides[0]	= cfg["is_cast_shadow_x"];
	if ( cfg.value_exists( "is_cast_shadow_neg_x" ) )	props.shadow_distribution_sides[1]	= cfg["is_cast_shadow_neg_x"];
	if ( cfg.value_exists( "is_cast_shadow_y" ) )		props.shadow_distribution_sides[2]	= cfg["is_cast_shadow_y"];
	if ( cfg.value_exists( "is_cast_shadow_neg_y" ) )	props.shadow_distribution_sides[3]	= cfg["is_cast_shadow_neg_y"];
	if ( cfg.value_exists( "is_cast_shadow_z" ) )		props.shadow_distribution_sides[4]	= cfg["is_cast_shadow_z"];
	if ( cfg.value_exists( "is_cast_shadow_neg_z" ) )	props.shadow_distribution_sides[5]	= cfg["is_cast_shadow_neg_z"];
	if ( cfg.value_exists( "z_bias" ) )					props.local_light_z_bias			= (float)cfg["z_bias"];
	if ( cfg.value_exists( "shadow_transparency" ) )	props.shadow_transparency			= (float)cfg["shadow_transparency"];

	if ( cfg.value_exists( "lighting_model" ) )
		props.lighting_model						= cfg["lighting_model"];
	else
		props.lighting_model						= 1;

	props.range										= (float)cfg["range"];
	props.color										= math::color_rgba( clr.x, clr.y, clr.z, 1.f );

	if ( cfg.value_exists( "scale" ) && cfg.value_exists( "rotation" ) && cfg.value_exists( "position" ) )
	{
		float3 const& scale							= cfg["scale"];
		float3 const& rotation						= cfg["rotation"];
		// claude@MATCH: position binds the config value, not a float3 like its two
		// neighbours - the target's statement is bare push/call with no deref, the
		// float3 conversion happens later inside create_translation's argument.
		config_value_type const& position			= cfg["position"];
		props.transform								= math::mul4x3( math::mul4x3( math::create_scale( scale ), math::create_rotation( rotation ) ), math::create_translation( position ) );
	}
	else
		props.transform								= math::float4x4( ).identity( );

	props.type										= (light_type)(u32)cfg["light_type"];

	if ( cfg.value_exists( "attenuation_power" ) )
		props.attenuation_power						= (float)cfg["attenuation_power"];
	else
		props.attenuation_power						= float3( cfg["attenuation"] ).x;

	props.intensity									= (float)cfg["intensity"];
	props.spot_umbra_angle							= (float)cfg["spot_umbra_angle"];
	props.spot_penumbra_angle						= (float)cfg["spot_penumbra_angle"];
	props.spot_falloff								= (float)cfg["spot_falloff"];

	props.diffuse_influence_factor					= cfg.value_exists( "diffuse_influence_factor" ) ? (float)cfg["diffuse_influence_factor"] : 1.f;
	props.specular_influence_factor					= cfg.value_exists( "specular_influence_factor" ) ? (float)cfg["specular_influence_factor"] : 1.f;

	props.shadower									= cfg.value_exists( "is_shadower" ) ? (bool)cfg["is_shadower"] : false;
	props.use_with_lpv								= cfg.value_exists( "use_with_lpv" ) ? (bool)cfg["use_with_lpv"] : false;

	props.enabled									= cfg.value_exists( "enabled" ) ? (bool)cfg["enabled"] : true;

	props.static_shadows							= cfg.value_exists( "static_shadows" ) ? (bool)cfg["static_shadows"] : false;

	if ( cfg.value_exists( "is_light_animated" ) )
	{
		props.is_light_animated						= cfg["is_light_animated"];

		if ( props.is_light_animated )
		{
			props.m_color_curve.load				( cfg["color_curve"] );
			props.light_animation_length			= (float)cfg["light_animation_length"];
		}
	}
	else
		props.is_light_animated						= false;
}

void light_props::load_light_props( configs::binary_config_value const& cfg )
{
	load_props_impl	( *this, cfg );
}

} // namespace render
} // namespace vostok
