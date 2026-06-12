////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\base_network_client.h"

namespace survarium {

// STATE[STUB]
explicit base_network_client::base_network_client( game& game )
{
	// STATICS
	// static console_commands::cc_delegate s_use_physics_controller_for_current_command = <0x4c2b3e8>;
	// ******

	// FUNCTION BODY[0x740d70]: 9
	// <0x740d70>|0x000|+0x03a:'21'
	// <0x740daa>|0x03a|+0x064:'22'
	// <0x740e0e>|0x09e|+0x05f:'23'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x740e6d>|0x0fd|+0x093:'29'
	// ******
}

// STATE[STUB]
 base_network_client::~base_network_client( )
{
	// CALL SITE INFO
	// <0x740c3a> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x740c10]: 3
	// <0x740c15>|0x005|+0x03b:'34'
	// <0x740c50>|0x040|+0x026:'35'
	// <0x740c76>|0x066|+0x026:'36'
	// ******
}

// STATE[STUB]
bool base_network_client::is_player_current( const u8 id ) const
{
	return false;

	// FUNCTION BODY[0x740960]: 1
	// <0x740960>|0x000|+0x000:'40'	{
	// <0x740960>|0x000|+0x021:'41'
	// <0x740981>|0x021|-0x003:'41'
	// <0x74097e>|0x01e|+0x005:'42'
	// <0x740983>|0x023|      :'42'	}
	// ******
}

// STATE[STUB]
game_team_id base_network_client::current_player_team( ) const
{
	// CALL SITE INFO
	// <0x74094c> -> game_team_id < unknown >() const
	// ******

	// FUNCTION BODY[0x740940]: 1
	// <0x740940>|0x000|+0x013:'46'
	// ******
}

// STATE[STUB]
player_ptr base_network_client::get_current_player( )
{
	// FUNCTION BODY[0x740d40]: 1
	// <0x740d41>|0x001|+0x02d:'51'
	// ******
}

// STATE[STUB]
float3 base_network_client::get_current_player_position( ) const
{
	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY[0x7408f0]: 1
	// <0x7408f0>|0x000|+0x03b:'56'
	// ******
}

// STATE[STUB]
void base_network_client::fill_current_player_stats( boost::function< void( u32, float, float, pcstr ) > callback )
{
	// CALL SITE INFO
	// <0x7409b7> -> damage_model_ptr const& < unknown >() const
	// <0x7409df> -> < unknown >
	// ******

	// FUNCTION BODY[0x740990]: 2
	// <0x740992>|0x002|+0x010:'61'
	// <0x7409a2>|0x012|+0x01e:'62'
	// ******
}

// STATE[STUB]
void `survarium::base_network_client::base_network_client'::`2'::`dynamic atexit destructor for 's_use_physics_controller_for_current_command''( )
{
	// FUNCTION BODY[0x7f05f0]: 12
	// <0>
	// <0x741014>|0x004|+0x01e:'68'
	// <0>
	// <1>
	// <0x741032>|0x022|+0x008:'71'
	// <0>
	// <1>
	// <0x74103a>|0x02a|+0x00f:'74'
	// <0x741049>|0x039|+0x005:'75'
	// <0x74104e>|0x03e|+0x002:'76'
	// <0>
	// <0x741050>|0x040|+0x013:'78'
	// ******
}

// STATE[STUB]
void base_network_client::attach_to_player( player_ptr player )
{
	// FUNCTION BODY[0x740f10]: 8
	// <0x740f14>|0x004|+0x010:'83'
	// <0x740f24>|0x014|+0x005:'84'
	// <0>
	// <0x740f29>|0x019|+0x043:'86'
	// <0>
	// <0x740f6c>|0x05c|+0x03a:'88'
	// <0>
	// <0x740fa6>|0x096|+0x01c:'90'
	// ******
}

// STATE[STUB]
void base_network_client::detach_from_player( )
{
	// FUNCTION BODY[0x740cd0]: 6
	// <0x740cda>|0x00a|+0x010:'95'
	// <0>
	// <0x740cea>|0x01a|+0x005:'97'
	// <0x740cef>|0x01f|+0x02a:'98'
	// <0x740d19>|0x049|+0x019:'99'
	// <0>
	// ******
}

// STATE[STUB]
void base_network_client::use_physics_controller_for_current( pcstr const arguments )
{
	// CALL SITE INFO
	// <0x740be7> -> void < unknown >()
	// <0x740c01> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x7409f0]: 28
	// <0x7409f0>|0x000|+0x00c:'104'	{
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7409fc>|0x00c|+0x1f6:'109'
	// <0x740bf2>|0x202|+0x005:'110'
	// <0>
	// <1>
	// <2>
	// <0x740bf7>|0x207|-0x1ae:'114'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x740a49>|0x059|+0x18e:'122'
	// <0x740bd7>|0x1e7|+0x00c:'123'
	// <0>
	// <1>
	// <0x740be3>|0x1f3|-0x158:'126'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x740a8b>|0x09b|+0x0a5:'131'
	// <0x740b30>|0x140|+0x09e:'132'
	// <0x740bce>|0x1de|+0x01b:'133'
	// <0x740be9>|0x1f9|+0x01a:'133'
	// <0x740c03>|0x213|      :'133'	}
	// ******
}

	// TYPEDEFS
	// typedef
	// 	survarium::base_project::resolve_link_object*
	// 	iterator_type;

	// typedef
	// 	survarium::scheduler::record*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data const*
	// 	iterator_type;

	// typedef
	// 	vostok::collision::bone_collision_data*
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_vector< std::pair< enum survarium::game_action_id, enum survarium::player_input_handler::action_state_enum >, 32 >
	// 	actions_type;

	// ******

} // namespace survarium
