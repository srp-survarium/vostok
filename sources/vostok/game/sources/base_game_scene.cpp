// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "base_game_scene.h"
#include "game.h"
#include "game_memory.h"
#include "camera_director.h"
#include "game_camera.h"
#include <vostok/render/facade/game_renderer.h>
#include <vostok/render/facade/scene_renderer.h>
#include <vostok/physics/api.h>
#include <vostok/physics/world.h>
#include <vostok/sound/world.h>
#include <vostok/sound/world_user.h>
#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/scaleform/sources/flash_factory.h>
#include <vostok/scaleform/sources/flash_text_manager.h>
#include <vostok/memory_extensions.h>
#include <vostok/console_command.h>

namespace survarium {

static bool s_freeze_culling_value = false;
static vostok::console_commands::cc_bool s_freeze_culling( "freeze_culling", s_freeze_culling_value, false, vostok::console_commands::command_type_user_specific );

base_game_scene::base_game_scene( game& g ) :
	m_mouse_pos			( 0, 0 ),
	m_text_manager		( NULL ),
	m_game				( g ),
	m_is_ui_shown		( false ),
	m_physics_world		( NULL ),
	m_is_active			( false )
{
	m_camera_director = VOSTOK_NEW_IMPL( *g_allocator, camera_director )( *this );
}

// claude@NOTE: target structure matches; residual is the retained empty camera_director vtable rewrite.
base_game_scene::~base_game_scene( )
{
	VOSTOK_DELETE_IMPL( *g_allocator, m_camera_director );
}

math::uint2 const& base_game_scene::output_window_size( ) const
{
	return m_game.render_output_window( )->current_size( );
}

void base_game_scene::show_movie( flash_movie_resource_ptr& movie )
{
	math::uint2 const& output_size = output_window_size( );
	movie->movie->SetViewport( output_size.x, output_size.y );
	renderer( ).show_movie( render_scene_view( ), movie );
}

void base_game_scene::hide_movie( flash_movie_resource_ptr& movie )
{
	if ( movie.c_ptr( ) )
		renderer( ).hide_movie( render_scene_view( ), movie );
}

void base_game_scene::show_text_manager( flash_text_manager* tm )
{
	math::uint2 const& output_size = output_window_size( );
	tm->set_viewport( output_size.x, output_size.y );
	renderer( ).show_text_manager( render_scene_view( ), tm );
}

void base_game_scene::hide_text_manager( flash_text_manager* tm )
{
	renderer( ).hide_text_manager( render_scene_view( ), tm );
}

// claude@NOTE: structure matches (1 stmt: alloc bullet_physics_world via g_mt_allocator
// then tail-call initialize()). Residual: my base routes through create_world_bt's
// base_allocator* param so VOSTOK_NEW_IMPL emits a strip_pointer( g_mt_allocator ) call;
// the target inlines strip_pointer to nothing and allocates straight through
// g_mt_allocator's vtable (so it likely NEWs bullet_physics_world directly with the
// g_mt_allocator OBJECT, needing the physics-private bullet_physics_world.h). The dropped
// allocator& ctor arg (target pushes only engine&) is an LTCG constant-arg elision.
void base_game_scene::init_physics( )
{
	( m_physics_world = physics::create_world_bt( &memory::g_mt_allocator, *this ) )->initialize( );
}

// claude@NOTE: structure matches (1 stmt: destroy_world = w->destroy() + delete via
// g_mt_allocator). Same allocator-path residual as init_physics (strip_pointer vs direct
// g_mt_allocator vtable).
void base_game_scene::destroy_physics( )
{
	physics::destroy_world( &memory::g_mt_allocator, m_physics_world );
}

scheduler& base_game_scene::scheduler( )
{
	return m_game.scheduler( );
}

void base_game_scene::tick( const u32 __formal, const u32 current_time_in_ms, const bool is_game_paused )
{
	VOSTOK_UNREFERENCED_PARAMETER( __formal );

	m_camera_director->tick( );

	if ( !is_game_paused && m_physics_world )
		m_physics_world->tick( current_time_in_ms );
}

render::game::renderer& base_game_scene::renderer( ) const
{
	return m_game.renderer( );
}

render::scene_renderer& base_game_scene::scene_renderer( ) const
{
	return m_game.renderer( ).scene( );
}

void base_game_scene::on_activate( )
{
	m_is_active = true;
	m_game.get_sound_world( ).get_logic_world_user( ).set_active_sound_scene( m_sound_scene, 0, 0 );
	m_camera_director->on_focus( true );
	m_mouse_pos.set( 50, 50 );
}

void base_game_scene::on_deactivate( )
{
	m_is_active = false;
	m_camera_director->on_focus( false );
}

// claude@NOTE: structure matches (4 stmts, 1 local cd). Residual: my base builds an ebp
// stack frame with 8-byte alignment (and esp,~8) for the invert4x3 temp, so it can't reuse
// ebp as a scratch register; the target has no aligned frame (sub esp,40h) and caches
// &m_projection_matrix in ebp across both set_*_matrix calls. Frame-layout/register-alloc
// only - the source shape is faithful.
void base_game_scene::apply_camera( camera_director& cd )
{
	m_inverted_view_matrix			= cd.get_inverted_view_matrix( );
	m_projection_matrix				= cd.get_projection_matrix( );

	scene_renderer( ).set_view_matrix		( render_scene_view( ), math::invert4x3( m_inverted_view_matrix ) );
	scene_renderer( ).set_projection_matrix	( render_scene_view( ), m_projection_matrix );
}

bool base_game_scene::point_to_screen( float3 const& p, float2& result )
{
	math::uint2 const& output_size = output_window_size( );
	u32 const half_width = output_size.x / 2, half_height = output_size.y / 2;

	float4x4 view;
	view.try_invert( m_inverted_view_matrix );
	float4x4 vp = math::mul4x4( view, m_projection_matrix );
	float3 pos = vp.transform( p );

	result.x = ( pos.x + 1.f ) * half_width;
	result.y = ( 1.f - pos.y ) * half_height;

	return pos.z < 0.f && result.x > 0.f && result.y > 0.f && result.x < output_size.x && result.y < output_size.y;
}

swf_input_translator& base_game_scene::input_translator( )
{
	return m_game.input_translator( );
}

void base_game_scene::create_text_manager( )
{
	m_text_manager = m_game.get_flash_factory( ).create_text_manager( );
	show_text_manager( m_text_manager );
}

void base_game_scene::on_after_tick( )
{
	m_camera_director->apply( );
}

} // namespace survarium
