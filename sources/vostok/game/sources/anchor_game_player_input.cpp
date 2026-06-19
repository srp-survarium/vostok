////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for the survarium player-input pair:
// the player_input_handler camera/input handler (player_input_handler.cpp) and the
// survarium::player tick/history logic (player_tick.cpp). These live in the game
// module whose owner TU (player.cpp) is still a STUB, so nothing in the linked EXE
// references the player_tick.cpp helpers (private history/smooth/time_warp helpers)
// or the player_input_handler out-of-line methods yet - /OPT:REF strips them, the
// delinker finds no base symbol, and every function pairs as "unpaired".
//
// use_game_player_input() CALLS each out-of-line method on a never-dereferenced
// placeholder object (the body is guarded by a never-true static volatile flag so it
// never actually runs). The call is a hard reference LTCG can not elide (unlike a
// member-function-pointer address-take in a dead branch, which whole-program opt
// folds away), so the linker keeps the carcass objects in the base EXE for the
// delinker to score. The anchor is a friend of both classes for the private members.
//
// Retire once the real game call graph (player.cpp's tick/serialize cone, and the
// player_input_handler construction inside player::attach_controller) reaches these.

#include "pch.h"

#include "player.h"
#include "player_input_handler.h"
#include "game_world.h"
#include "client_player_state.h"
#include "client_player_history_item.h"

namespace survarium {

struct server_player_update;

void use_game_player_input( )
{
	static volatile bool s_run = false;
	if( !s_run )
		return;

	// construct a player_input_handler so its ctor / dtor / vtable stay linked
	// (the ctor is reached only from the still-stub player::attach_controller).
	static game_world* volatile s_world = 0;
	player_input_handler		built_handler( *s_world );

	// launder the placeholders through volatile pointers so LTCG can not see they
	// are NULL (it would const-fold member accesses) - the bodies never run.
	static player* volatile						s_player	= 0;
	static player_input_handler* volatile		s_handler	= 0;
	static client_player_history_item* volatile	s_item		= 0;
	static client_player_state* volatile		s_state		= 0;
	static server_player_update* volatile		s_update	= 0;
	static float4x4* volatile					s_xf		= 0;
	static player_input* volatile				s_input		= 0;

	player&							p		= *s_player;
	player_input_handler&			h		= *s_handler;
	client_player_history_item&		item	= *s_item;
	client_player_state&			state	= *s_state;
	server_player_update const&		update	= *s_update;
	float4x4&						xf		= *s_xf;
	player_input const&				input	= *s_input;

	static volatile u32 v = 0;
	const u32 any_u32 = v;
	bool b = s_run;

	// player_input_handler out-of-line members.
	h.set_yaw_pitch_distance( 0.f, 0.f, 0.f );
	h.set_input_mode( first_person_mode );
	h.update_inverted_view( xf );
	h.alt_is_held( );
	h.process_first_person_mode( b );
	h.process_third_person_mode( );

	// player tick / history helpers (player_tick.cpp); mostly private - reached
	// through friend access.
	p.history_lower_bound_index( any_u32 );
	p.remove_oldest_history_item( );
	p.remove_oldest_history_items( any_u32 );
	p.smooth( 0.f );
	p.restore_history_item( item );
	p.update_history_item_from_previous( item, item, xf );
	p.update_history_item( item, s_item, update, any_u32, xf, b );
	p.apply_input_before_new_transform( state, input, 0.f );
	p.replay_history( any_u32, xf );
	p.time_warp( update, any_u32 );
	p.tick( any_u32 );
	p.log_active_object( "" );
}

} // namespace survarium
