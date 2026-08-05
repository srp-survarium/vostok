#include "pch.h"
// claude@NOTE: legacy-harvest disposition: no temp/render_legacy ancestor (the visibility stage and its occlusion-bound gathering are new-in-target) - matcher-phase work.
#include "stage_visibility.h"

#include <vostok/math_aabb.h>
#include <vostok/math_float4x4.h>
#include <vostok/render/core/options.h>

#include "grass_world.h"
#include "hw_hiz_occlusion_manager.h"
#include "light.h"
#include "render_surface_instance.h"
#include "renderer_context.h"
#include "scene.h"
#include "scene_view.h"

namespace vostok {
namespace render {

float4 aabb_to_occlusion_bound(
	math::aabb const&,
	float4x4 const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x604bd0]
	return float4( 1.0f, 1.0f, 1.0f, 1.0f );
}

stage_visibility::stage_visibility(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage								( in_renderer, context ),
	m_data_ready						( false ),
	m_occlusion_manager					( 0 ),
	m_static_bounds_array				( 0 ),
	m_static_results_array				( 0 ),
	m_current_occlusion_buffer_size		( 0 ),
	m_portals_offset_to_results			( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x606490]
}

stage_visibility::~stage_visibility( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x605f20]
}

void stage_visibility::debug_render( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x605f90]
}

void stage_visibility::execute( )
{
	// STATE[STUB]
	if ( !options::ref( ).current.m_use_hiz_occlusion_culling )
		frustum_culling( );
	else {
		m_data_ready = m_occlusion_manager->quary_and_get_results_if_ready(
			m_static_results_array,
			m_current_occlusion_buffer_size
		);
		frustum_culling( );
		if ( m_data_ready ) {
			occlusion_culling( );
			m_data_ready = false;
		}
	}

	gather_statistics( );
}

bool stage_visibility::query_data( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x604680]
	return false;
}

void stage_visibility::frustum_culling( )
{
	// STATE[STUB]
	scene* render_scene = m_context->scene( );
	if ( render_scene->get_grass( ) ) {
		render_scene->get_grass( )->process_culling( m_context, 100.f );
		m_context->get_scene_view( )->get_visible_grass_patches( ) =
			render_scene->get_grass( )->get_visible_patches( );
	}
}

void stage_visibility::occlusion_culling( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x605fb0]
}

bool stage_visibility::occluded( u32 ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x6044a0]
	return false;
}

void stage_visibility::get_results_and_prepare_bounds_models( float4*&, u32& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x605600]
}

void stage_visibility::get_results_and_prepare_bounds_lights( float4*&, u32& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x605470]
}

void stage_visibility::get_results_and_prepare_bounds_grass( float4*&, u32& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6052f0]
}

void stage_visibility::get_results_and_prepare_bounds_decals( float4*&, u32& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x605150]
}

void stage_visibility::get_results_and_prepare_bounds_env_probes( float4*&, u32& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x604fb0]
}

void stage_visibility::get_results_and_prepare_bounds_ambient_volumes( float4*&, u32& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x604e20]
}

void stage_visibility::get_results_and_prepare_bounds_particles( float4*&, u32& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x604ca0]
}

void stage_visibility::get_results_and_prepare_bounds_portals( float4*&, u32& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x604ae0]
}

bool is_not_occluded_predicate_light( light_ptr const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x604640]
	return false;
}

bool is_occluded_predicate_light( light_ptr const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x604610]
	return false;
}

template <typename T>
bool is_not_occluded_predicate( T const* const )
{
	return false;
}

template <>
bool is_not_occluded_predicate<render_surface_instance>(
	render_surface_instance const* const
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x604740]
	return false;
}

template <typename T>
bool is_occluded_predicate( T const* const )
{
	return false;
}

template <>
bool is_occluded_predicate<render_surface_instance>(
	render_surface_instance const* const
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x6046d0]
	return false;
}

void stage_visibility::gather_statistics( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x604780]
}

} // namespace render
} // namespace vostok
