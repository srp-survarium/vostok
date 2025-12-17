////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_state.h>

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_shotgun_reload_state::weapon_core_shotgun_reload_state(survarium::weapon_core&, survarium::weapon_core_shotgun_reload_base_substate*, survarium::weapon_core_shotgun_reload_base_substate*, survarium::weapon_core_shotgun_reload_base_substate*)
weapon_core_shotgun_reload_state::weapon_core_shotgun_reload_state(
	weapon_core&								weapon,
	weapon_core_shotgun_reload_base_substate*	reload_start,
	weapon_core_shotgun_reload_base_substate*	reload_one_round,
	weapon_core_shotgun_reload_base_substate*	reload_finish
) : weapon_core_base_state( weapon, true )
{
	// FUNCTION BODY
	// <0x599d7f>|0x03f|+0x00d:'27'
	// <0x599d8c>|0x04c|+0x014:'28'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::~weapon_core_shotgun_reload_state()
weapon_core_shotgun_reload_state::~weapon_core_shotgun_reload_state( )
{
	// LOCALS
	// ai::fsm_state* 				state<1>
	// ******

	// FUNCTION BODY
	// <0x5998ec>|0x01c|+0x00e:'33'
	// <0x5998fa>|0x02a|+0x017|[1]:'34'
	// <0x599911>|0x041|+0x026:'35'
	// <0x599937>|0x067|+0x002:'36'
	// <0>
	// <0x599939>|0x069|+0x02c:'38'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::initialize()
void weapon_core_shotgun_reload_state::initialize( )
{
	// FUNCTION BODY
	// <0>
	// <0x599737>|0x007|+0x022:'44'
	// <0x599759>|0x029|+0x00a:'45'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::finalize()
void weapon_core_shotgun_reload_state::finalize( )
{
	// FUNCTION BODY
	// <0x599777>|0x007|+0x00d:'50'
	// <0x599784>|0x014|+0x010:'51'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::serialize(vostok::network_core::udp_match_packet&) const
void weapon_core_shotgun_reload_state::serialize( network_core::udp_match_packet& packet ) const
{
	// LOCALS
	// u8 							state_id
	// bool 						found
	// ai::fsm_state const* 		current
	// ai::fsm_state const* 		i<1>
	// ******

	// FUNCTION BODY
	// <0x599839>|0x009|+0x004:'56'
	// <0x59983d>|0x00d|+0x004:'57'
	// <0x599841>|0x011|+0x00f:'58'
	// <0x599850>|0x020|+0x02f|[1]:'59'
	// <0x59987f>|0x04f|+0x008:'60'
	// <0x599887>|0x057|+0x004:'61'
	// <0x59988b>|0x05b|+0x002:'62'
	// <0>
	// <0x59988d>|0x05d|+0x002:'64'
	// <0>
	// <0x59988f>|0x05f|+0x00c:'66'
	// <0x59989b>|0x06b|+0x00d:'67'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::deserialize(vostok::network_core::packet_reader&)
void weapon_core_shotgun_reload_state::deserialize( network_core::packet_reader& reader )
{
	// LOCALS
	// u8 							target_state_id
	// u8 							state_id
	// ai::fsm_state* 				current
	// ai::fsm_state* 				i<1>
	// ******

	// FUNCTION BODY
	// <0x5997a9>|0x009|+0x00b:'72'
	// <0x5997b4>|0x014|+0x004:'73'
	// <0x5997b8>|0x018|+0x007:'74'
	// <0x5997bf>|0x01f|+0x02f|[1]:'75'
	// <0x5997ee>|0x04e|+0x00c:'76'
	// <0x5997fa>|0x05a|+0x006:'77'
	// <0x599800>|0x060|+0x002:'78'
	// <0>
	// <0x599802>|0x062|+0x002:'80'
	// <0>
	// <0x599804>|0x064|+0x00c:'82'
	// <0x599810>|0x070|+0x012:'83'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_shotgun_reload_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
animation::mixing::expression weapon_core_shotgun_reload_state::weapon_and_hands_expression(
	mutable_buffer&						buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	animation::mixing::animation_lexeme&	weight_driving_animation
) const
{
	// FUNCTION BODY
	// <0>
	// <0x599db9>|0x009|+0x035:'89'
	// ******
}

// STATE[STUB]
// bool survarium::true_predicate()
static bool true_predicate( )
{
	return false;

	// FUNCTION BODY
	// <0x599723>|0x003|+0x002:'94'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::initialize_logic(survarium::weapon_core_shotgun_reload_base_substate*, survarium::weapon_core_shotgun_reload_base_substate*, survarium::weapon_core_shotgun_reload_base_substate*)
void weapon_core_shotgun_reload_state::initialize_logic( weapon_core_shotgun_reload_base_substate* reload_start, weapon_core_shotgun_reload_base_substate* reload_one_round, weapon_core_shotgun_reload_base_substate* reload_finish )
{
	// FUNCTION BODY
	// <0x599ae0>|0x010|+0x05d:'99'
	// <0x599b3d>|0x06d|+0x014:'100'
	// <0x599b51>|0x081|+0x015:'101'
	// <0x599b66>|0x096|+0x01b:'102'
	// <0x599b81>|0x0b1|+0x015:'103'
	// <0x599b96>|0x0c6|+0x015:'104'
	// <0x599bab>|0x0db|+0x015:'105'
	// <0x599bc0>|0x0f0|+0x01b:'106'
	// <0x599bdb>|0x10b|+0x065:'107'
	// <0x599c40>|0x170|+0x0ea:'108'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core_shotgun_reload_state::finish_reload_predicate() const
bool weapon_core_shotgun_reload_state::finish_reload_predicate( ) const
{
	return false;

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <0x599991>|0x011|+0x137:'116'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::execute()
void weapon_core_shotgun_reload_state::execute( )
{
	// FUNCTION BODY
	// <0x5998b7>|0x007|+0x00e:'121'
	// ******
}

} // namespace survarium
