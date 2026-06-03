////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_logic_sprint_state.h"

namespace survarium {

// STATE[STUB]
// survarium::player_logic_sprint_state::player_logic_sprint_state(survarium::weapon_user_animations_selector&)
player_logic_sprint_state::player_logic_sprint_state( weapon_user_animations_selector& owner ) :
	player_logic_base_state	( owner, type_sprint )
{
	// FUNCTION BODY[0x6f9510]: 1
	// <0x6f9563>|0x053|+0x055:'31'
	// ******
}

// STATE[STUB]
// stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> survarium::player_logic_sprint_state::selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const
std::pair<animation::mixing::expression,animation::mixing::animation_lexeme> player_logic_sprint_state::selected_animations( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const
{
	// LOCALS
	// pcstr 						look_animation_id
	// animation::instant_interpolator interpolator
	// animation::mixing::animation_lexeme_parameters look_lexeme_parameters
	// float 						start_animation_interval_time
	// animation::mixing::animation_lexeme look_lexeme
	// resources::managed_resource_ptr look_animation
	// animation::mixing::animation_lexeme movement_lexeme
	// animation::linear_interpolator l_interpolator
	// ******

	// CONSTANTS
	// const player_logic_sprint_state::selected_animations::__l2::<unnamed-tag> main_sprint_animation_index = 0;
	// const player_logic_sprint_state::selected_animations::__l2::<unnamed-tag> sprint_look_animation_index = 1;
	// ******

	// FUNCTION BODY[0x6f9670]: 33
	// <0>
	// <0x6f9681>|0x011|+0x010:'41'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <0x6f9691>|0x021|+0x145:'56'
	// <0x6f97d6>|0x166|+0x00b:'57'
	// <0x6f97e1>|0x171|+0x049:'58'
	// <0x6f982a>|0x1ba|+0x05c:'59'
	// <0>
	// <0x6f9886>|0x216|+0x01f:'61'
	// <0x6f98a5>|0x235|+0x039:'62'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6f98de>|0x26e|+0x08a:'68'
	// <0>
	// <0x6f9968>|0x2f8|+0x011:'70'
	// <0>
	// <0x6f9979>|0x309|+0x075:'72'
	// ******
}

// STATE[STUB]
// void survarium::player_logic_sprint_state::set_callbacks(boost::function<void __cdecl(void)> const&, boost::function<void __cdecl(void)> const&)
void player_logic_sprint_state::set_callbacks( boost::function<void()> const& initialize_callback, boost::function<void()> const& finalize_callback )
{
	// FUNCTION BODY[0x6f94d0]: 2
	// <0x6f94df>|0x00f|+0x012:'77'
	// <0x6f94f1>|0x021|+0x012:'78'
	// ******
}

// STATE[STUB]
// void survarium::player_logic_sprint_state::on_stamina_depleted()
void player_logic_sprint_state::on_stamina_depleted( )
{
	// FUNCTION BODY[0x6f94b0]: 1
	// <0x6f94b7>|0x007|+0x00b:'83'
	// ******
}

// STATE[STUB]
// void survarium::player_logic_sprint_state::initialize()
void player_logic_sprint_state::initialize( )
{
	// CALL SITE INFO
	// <0x6f9650> -> player_stamina& <unknown>()
	// ******

	// FUNCTION BODY[0x6f9620]: 3
	// <0>
	// <0x6f962f>|0x00f|+0x02a:'89'
	// <0x6f9659>|0x039|+0x00e:'90'
	// ******
}

// STATE[STUB]
// void survarium::player_logic_sprint_state::finalize()
void player_logic_sprint_state::finalize( )
{
	// CALL SITE INFO
	// <0x6f9600> -> player_stamina& <unknown>()
	// ******

	// FUNCTION BODY[0x6f95d0]: 3
	// <0>
	// <0x6f95df>|0x00f|+0x02a:'95'
	// <0x6f9609>|0x039|+0x00e:'96'
	// ******
}

} // namespace survarium
