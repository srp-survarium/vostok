////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "weapon_core_shotgun_reload_state.h"

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_shotgun_reload_state::weapon_core_shotgun_reload_state(survarium::weapon_core&, survarium::weapon_core_shotgun_reload_base_substate*, survarium::weapon_core_shotgun_reload_base_substate*, survarium::weapon_core_shotgun_reload_base_substate*)
weapon_core_shotgun_reload_state::weapon_core_shotgun_reload_state(
	weapon_core&						weapon,
	weapon_core_shotgun_reload_base_substate*	reload_start,
	weapon_core_shotgun_reload_base_substate*	reload_one_round,
	weapon_core_shotgun_reload_base_substate*	reload_finish)
{
	// FUNCTION BODY
	// <0x599d7f>|0x000|0x000:'27'
	// <0x599d8c>|0x00d|0x00d:'28'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::~weapon_core_shotgun_reload_state()
void weapon_core_shotgun_reload_state::~weapon_core_shotgun_reload_state( )
{
	// LOCALS
	// vostok::ai::fsm_state* 		state<1>
	// ******

	// FUNCTION BODY
	// <0x5998ec>|0x000|0x000:'33'
	// <0x5998fa>|0x00e|0x00e|[1]:'34'
	// <0x599911>|0x025|0x017:'35'
	// <0x599937>|0x04b|0x026:'36'
	// 1
	// <0x599939>|0x04d|0x002:'38'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::initialize()
void weapon_core_shotgun_reload_state::initialize( )
{
	// FUNCTION BODY
	// 1
	// <0x599737>|0x000|0x000:'44'
	// <0x599759>|0x022|0x022:'45'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::finalize()
void weapon_core_shotgun_reload_state::finalize( )
{
	// FUNCTION BODY
	// <0x599777>|0x000|0x000:'50'
	// <0x599784>|0x00d|0x00d:'51'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::serialize(vostok::network_core::udp_match_packet&) const
void weapon_core_shotgun_reload_state::serialize( vostok::network_core::udp_match_packet& packet ) const
{
	// LOCALS
	// u8 							state_id
	// bool 						found
	// vostok::ai::fsm_state const* current
	// vostok::ai::fsm_state const* i<1>
	// ******

	// FUNCTION BODY
	// <0x599839>|0x000|0x000:'56'
	// <0x59983d>|0x004|0x004:'57'
	// <0x599841>|0x008|0x004:'58'
	// <0x599850>|0x017|0x00f|[1]:'59'
	// <0x59987f>|0x046|0x02f:'60'
	// <0x599887>|0x04e|0x008:'61'
	// <0x59988b>|0x052|0x004:'62'
	// 1
	// <0x59988d>|0x054|0x002:'64'
	// 1
	// <0x59988f>|0x056|0x002:'66'
	// <0x59989b>|0x062|0x00c:'67'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::deserialize(vostok::network_core::packet_reader&)
void weapon_core_shotgun_reload_state::deserialize( vostok::network_core::packet_reader& reader )
{
	// LOCALS
	// u8 							target_state_id
	// u8 							state_id
	// vostok::ai::fsm_state* 		current
	// vostok::ai::fsm_state* 		i<1>
	// ******

	// FUNCTION BODY
	// <0x5997a9>|0x000|0x000:'72'
	// <0x5997b4>|0x00b|0x00b:'73'
	// <0x5997b8>|0x00f|0x004:'74'
	// <0x5997bf>|0x016|0x007|[1]:'75'
	// <0x5997ee>|0x045|0x02f:'76'
	// <0x5997fa>|0x051|0x00c:'77'
	// <0x599800>|0x057|0x006:'78'
	// 1
	// <0x599802>|0x059|0x002:'80'
	// 1
	// <0x599804>|0x05b|0x002:'82'
	// <0x599810>|0x067|0x00c:'83'
	// ******
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_core_shotgun_reload_state::weapon_and_hands_expression(vostok::mutable_buffer&, const bool, const survarium::weapon_user_state_enum, vostok::animation::mixing::animation_lexeme&) const
vostok::animation::mixing::expression weapon_core_shotgun_reload_state::weapon_and_hands_expression(
	vostok::mutable_buffer&				buffer,
	bool								is_third_view,
	weapon_user_state_enum				user_state_id,
	vostok::animation::mixing::animation_lexeme&	weight_driving_animation) const
{
	// FUNCTION BODY
	// 1
	// <0x599db9>|0x000|0x000:'89'
	// ******
}

// STATE[STUB]
// bool survarium::true_predicate()
bool true_predicate( )
{
	return false;
	// FUNCTION BODY
	// <0x599723>|0x000|0x000:'94'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::initialize_logic(survarium::weapon_core_shotgun_reload_base_substate*, survarium::weapon_core_shotgun_reload_base_substate*, survarium::weapon_core_shotgun_reload_base_substate*)
void weapon_core_shotgun_reload_state::initialize_logic( weapon_core_shotgun_reload_base_substate* reload_start, weapon_core_shotgun_reload_base_substate* reload_one_round, weapon_core_shotgun_reload_base_substate* reload_finish )
{
	// FUNCTION BODY
	// <0x599ae0>|0x000|0x000:'99'
	// <0x599b3d>|0x05d|0x05d:'100'
	// <0x599b51>|0x071|0x014:'101'
	// <0x599b66>|0x086|0x015:'102'
	// <0x599b81>|0x0a1|0x01b:'103'
	// <0x599b96>|0x0b6|0x015:'104'
	// <0x599bab>|0x0cb|0x015:'105'
	// <0x599bc0>|0x0e0|0x015:'106'
	// <0x599bdb>|0x0fb|0x01b:'107'
	// <0x599c40>|0x160|0x065:'108'
	// ******
}

// STATE[STUB]
// bool survarium::weapon_core_shotgun_reload_state::finish_reload_predicate() const
bool weapon_core_shotgun_reload_state::finish_reload_predicate( ) const
{
	return false;
	// FUNCTION BODY
	// 1
	// 2
	// 3
	// <0x599991>|0x000|0x000:'116'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_shotgun_reload_state::execute()
void weapon_core_shotgun_reload_state::execute( )
{
	// FUNCTION BODY
	// <0x5998b7>|0x000|0x000:'121'
	// ******
}

} // namespace survarium
