////////////////////////////////////////////////////////////////////////////
//	Created 	: 11.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/input/api.h>
#include <vostok/ai_navigation/api.h>
#include <vostok/ai/api.h>

// api.h takes render::world& but leaves declaring it to the includer
// (the legacy TU got it via physics/api.h -> render/engine/base_classes.h)
namespace vostok {
namespace render {
	class world;
} // namespace render
} // namespace vostok

#include <vostok/game/api.h>
// /OPT:REF reachability dispatcher: survarium::IncludeAll's ctor fans out to every
// per-module anchor (anchor_game_core / anchor_network{,_core} / anchor_physics /
// anchor_collision / anchor_logging / anchor_ai_navigation / anchor_game), keeping
// the whole carcass cone in the base EXE for the delinker. create_world is the real
// engine entry point (the game_world ctor is a never-instantiated stub, so an anchor
// there gets /OPT:REF-stripped). Replaces the old monolithic temp_include_all anchor.
#include "../../game_core/sources/anchor.h"

// Canonical statics (game_entry_point.cpp lines 46-55): s_input_allocator,
// s_ui_allocator, s_ai_navigation_allocator, s_ai_allocator, s_game_allocator,
// s_game, s_generate_shaders, s_generate_shaders_world. The last three need
// game.h / game_generate_shaders.h rebuilt first - they are added when those
// land (game_entry_point.cpp stays in temp/structure_queue until then).
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_input_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_ui_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_ai_navigation_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_ai_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_game_allocator;

namespace survarium {

vostok::memory::doug_lea_allocator_type*	g_allocator		= NULL;

// STATE[STUB]
vostok::engine_user::world* game_module::create_world	(
		vostok::engine_user::engine& engine,
		vostok::render::world& render_world,
		vostok::sound::world& sound,
		vostok::network::world& network
	)
{
	VOSTOK_UNREFERENCED_PARAMETER	( engine );
	VOSTOK_UNREFERENCED_PARAMETER	( render_world );
	VOSTOK_UNREFERENCED_PARAMETER	( sound );
	VOSTOK_UNREFERENCED_PARAMETER	( network );

	survarium::IncludeAll	include_all_anchor;

	return NULL;

	// FUNCTION BODY[0x709350]: 54
	// <0x709353>|0x003|+0x005:'73'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x709358>|0x008|+0x005:'80'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x70935d>|0x00d|+0x03d:'86'
	// <0x70939a>|0x04a|+0x006:'87'
	// <0>
	// <0x7093a0>|0x050|+0x037:'89'
	// <0>
	// <1>
	// <0x7093d7>|0x087|+0x02d:'92'
	// <0x709404>|0x0b4|+0x00d:'93'
	// <0>
	// <0x709411>|0x0c1|+0x034:'95'
	// <0x709445>|0x0f5|+0x00c:'96'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x709451>|0x101|+0x034:'102'
	// <0>
	// <0x709485>|0x135|+0x02e:'104'
	// <0>
	// <0x7094b3>|0x163|+0x029:'106'
	// <0>
	// <0x7094dc>|0x18c|+0x002:'108'
	// <0>
	// <0x7094de>|0x18e|+0x026:'110'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x709504>|0x1b4|+0x027:'118'
	// <0>
	// <1>
	// <0x70952b>|0x1db|+0x009:'121'
	// <0>
	// <1>
	// <2>
	// <0x709534>|0x1e4|+0x005:'125'
	// <0>
	// ******
}

// STATE[STUB]
void game_module::destroy_world						( vostok::engine_user::world*& world )
{
	VOSTOK_UNREFERENCED_PARAMETER	( world );

	// CALL SITE INFO
	// <0x7090ad> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x7090a0]: 3
	// <0>
	// <0x7090a0>|0x000|+0x00f:'132'
	// <0x7090af>|0x00f|+0x014:'133'
	// ******
}

// STATE[STUB]
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
	s_game_allocator->do_register			(  8*Mb,	"survarium" );

	g_allocator								= &*s_game_allocator;

	// FUNCTION BODY[0x7090d0]: 23
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7090d9>|0x009|+0x071:'143'
	// <0x70914a>|0x07a|+0x018:'144'
	// <0>
	// <0x709162>|0x092|+0x05e:'146'
	// <0x7091c0>|0x0f0|+0x018:'147'
	// <0>
	// <0x7091d8>|0x108|+0x05e:'149'
	// <0x709236>|0x166|+0x018:'150'
	// <0>
	// <0x70924e>|0x17e|+0x05e:'152'
	// <0x7092ac>|0x1dc|+0x018:'153'
	// <0>
	// <1>
	// <0x7092c4>|0x1f4|+0x05c:'156'
	// <0x709320>|0x250|+0x015:'157'
	// <0>
	// <1>
	// <0x709335>|0x265|+0x008:'160'
	// ******
}

// STATE[STUB]
vostok::memory::doug_lea_allocator_type& game_module::allocator	( )
{
	R_ASSERT			( g_allocator );
	return				*g_allocator;

	// FUNCTION BODY[0x709090]: 2
	// <0>
	// <0x709090>|0x000|+0x005:'166'
	// ******
}

} // namespace survarium
