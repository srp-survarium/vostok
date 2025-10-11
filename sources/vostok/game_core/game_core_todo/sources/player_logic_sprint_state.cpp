////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "player_logic_sprint_state.h"

namespace survarium {

// STATE[STUB]
// survarium::player_logic_sprint_state::player_logic_sprint_state(survarium::weapon_user_animations_selector&)
player_logic_sprint_state::player_logic_sprint_state( weapon_user_animations_selector& owner )
{
	// FUNCTION BODY
	// <0x6f9563>|0x000|0x000:'31'
	// ******
}

// STATE[STUB]
// stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> survarium::player_logic_sprint_state::selected_animations(vostok::mutable_buffer&, survarium::weapon_animation_parameters const&, const bool) const
std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme> player_logic_sprint_state::selected_animations( vostok::mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, bool is_third_view ) const
{
	// LOCALS
	// pcstr 						look_animation_id
	// vostok::animation::instant_interpolator interpolator
	// vostok::animation::mixing::animation_lexeme_parameters look_lexeme_parameters
	// float 						start_animation_interval_time
	// vostok::animation::mixing::animation_lexeme look_lexeme
	// vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> look_animation
	// vostok::animation::mixing::animation_lexeme movement_lexeme
	// vostok::animation::linear_interpolator l_interpolator
	// ******

	// CONSTANTS
	// const player_logic_sprint_state::selected_animations::__l2::<unnamed-tag> main_sprint_animation_index = 0;
	// const player_logic_sprint_state::selected_animations::__l2::<unnamed-tag> sprint_look_animation_index = 1;
	// ******

	// FUNCTION BODY
	// 1
	// <0x6f9681>|0x000|0x000:'41'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// 13
	// 14
	// <0x6f9691>|0x010|0x010:'56'
	// <0x6f97d6>|0x155|0x145:'57'
	// <0x6f97e1>|0x160|0x00b:'58'
	// <0x6f982a>|0x1a9|0x049:'59'
	// 1
	// <0x6f9886>|0x205|0x05c:'61'
	// <0x6f98a5>|0x224|0x01f:'62'
	// 1
	// 2
	// 3
	// 4
	// 5
	// <0x6f98de>|0x25d|0x039:'68'
	// 1
	// <0x6f9968>|0x2e7|0x08a:'70'
	// 1
	// <0x6f9979>|0x2f8|0x011:'72'
	// ******
}

// STATE[STUB]
// void survarium::player_logic_sprint_state::set_callbacks(boost::function<void __cdecl(void)> const&, boost::function<void __cdecl(void)> const&)
void player_logic_sprint_state::set_callbacks( boost::function<void __cdecl(void)> const& initialize_callback, boost::function<void __cdecl(void)> const& finalize_callback )
{
	// FUNCTION BODY
	// <0x6f94df>|0x000|0x000:'77'
	// <0x6f94f1>|0x012|0x012:'78'
	// ******
}

// STATE[STUB]
// void survarium::player_logic_sprint_state::on_stamina_depleted()
void player_logic_sprint_state::on_stamina_depleted( )
{
	// FUNCTION BODY
	// <0x6f94b7>|0x000|0x000:'83'
	// ******
}

// STATE[STUB]
// void survarium::player_logic_sprint_state::initialize()
void player_logic_sprint_state::initialize( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6e8650 }, type_index: TypeIndex(0x9176) })
	// ******

	// FUNCTION BODY
	// 1
	// <0x6f962f>|0x000|0x000:'89'
	// <0x6f9659>|0x02a|0x02a:'90'
	// ******
}

// STATE[STUB]
// void survarium::player_logic_sprint_state::finalize()
void player_logic_sprint_state::finalize( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6e8600 }, type_index: TypeIndex(0x9176) })
	// ******

	// FUNCTION BODY
	// 1
	// <0x6f95df>|0x000|0x000:'95'
	// <0x6f9609>|0x02a|0x02a:'96'
	// ******
}

} // namespace survarium
