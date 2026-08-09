#include "pch.h"
#include "stage_decals_accumulate.h"

#include <algorithm>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>
#include <vostok/render/core/res_effect.h>
#include <vostok/render/facade/render_stage_types.h>

#include "decal_instance.h"
#include "effect_apply_decal_normals.h"
#include "effect_decal_mask.h"
#include "renderer_context.h"
#include "renderer_context_targets.h"
#include "scene_view.h"
#include "statistics.h"
#include "system_renderer.h"

namespace vostok {
namespace render {

stage_decals_accumulate::stage_decals_accumulate(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage( in_renderer, context )
{
	effect_manager::ref( ).create_effect<effect_decal_mask>( &m_opaque_geometry_mask_effect );
	effect_manager::ref( ).create_effect<effect_apply_decal>( &m_apply_decal_effect );
	m_enabled = options::ref( ).current.m_enabled_decals_accumulate_stage;
}

stage_decals_accumulate::~stage_decals_accumulate( )
{
}

bool stage_decals_accumulate::is_effects_ready( ) const
{
	return m_opaque_geometry_mask_effect.c_ptr( ) != NULL && m_apply_decal_effect.c_ptr( ) != NULL;
}

void stage_decals_accumulate::debug_render( )
{
	static bool s_render_debug = false;
	if ( !s_render_debug )
		return;

	vector<decal_instance*>& visible_decals = m_context->get_scene_view( )->get_visible_decals( );

	for ( vector<decal_instance*>::iterator it = visible_decals.begin( ); it != visible_decals.end( ); ++it )
	{

		system_renderer::ref( ).draw_aabb( ( *it )->get_aabb( ), math::color( 255.0f, 0.0f, 0.0f, 255.0f ) );
	}
}

void stage_decals_accumulate::execute_disabled( )
{
	backend::ref( ).set_render_targets( &*m_context->get_rt( rt_decals_smoothness ), 0, 0, 0 );
	math::color const clear_color( 0.0f, 0.0f, 0.0f, 0.0f );
	backend::ref( ).clear_render_targets(
		clear_color,
		clear_color,
		clear_color,
		clear_color
	);
}

void stage_decals_accumulate::execute( )
{
	if ( !is_effects_ready( ) )
		return;

	vector<decal_instance*> visible_decals = m_context->get_scene_view( )->get_visible_decals( );

	struct sort_by_priority_predicate {
		bool operator()( decal_instance const* const a, decal_instance const* const b )
		{
			return a->get_properties( ).draw_priority > b->get_properties( ).draw_priority;
		}
	};

	if ( visible_decals.size( ) )
		std::sort( visible_decals.begin( ), visible_decals.end( ), sort_by_priority_predicate( ) );

	if ( !is_enabled( ) || visible_decals.empty( ) )
	{
		execute_disabled( );
		return;
	}
	// 3 target lines are likely retail-compiled-out source.
	backend::ref( ).set_render_targets(
		&*m_context->get_rt( rt_decals_diffuse ),
		&*m_context->get_rt( rt_decals_normal ),
		&*m_context->get_rt( rt_decals_smoothness ),
		0
	);

	backend::ref( ).reset_depth_stencil_target( );
	math::color const clear_color( 0.0f, 0.0f, 0.0f, 0.0f );

	backend::ref( ).clear_render_targets(
		clear_color,
		math::color( 0.5f, 0.5f, 0.0f, 0.0f ),
		clear_color,
		clear_color
	);
	// 2 target lines are likely retail-compiled-out source.
	D3D11_VIEWPORT tmp_viewport;
	D3D11_VIEWPORT orig_viewport;

	backend::ref( ).get_viewport( orig_viewport );
	// 2 target lines are likely retail-compiled-out source.
	tmp_viewport.TopLeftX = 0.0f;
	tmp_viewport.TopLeftY = 0.0f;
	tmp_viewport.Width = float( m_context->get_rt( rt_decals_diffuse )->width( ) );
	tmp_viewport.Height = float( m_context->get_rt( rt_decals_diffuse )->height( ) );
	tmp_viewport.MinDepth = 0.0f;
	tmp_viewport.MaxDepth = 1.0f;

	backend::ref( ).set_viewport( tmp_viewport );

	bool has_decals = false;

	for ( vector<decal_instance*>::iterator it = visible_decals.begin( ); it != visible_decals.end( ); ++it )
	{
		decal_instance* decal = *it;

		if ( options::ref( ).current.m_use_hiz_occlusion_culling && decal->is_occluded( ) )
			continue;

		statistics::ref( ).deferred_decals_stat_group.num_decal_draw_calls.value +=
			decal->draw(
				m_context,
				m_opaque_geometry_mask_effect,
				decals_accumulate_render_stage );
		has_decals = true;
	}

	backend::ref( ).set_viewport( orig_viewport );

	if ( has_decals )
	{
		m_apply_decal_effect->apply( effect_apply_decal::normals_blend_pass, 0 );
		system_renderer::ref( ).fill_surface( m_context->get_rt( rt_decals_blend_result ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), true, 0, 0.0f, 0.0f, 1.0f, 1.0f );

		backend::ref( ).flush_rt_shader_resources( );

		m_apply_decal_effect->apply( effect_apply_decal::normals_write_pass, 0 );
		system_renderer::ref( ).fill_surface( m_context->get_rt( rt_normal ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), true, 0, 0.0f, 0.0f, 1.0f, 1.0f );

		m_apply_decal_effect->apply( effect_apply_decal::diffuse_write_pass, 0 );
		system_renderer::ref( ).fill_surface( m_context->get_rt( rt_albedo ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), render_target_ptr( ), true, 0, 0.0f, 0.0f, 1.0f, 1.0f );
	}

	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
	// 2 target lines are likely retail-compiled-out source.
}

} // namespace render
} // namespace vostok
