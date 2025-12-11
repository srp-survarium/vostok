////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_logic_jump_state.h"

namespace survarium {

// STATE[STUB]
// survarium::player_logic_jump_state::player_logic_jump_state(survarium::weapon_user_animations_selector&)
player_logic_jump_state::player_logic_jump_state( weapon_user_animations_selector& owner )
{
	// FUNCTION BODY
	// <0x6f9470>|0x000|+0x02d:'25'	{
	// <0x6f949d>|0x02d|      :'26'	}
	// ******
}

// STATE[STUB]
// void survarium::player_logic_jump_state::initialize()
void player_logic_jump_state::initialize( )
{
	// FUNCTION BODY
	// <0>
	// <1>
	// <0x6f9379>|0x009|+0x00b:'32'
	// <0x6f9384>|0x014|+0x00c:'33'
	// ******
}

// STATE[STUB]
// void survarium::player_logic_jump_state::execute()
void player_logic_jump_state::execute( )
{
	// FUNCTION BODY
	// <0x6f9457>|0x007|+0x00b:'38'
	// ******
}

// STATE[STUB]
// bool survarium::player_logic_jump_state::is_ready_for_transition() const
bool player_logic_jump_state::is_ready_for_transition( ) const
{
	// CALL SITE INFO
	// <0x6f93cc> -> resources::resource_ptr<damage_model,resources::unmanaged_intrusive_base> const& <unknown>() const
	// ******

	return false;

	// FUNCTION BODY
	// <0x6f93a9>|0x009|+0x070:'43'
	// ******
}

// STATE[STUB]
// void survarium::player_logic_jump_state::finalize()
void player_logic_jump_state::finalize( )
{
	// FUNCTION BODY
	// <0>
	// <0x6f9357>|0x007|+0x00b:'49'
	// ******
}

// STATE[STUB]
// stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> survarium::player_logic_jump_state::selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const
std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> player_logic_jump_state::selected_animations( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const
{
	// FUNCTION BODY
	// <0x6f9427>|0x007|+0x01f:'58'
	// ******
}

// STATE[STUB]
// void survarium::player_logic_jump_state::set_user(survarium::base_player&)
void player_logic_jump_state::set_user( base_player& user )
{
	// FUNCTION BODY
	// <0x6f9327>|0x007|+0x00c:'63'
	// <0x6f9333>|0x013|+0x00f:'64'
	// ******
}

} // namespace survarium
