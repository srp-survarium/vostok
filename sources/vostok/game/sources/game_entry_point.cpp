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
// s_game (uninitialized_reference<game>), s_generate_shaders (command_line::key),
// s_generate_shaders_world (uninitialized_reference<generate_shaders_world>). The
// last three are used ONLY by create_world/destroy_world, which are parked on the
// IncludeAll-anchor wall (see create_world note); declaring them now would be dead
// statics, so they land WITH that reconstruction.
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_input_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_ui_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_ai_navigation_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_ai_allocator;
static vostok::uninitialized_reference< vostok::memory::doug_lea_allocator_type >	s_game_allocator;

namespace survarium {

vostok::memory::doug_lea_allocator_type*	g_allocator		= NULL;

// claude@NOTE: PARKED (~3.6%). create_world is the game carcass's /OPT:REF
// reachability ROOT - it must instantiate survarium::IncludeAll (anchor_game.cpp
// dispatches every per-module use_*() from IncludeAll::IncludeAll), so the source
// permanently carries one extra `IncludeAll include_all_anchor;` statement the
// target body does not have. The real target body (structure @0x709350, 17 stmts):
// game_core_initialize(); physics::set_memory_allocator(...); input/ui/ai_navigation/ai
// ::set_memory_allocator(...) (the input/ui ones inline to a g_allocator store + an
// allocator owning-thread TLS context, repeated per allocator); then a function-local-
// guarded `static command_line::key s_generate_shaders(...)` magic-static gates two
// branches: if set, construct s_generate_shaders_world (uninitialized_reference<
// generate_shaders_world>) and return it; else construct s_game (uninitialized_reference<
// game>) via the LTCG-custom-convention game::game ctor (this pushed, not in ecx - a
// DIFFERENT, divergent TU) and return it. NEXT STEP: reconstruct the init sequence +
// branch ONLY AFTER the carcass is complete and IncludeAll is retired (memory:
// "Network anchors are temporary"); the divergent game::game ctor + magic-static key
// + the +1 anchor statement cap this below a clean match until then.
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
}

// claude@NOTE: PARKED (unpaired). destroy_world's body is small (3 stmts) but reads
// the s_game static created by create_world: it virtual-destroys s_game via vtable
// slot 0x2c (`push 0; call [vptr+2Ch]` = explicit 'vector deleting destructor'(0),
// destroy-don't-free), zeroes the s_game `constructed` flag, then `world = NULL`.
// Cannot be reconstructed faithfully until s_game (uninitialized_reference<game>) is
// declared and its create_world construction lands - both blocked on the same
// IncludeAll-anchor / divergent-game-ctor wall above. NEXT STEP: body together with
// create_world once that unblocks; the static layout pins the exact spelling.
void game_module::destroy_world						( vostok::engine_user::world*& world )
{
	VOSTOK_UNREFERENCED_PARAMETER	( world );
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
	s_game_allocator->do_register			( 64*Mb,	"survarium" );

	g_allocator								= &*s_game_allocator;
}

vostok::memory::doug_lea_allocator_type& game_module::allocator	( )
{
	return				*g_allocator;
}

} // namespace survarium
