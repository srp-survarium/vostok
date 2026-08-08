////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "base_game_scene.h"
#include "game.h"
#include "game_memory.h"	// NEW / DELETE (survarium::g_allocator)
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

// TU console-command static (survarium:: namespace scope). The atexit destructor
// (??__F) is a byte-for-byte match. The dynamic initializer (??__E) reconstructs
// to the SAME shape as every matched game-module cc_bool static (human_npc
// s_npc_debug_draw_command, login_menu s_store_user_pass_cc): list insertion +
// value-pointer + need_args byte + cc_bool vtable + atexit. The shared residual is
// the target's two extra console_command-base bool stores at +0x3C/+0x3D, which our
// console_command.h layout lacks - a cross-module (core) gap present in all repo
// cc_bool initializers, not steerable here. The command name / command_type /
// default live in .data (separate object), not these function bytes - "freeze_culling"
// is a best-guess for this render-debug culling toggle; the scored ??__E/??__F symbols
// do not depend on it.
static bool s_freeze_culling_value = false;
static vostok::console_commands::cc_bool s_freeze_culling( "freeze_culling", s_freeze_culling_value, true, vostok::console_commands::command_type_engine_internal );

// claude@NOTE: init-list structure is correct (members + the NEW(camera_director)),
// residual is /Od scheduling: the target hoists every scalar member store before the
// camera_director malloc and keeps two full epilogues (the alloc-success / alloc-fail
// branches), my base shares one epilogue and stores the trailing members after the
// malloc. Also the engine-base vtable at +0x0C is written once in the target but twice
// in base (engine base ctor then base_game_scene). Both are codegen-scheduling, not a
// source-structure error.
base_game_scene::base_game_scene( game& g ) :
	m_mouse_pos			( 0, 0 ),
	m_camera_director	( NEW( camera_director )( *this ) ),
	m_text_manager		( NULL ),
	m_game				( g ),
	m_is_ui_shown		( false ),
	m_physics_world		( NULL ),
	m_is_active			( false )
{
}

// claude@NOTE: body is correct (DELETE m_camera_director; the m_sound_scene release +
// game_scene base destruction are implicit). Residual: (1) the game_scene base dtor in the
// shipped engine is a boost::_bi::storage3<one_way_render_channel*, base_scene_ptr,
// base_scene_view_ptr>::~storage3 - our game_core/game_scene.h holds only 2 scene ptrs, no
// render-channel/storage3 (a game_core base-class structure gap, not editable here); (2) my
// DELETE inlines ~camera_director which rewrites the camera_director vtable, the target's
// does not. Both are other-class concerns.
base_game_scene::~base_game_scene( )
{
	DELETE( m_camera_director );
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

// claude@NOTE: scheduler / input_translator / scene_renderer are body-correct; the only
// residual is the entry register for `this` - the target keeps these tiny ref-returning
// accessors out-of-line with `this` in EAX (the LTCG eax-this convention, like the matched
// renderer() above), my base gets canonical thiscall ECX. Per patterns/eax-this-convention.md
// this is caused BY the anchor's member-fn-ptr address-take (it forces __thiscall ECX) and
// is verified non-steerable - base_game_scene is abstract so a real EAX-this CALL site
// cannot be created here. Structurally 100%; residual is the address-escape penalty.
scheduler& base_game_scene::scheduler( )
{
	return m_game.scheduler( );
}

// claude@NOTE: structure matches (3 stmts). Residual: the target re-reads m_active_camera
// for the call (`cmp [+84],0` / reload), my base CSEs it through one local because the
// const_cast caches the get_active_camera() result. The target calls a NON-const
// game_camera::tick() on the active camera, but camera_director only exposes a const
// get_active_camera() (returns game_camera const*) - so the shipped source had a non-const
// get_active_camera() overload (camera_director.h, another worker's file). const_cast here
// is the buildable stand-in; -7 byte residual is that CSE.
void base_game_scene::tick( const u32 __formal, const u32 current_time_in_ms, const bool is_game_paused )
{
	VOSTOK_UNREFERENCED_PARAMETER( __formal );

	if ( m_camera_director->get_active_camera( ) ) const_cast< game_camera* >( m_camera_director->get_active_camera( ) )->tick( );

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

// STATE[STUB]
// claude@NOTE: BLOCKED on output_window_size() (render-module accessor, see above).
// Target structure (10 stmts, 3 named locals: float3 pos, float4x4 vp, float4x4 view):
// reads output_window_size() into a uint2, halves x/y (shr 1), float4x4::try_invert on
// the camera view, math::mul4x4 to build vp, projects p through vp into screen coords
// (the SSE/x87 perspective-divide block at 197), writes result, then returns whether the
// point lands in the [0..size) screen rect. The math is recoverable once
// output_window_size() is matched - it is the only missing dependency.
bool base_game_scene::point_to_screen( float3 const& p, float2& result )
{
	VOSTOK_UNREFERENCED_PARAMETER( p );			// buildability stub
	VOSTOK_UNREFERENCED_PARAMETER( result );

	return false;
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
