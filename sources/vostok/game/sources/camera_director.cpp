////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "camera_director.h"
#include "base_game_scene.h"
#include "game.h"
#include "game_camera.h"
#include <vostok/console_command.h>

namespace survarium {
static vostok::console_commands::cc_float	cc_cam_fov( "fov", default_vertical_fov, 60.0f, 70.0f, true, vostok::console_commands::command_type_user_specific );
camera_director::camera_director( base_game_scene& w ) :
	m_game_scene		( w ),
	m_active_camera		( NULL )
{
	static vostok::console_commands::cc_float3	cc_cam_pos	(
		"camera_pos", m_inverted_view.c.xyz(),
		float3( -1000, -1000, -1000 ), float3( 1000, 1000, 1000 ),
		false, vostok::console_commands::command_type_user_specific );

	set_position_direction	( float3( 16.f, 10.f, 0.f ), float3( -1.f, -1.f, 0.f ).normalize() );

	m_projection		= math::create_perspective_projection(
		( default_vertical_fov * ( math::pi / 180.0f ) ) * 0.75f,
		4.f / 3.f, 0.2f, 5000.0f );
}

void camera_director::set_position_direction( float3 const& p, float3 const& d )
{
	m_inverted_view	= math::invert4x3( math::create_camera_direction( p, d, float3( 0.f, 1.f, 0.f ) ) );
}

void camera_director::tick( )
{
	if ( m_active_camera )
		m_active_camera->tick	( );
}

void camera_director::apply( )
{
	if ( m_active_camera )
	{
		m_inverted_view			= m_active_camera->get_inverted_view_matrix();

		float2 window_size		= m_game_scene.get_game().engine().get_render_window_size();
		m_projection			= m_active_camera->get_projection_matrix( window_size );
	}
	m_game_scene.apply_camera( *this );
}

// the LOG_INFO below bakes __LINE__ (57) into the call - keep its source line fixed.
// Byte residual is the logging::helper inline init shape (non-steerable from here).
void camera_director::switch_to_camera( game_camera* c, pcstr camera_name )
{
	LOG_INFO( "switching to camera %s", camera_name );

	bool bfocused = m_game_scene.is_active();

	if ( m_active_camera )
	{
		if ( m_active_camera == c )
			return;

		if ( bfocused )
			m_active_camera->on_focus	( false );

		m_active_camera->on_deactivate	( );
	}

	m_active_camera = c;

	if ( m_active_camera )
	{
		m_active_camera->on_activate( this );

		if ( bfocused )
			m_active_camera->on_focus	( true );
	}
}

void camera_director::on_focus( bool b_focus_enter )
{
	if ( m_active_camera )
		m_active_camera->on_focus( b_focus_enter );
}

// claude@NOTE: ctor / apply / switch_to_camera are all STRUCTURE MATCH; remaining byte
// residual is non-steerable codegen, not a structural gap:
//   ctor    - MSVC keeps a frame pointer + esp realignment (target omits both), the
//             cc_float3 default-arg float3 temporaries schedule on different esp slots,
//             and the .normalize() / far-plane float constants ICF-fold against
//             differently-named same-valued .rdata slots.
//   apply   - the m_game_scene.apply_camera( *this ) call materializes *this in a
//             different register (push ebx vs push ecx + mov edx,ebx).
//   switch  - the inlined logging::helper init orders ebx (xor vs mov 1) differently.
} // namespace survarium
