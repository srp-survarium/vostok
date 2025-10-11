////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_logic_jump_state.h"

namespace survarium {

// STATE[STUB]
// survarium::player_logic_jump_state::player_logic_jump_state(survarium::weapon_user_animations_selector&)
player_logic_jump_state::player_logic_jump_state( weapon_user_animations_selector& owner )
{
}

// STATE[STUB]
// void survarium::player_logic_jump_state::initialize()
void player_logic_jump_state::initialize( )
{
	// FUNCTION BODY
	// 1
	// 2
	// <0x6f9379>|0x000|0x000:'32'
	// <0x6f9384>|0x00b|0x00b:'33'
	// ******
}

// STATE[STUB]
// void survarium::player_logic_jump_state::execute()
void player_logic_jump_state::execute( )
{
	// FUNCTION BODY
	// <0x6f9457>|0x000|0x000:'38'
	// ******
}

// STATE[STUB]
// bool survarium::player_logic_jump_state::is_ready_for_transition() const
bool player_logic_jump_state::is_ready_for_transition( ) const
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6e83cc }, type_index: TypeIndex(0x916d) })
	// ******

	return false;
	// FUNCTION BODY
	// <0x6f93a9>|0x000|0x000:'43'
	// ******
}

// STATE[STUB]
// void survarium::player_logic_jump_state::finalize()
void player_logic_jump_state::finalize( )
{
	// FUNCTION BODY
	// 1
	// <0x6f9357>|0x000|0x000:'49'
	// ******
}

// STATE[STUB]
// stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> survarium::player_logic_jump_state::selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const
std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> player_logic_jump_state::selected_animations( vostok::mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const
{
	// FUNCTION BODY
	// <0x6f9427>|0x000|0x000:'58'
	// ******
}

// STATE[STUB]
// void survarium::player_logic_jump_state::set_user(survarium::base_player&)
void player_logic_jump_state::set_user( base_player& user )
{
	// FUNCTION BODY
	// <0x6f9327>|0x000|0x000:'63'
	// <0x6f9333>|0x00c|0x00c:'64'
	// ******
}

} // namespace survarium
