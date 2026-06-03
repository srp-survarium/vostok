////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_logic_jump_state.h"
#include <vostok/game_core/base_player.h>

namespace survarium {

// STATE[100%|DONE]
player_logic_jump_state::player_logic_jump_state( weapon_user_animations_selector& owner ) :
	player_logic_base_state	( owner, type_jump ),
	m_logic					( owner )
{
}

// STATE[100%|DONE]
void player_logic_jump_state::initialize( )
{
	m_logic.activate( );
	ASSERT( UNKNOWN_EXPRESSION );
}

// STATE[100%|DONE]
void player_logic_jump_state::execute( )
{
	m_logic.tick( );
}

// STATE[79.48%|PARTIAL]: control flow + broken_legs_count match; residual is two
// upstream inline-vs-outline decisions this unit cannot steer:
//   1. intrusive_ptr::operator* is inlined here but the target keeps it out-of-line
//      (`return *m_object;` deref emitted inline -> extra mov chain @0x3d-0x42).
//   2. base_player::is_alive() is a real out-of-line symbol in the target (rva 0x86310),
//      but the header declares it `inline { return m_is_alive; }`, so it folds to a
//      direct `[+119h]` read instead of `call is_alive`. Fixing both needs out-of-unit
//      header edits (intrusive_ptr_inline.h / base_player.h+.cpp). See .md.
bool player_logic_jump_state::is_ready_for_transition( ) const
{
	return m_logic.is_jump_finished( )
		|| ( *m_user->damage_model( ) ).broken_legs_count( ) == 2
		|| !m_user->is_alive( );

	// CALL SITE INFO
	// <0x6f93cc> -> resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const& <unknown>() const
	// ******

	// FUNCTION BODY
	// <0x6f93a9>|0x009|+0x070:'43'	return is_jump_finished() || broken_legs_count()==2 || !is_alive();
	// ******
	// --- target asm (divergence: out-of-line operator* @0x97, out-of-line is_alive @0xC3) ---
	// 0x2c: call eax                          ; m_user->damage_model() (virtual)
	// 0x2e: mov [ebp-8], eax                  ; (target stores operator* result once)
	// 0x38: call operator*                    ; intrusive_ptr::operator* (out-of-line)
	// 0x48: movzx edx, byte [ecx+339h]        ; m_broken_legs_count[1]
	// 0x52: movzx ecx, byte [eax+338h]        ; m_broken_legs_count[0]
	// 0x59: add ecx, edx ; 0x5b: movzx edx, cl; 0x5e: cmp edx, 2
	// 0x69: call is_alive                     ; base_player::is_alive (out-of-line)
}

// STATE[100%|DONE]
void player_logic_jump_state::finalize( )
{
	m_logic.deactivate( );
}

// STATE[INPROGRESS]
// claude@TODO: body is `return m_logic.selected_animations( buffer, weapon_parameters,
// is_third_view );` (single delegation, asm @0x6e9420 below). BLOCKED: jump_logic::
// selected_animations is still a STUB with no return value, so linking the real
// delegation triggers C4716 -> LNK1257 during LTCG. Match jump_logic::selected_animations
// first (separate unit), then drop in the one-line delegation here.
// std::pair<...> survarium::player_logic_jump_state::selected_animations(mutable_buffer&, weapon_animation_parameters const&, const bool) const
std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> player_logic_jump_state::selected_animations( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const
{
	// claude@TODO: real body is the one-line delegation below; placeholder keeps the
	// pair-returning override compilable (C4716) until jump_logic::selected_animations
	// is matched.
	VOSTOK_UNREACHABLE_CODE( );

	// FUNCTION BODY
	// <0x6f9427>|0x007|+0x01f:'58'	return m_logic.selected_animations( buffer, weapon_parameters, is_third_view );
	// ******
}

// STATE[100%|DONE]
void player_logic_jump_state::set_user( base_player& user )
{
	player_logic_base_state::set_user( user );
	m_logic.set_user( user );
}

} // namespace survarium
