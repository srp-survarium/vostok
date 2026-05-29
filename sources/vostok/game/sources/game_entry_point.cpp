////////////////////////////////////////////////////////////////////////////
//	Created 	: 11.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/physics/api.h>
#include <vostok/input/api.h>
#include <vostok/animation/api.h>
#include <vostok/rtp/api.h>
#include <vostok/ui/api.h>
#include <vostok/ai_navigation/api.h>
#include <vostok/ai/api.h>
#include <vostok/game/api.h>
#include "game.h"

using vostok::non_null;
using vostok::raw;

namespace boost
{
#ifndef VOSTOK_STATIC_LIBRARIES
	void throw_exception(std::exception const & e)
	{
		FATAL("boost exception caught: %s", e.what());
	}
#endif // #ifndef VOSTOK_STATIC_LIBRARIES
	void throw_exception(std::bad_alloc const & e)
	{
		VOSTOK_UNREFERENCED_PARAMETER	( e );
		FATAL("boost exception caught: %s", e.what());
	}
} // namespace boost


static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_input_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_animation_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_rtp_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_ui_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_ai_navigation_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_ai_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_game_allocator;
static vostok::uninitialized_reference< survarium::game >							s_game;

namespace survarium {

#ifdef VOSTOK_STATIC_LIBRARIES
vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	g_physics_allocator;
#endif // #ifdef VOSTOK_STATIC_LIBRARIES

vostok::memory::doug_lea_allocator_type*	g_allocator		= NULL;

vostok::engine_user::world* game_module::create_world	(
		vostok::engine_user::engine& engine, 
		vostok::render::world& render_world, 
		vostok::sound::world& sound,
		vostok::network::world& network
	)
{
#ifdef VOSTOK_STATIC_LIBRARIES
	g_physics_allocator->user_current_thread_id			( );
	vostok::physics::set_memory_allocator					( g_physics_allocator.c_ptr() );
#else // #ifdef VOSTOK_STATIC_LIBRARIES
	vostok::physics::set_memory_allocator					( memory::g_crt_allocator );
#endif // #ifdef VOSTOK_STATIC_LIBRARIES

	s_input_allocator->user_current_thread_id			( );
	vostok::input::set_memory_allocator					( *s_input_allocator );

	s_animation_allocator->user_current_thread_id		( );
	vostok::animation::set_memory_allocator				( *s_animation_allocator );

#if (!VOSTOK_RTP_LEARNING)
	s_rtp_allocator->user_current_thread_id				( );
	vostok::rtp::set_memory_allocator						( *s_rtp_allocator );
#else
	vostok::rtp::set_memory_allocator						( debug::g_mt_allocator );
#endif

	s_ui_allocator->user_current_thread_id				( );
//.	vostok::ui::set_memory_allocator						( *s_ui_allocator );

	s_ai_navigation_allocator->user_current_thread_id	( );
	vostok::ai::navigation::set_memory_allocator			( memory::g_mt_allocator );

	s_ai_allocator->user_current_thread_id				( );
	vostok::ai::set_memory_allocator						( *s_ai_allocator );

	s_game_allocator->user_current_thread_id			( );

	VOSTOK_CONSTRUCT_REFERENCE	( s_game, game )		( engine, render_world, sound, network );

	return												( &*s_game );
}

void game_module::destroy_world						( vostok::engine_user::world*& world )
{
	R_ASSERT								( &*s_game == world );
	VOSTOK_DESTROY_REFERENCE					( s_game );
	world									= 0;
}

static void register_rtp_allocator( )
{
#if (!VOSTOK_RTP_LEARNING)
	VOSTOK_CONSTRUCT_REFERENCE				( s_rtp_allocator, vostok::memory::doug_lea_allocator_type );
	s_rtp_allocator->do_register			(  3*Mb,	"rtp"		);
#endif // #if (!VOSTOK_RTP_LEARNING)
}


void game_module::register_memory_allocators	( )
{
#ifdef VOSTOK_STATIC_LIBRARIES
	VOSTOK_CONSTRUCT_REFERENCE				( g_physics_allocator, vostok::memory::doug_lea_allocator_type );
	g_physics_allocator->do_register		( 16*Mb,	"physics" );
#endif

	VOSTOK_CONSTRUCT_REFERENCE				( s_input_allocator, vostok::memory::doug_lea_allocator_type );
	s_input_allocator->do_register			( 64*Kb,	"input" );

	VOSTOK_CONSTRUCT_REFERENCE				( s_animation_allocator, vostok::memory::doug_lea_allocator_type );
	s_animation_allocator->do_register		(  10*Mb,	"animation" );

	register_rtp_allocator					( );

	VOSTOK_CONSTRUCT_REFERENCE				( s_ui_allocator, vostok::memory::doug_lea_allocator_type );
	s_ui_allocator->do_register				(  64*Kb,	"ui" );

	VOSTOK_CONSTRUCT_REFERENCE				( s_ai_navigation_allocator, vostok::memory::doug_lea_allocator_type );
	s_ai_navigation_allocator->do_register	(  64*Kb,	"ai navigation" );

	VOSTOK_CONSTRUCT_REFERENCE				( s_ai_allocator, vostok::memory::doug_lea_allocator_type );
	s_ai_allocator->do_register				(	8*Mb,	"ai" );


	VOSTOK_CONSTRUCT_REFERENCE				( s_game_allocator, vostok::memory::doug_lea_allocator_type );
	s_game_allocator->do_register			(   8*Mb,	"survarium" );

//	s_game_allocator->user_current_thread_id( );
	g_allocator								= &*s_game_allocator;
}

vostok::memory::doug_lea_allocator_type& game_module::allocator( )
{
	R_ASSERT								( g_allocator );
	return									*g_allocator;
}

} // namespace survarium
