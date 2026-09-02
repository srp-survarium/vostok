////////////////////////////////////////////////////////////////////////////
//	Created 	: 11.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/input/api.h>
#include <vostok/physics/api.h>
#include <vostok/ai_navigation/api.h>
#include <vostok/ai/api.h>
#include <vostok/command_line_extensions.h>

#include "game.h"
#include "game_generate_shaders.h"

// api.h takes render::world& but leaves declaring it to the includer
// (the legacy TU got it via physics/api.h -> render/engine/base_classes.h)
namespace vostok {
namespace render {
	class world;
} // namespace render
} // namespace vostok

#include <vostok/game/api.h>
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_input_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_ui_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_ai_navigation_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_ai_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_game_allocator;
static vostok::uninitialized_reference< survarium::game >							s_game;
static vostok::command_line::key											s_generate_shaders( "generate_shaders", "", "", "" );
static vostok::uninitialized_reference< survarium::generate_shaders_world >			s_generate_shaders_world;

namespace survarium {

void game_core_initialize( );

vostok::memory::doug_lea_allocator_type*	g_allocator		= NULL;

vostok::engine_user::world* game_module::create_world	(
		vostok::engine_user::engine& engine,
		vostok::render::world& render_world,
		vostok::sound::world& sound,
		vostok::network::world& network
	)
{
	game_core_initialize						( );
	physics::set_memory_allocator			( &memory::g_mt_allocator );

	s_input_allocator->user_current_thread_id	( );
	input::set_memory_allocator				( *s_input_allocator );
	s_ui_allocator->user_current_thread_id		( );

	s_ai_navigation_allocator->user_current_thread_id	( );
	ai::navigation::set_memory_allocator				( memory::g_mt_allocator );
	s_ai_allocator->user_current_thread_id				( );
	ai::set_memory_allocator							( *s_ai_allocator );

	s_game_allocator->user_current_thread_id	( );

	if ( s_generate_shaders.is_set( ) )
		VOSTOK_CONSTRUCT_REFERENCE	( s_generate_shaders_world, generate_shaders_world )( render_world );
	else
		VOSTOK_CONSTRUCT_REFERENCE	( s_game, game )( engine, render_world, sound, network );

	if ( s_generate_shaders.is_set( ) )
		return							( &*s_generate_shaders_world );

	return								( &*s_game );
}

void game_module::destroy_world						( vostok::engine_user::world*& world )
{
	VOSTOK_DESTROY_REFERENCE			( s_game );
	world									= NULL;
}

void game_module::register_memory_allocators		( )
{
	VOSTOK_CONSTRUCT_REFERENCE				( s_input_allocator, vostok::memory::doug_lea_allocator_type );
	s_input_allocator->do_register			( 64*Kb,	"input" );

	VOSTOK_CONSTRUCT_REFERENCE				( s_ui_allocator, vostok::memory::doug_lea_allocator_type );
	s_ui_allocator->do_register				( 64*Kb,	"ui" );

	VOSTOK_CONSTRUCT_REFERENCE				( s_ai_navigation_allocator, vostok::memory::doug_lea_allocator_type );
	s_ai_navigation_allocator->do_register	( 64*Kb,	"ai navigation" );

	VOSTOK_CONSTRUCT_REFERENCE				( s_ai_allocator, vostok::memory::doug_lea_allocator_type );
	s_ai_allocator->do_register				(  8*Mb,	"ai" );

	VOSTOK_CONSTRUCT_REFERENCE				( s_game_allocator, vostok::memory::doug_lea_allocator_type );
	s_game_allocator->do_register			( 64*Mb,	"survarium" );

	g_allocator								= &*s_game_allocator;
}

vostok::memory::doug_lea_allocator_type& game_module::allocator	( )
{
	return				*g_allocator;
}

} // namespace survarium
