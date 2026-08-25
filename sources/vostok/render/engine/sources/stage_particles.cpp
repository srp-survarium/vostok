#include "pch.h"
#include "stage_particles.h"
#include "effect_resolve_particles.h"

#include <vostok/console_command.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/core/dx11/effect_compiler.h>
#include <vostok/render/core/effect_manager.h>
#include <vostok/render/core/options.h>
#include <vostok/render/facade/render_stage_types.h>

#include "material_effects.h"
#include "render_particle_emitter_instance.h"
#include "renderer_context.h"
#include "scene.h"
#include "scene_view.h"

namespace vostok {
namespace render {

void effect_resolve_particles::compile(
	effect_compiler& compiler,
	custom_config_value const&
)
{
	compiler.begin_technique( );
	compiler.begin_pass( "eye_adaptation", NULL, "resolve_particles", shader_configuration( ), NULL );
	compiler.set_depth( false, false );
	compiler.set_cull_mode( D3D_CULL_NONE );
	compiler.set_fill_mode( D3D_FILL_SOLID );
	compiler.set_alpha_blend( false );
	compiler.set_alpha_blend( true, D3D_BLEND_SRC_ALPHA, D3D_BLEND_INV_SRC_ALPHA );
	compiler.set_texture( "t_particle_result", "$user$particle_result", 0, false, u32( -1 ) );

	compiler.end_pass( );
	compiler.end_technique( );
}

static u32 s_particle_render_mode_value = 0;
static console_commands::cc_u32 s_particle_render_mode(
	"particle_render_mode",
	s_particle_render_mode_value,
	0,
	100,
	true,
	console_commands::command_type_engine_internal
);

particle_shader_constants::particle_shader_constants( )
{
	m_right_view_vector				= backend::ref().register_constant_host( "right_view_vector", rc_float );
	m_up_view_vector				= backend::ref().register_constant_host( "up_view_vector", rc_float );
	m_view_location					= backend::ref().register_constant_host( "view_location", rc_float );
	m_rotation_fixed_axis			= backend::ref().register_constant_host( "rotation_fixed_axis", rc_float );

	m_current_time					= backend::ref().register_constant_host( "current_time", rc_float );

	m_use_align_by_dir				= backend::ref().register_constant_host( "use_align_by_dir", rc_float );
	m_use_fixed_axis				= backend::ref().register_constant_host( "use_fixed_axis", rc_float );
	m_locked_no_ratate_axis_index	= backend::ref().register_constant_host( "locked_no_ratate_axis_index", rc_float );

}

void particle_shader_constants::set_time( float time )
{
	backend::ref().set_ps_constant ( m_current_time, time);
}

void particle_shader_constants::set(
	float3 const up_vector,
	float3 const right_vector,
	float3 const view_location,
	particle::enum_particle_locked_axis locked_axis,
	particle::enum_particle_screen_alignment screen_alignment
)
{
	backend::ref().set_vs_constant ( m_right_view_vector, right_vector);
	backend::ref().set_vs_constant ( m_up_view_vector, up_vector);
	backend::ref().set_vs_constant ( m_view_location, view_location);

	float f_use_align_by_dir			= 0.0f;
	float f_use_fixed_axis				= 0.0f;
	float f_locked_no_ratate_axis_index = -1.0f;

	float3 rotation_fixed_axis	(0,0,0);

	if (screen_alignment==particle::particle_screen_alignment_to_axis)
	{
		bool const is_rotate_x = locked_axis==particle::particle_locked_axis_rotate_x;
		bool const is_rotate_y = locked_axis==particle::particle_locked_axis_rotate_y;
		bool const is_rotate_z = locked_axis==particle::particle_locked_axis_rotate_z;

		rotation_fixed_axis = float3(
			is_rotate_x,
			is_rotate_y,
			is_rotate_z
		);

		if (is_rotate_x || is_rotate_y || is_rotate_z)
		{
			f_use_fixed_axis = 1.0f;
		}
		else
		{
			f_locked_no_ratate_axis_index = float(locked_axis);
		}
	}
	else
	{
		f_use_align_by_dir = (screen_alignment==particle::particle_screen_alignment_to_path) ? 1.0f : 0.0f;
	}

	backend::ref().set_vs_constant ( m_rotation_fixed_axis, rotation_fixed_axis);
	backend::ref().set_vs_constant ( m_locked_no_ratate_axis_index, f_locked_no_ratate_axis_index);
	backend::ref().set_vs_constant ( m_use_align_by_dir, f_use_align_by_dir);
	backend::ref().set_vs_constant ( m_use_fixed_axis, f_use_fixed_axis);
}

stage_particles::stage_particles(
	renderer* in_renderer,
	renderer_context* context
) :
	stage( in_renderer, context )
{
	m_enabled = options::ref( ).current.m_enabled_particles_stage;

	effect_manager::ref( ).create_effect< effect_resolve_particles >( &m_resolve_particles_effect );
}

stage_particles::~stage_particles( )
{
}

bool stage_particles::is_effects_ready( ) const
{
	return m_resolve_particles_effect.c_ptr( ) != NULL;
}

void stage_particles::execute( )
{
	if ( !is_enabled( ) || !is_effects_ready( ) )
	{
		execute_disabled( );
		return;
	}

	particle::world* part_world = m_context->scene( )->particle_world( );
	if ( !part_world )
	{
		m_context->set_w( float4x4( ).identity( ) );
		return;
	}

	D3D11_VIEWPORT orig_viewport;
	backend::ref( ).get_viewport( orig_viewport );

	D3D11_VIEWPORT tmp_viewport;
	tmp_viewport.TopLeftX = 0.0f;
	tmp_viewport.TopLeftY = 0.0f;
	tmp_viewport.Width = float( m_context->get_rt( rt_particle_result )->width( ) );
	tmp_viewport.Height = float( m_context->get_rt( rt_particle_result )->height( ) );
	tmp_viewport.MinDepth = 0.0f;
	tmp_viewport.MaxDepth = clear_value;
	backend::ref( ).set_viewport( tmp_viewport );

	backend::ref( ).set_render_targets(
		&*m_context->get_rt( rt_particle_result ),
		0,
		0,
		0
	);
	backend::ref( ).reset_depth_stencil_target( );

	bool has_particles = false;
	vectora< particle::render_particle_emitter_instance* >& emitters =
		m_context->get_scene_view( )->get_visible_particle_instances( );

	for ( particle::render_particle_emitter_instances_type::const_iterator it = emitters.begin( ); it != emitters.end( ); ++it )
	{
		render_particle_emitter_instance* instance =
			static_cast< render_particle_emitter_instance* >( *it );

		if ( instance->is_occluded( ) )
			continue;

		u32 const num_particles = instance->get_num_particles( );
		if ( !num_particles )
			continue;

		particle::enum_particle_render_mode particle_render_mode =
			m_context->get_scene_view( )->get_particles_render_mode( );

		if (
			particle_render_mode == particle::normal_particle_render_mode &&
			instance->get_material_effects( ).stage_enable[particles_render_stage]
		)
		{
			instance->get_material_effects( ).m_effects[particles_render_stage]->apply( 0, 0 );

			particle_shader_constants::ref( ).set(
				m_context->get_v_inverted( ).transform_direction( float3( 0, 1000, 0 ) ).normalize( ),
				m_context->get_v_inverted( ).transform_direction( float3( 1000, 0, 0 ) ).normalize( ),
				m_context->get_v_inverted( ).lines[3].xyz( ),
				instance->locked_axis( ),
				instance->screen_alignment( )
			);
			particle_shader_constants::ref( ).set_time( m_context->m_current_time );

			m_context->set_w( instance->transform( ) );
			instance->render( m_context->get_v_inverted( ).lines[3].xyz( ), num_particles );

			if ( !has_particles )
				has_particles = true;
		}
		else
		{
			instance->draw_debug( m_context->get_v( ), particle_render_mode );
		}
	}

	backend::ref( ).set_viewport( orig_viewport );
	m_context->set_w( float4x4( ).identity( ) );
	backend::ref( ).reset_render_targets( );
	backend::ref( ).reset_depth_stencil_target( );
}

} // namespace render
} // namespace vostok
