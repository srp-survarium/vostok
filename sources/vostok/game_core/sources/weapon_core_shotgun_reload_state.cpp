////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/weapon_core_shotgun_reload_state.h>
#include <vostok/ai/fsm.h>
#include <vostok/network_core/udp_match_packet.h>
#include <vostok/network_core/packet_reader.h>

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

// STATE[PARTIAL]: walks the internal reload substate fsm to find the current substate's
// index and appends it (u8). ASSERT( found ) compiled out.
void weapon_core_shotgun_reload_state::serialize( network_core::udp_match_packet& packet ) const
{
	u8						state_id	= 0;
	bool					found		= false;
	ai::fsm_state const*	current		= m_logic->current_state( );

	for ( ai::fsm_state const* i = m_logic->states( ).front( ); i; i = i->next )
	{
		if ( i == current )
		{
			found	= true;
			break;
		}
		++state_id;
	}

	ASSERT( UNKNOWN_EXPRESSION_T( found ) );

	packet.append( state_id );
}

// STATE[PARTIAL]: reads the target substate index, walks the substate fsm to it and
// promotes it to the initial state. ASSERT compiled out.
void weapon_core_shotgun_reload_state::deserialize( network_core::packet_reader& reader )
{
	u8						target_state_id	= reader.r< bool >( );
	u8						state_id		= 0;
	ai::fsm_state*			current			= NULL;

	for ( ai::fsm_state* i = m_logic->states( ).front( ); i; i = i->next, ++state_id )
	{
		if ( state_id == target_state_id )
		{
			current	= i;
			break;
		}
	}

	ASSERT( UNKNOWN_EXPRESSION_T( current ) );

	m_logic->set_initial_state( current );
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
