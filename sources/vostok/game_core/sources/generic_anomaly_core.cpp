////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/generic_anomaly_core.h>
#include <vostok/game_core/anomaly_state.h>
#include <vostok/game_core/zone_group.h>

#include <vostok/game_core/scheduler.h>

namespace survarium {

// STATE[SKIPPED]
generic_anomaly_core::generic_anomaly_core( ):
	m_artefact_grab_time_ms		( 0 ),
	m_current_state				( NULL ),
	m_was_zone_trigger_event	( false ),
	m_was_shoot_trigger_event	( false ),
	m_physics_world				( NULL ),
	m_scheduler					( NULL )
{
}

// STATE[SKIPPED]
generic_anomaly_core::~generic_anomaly_core( )
{
}

// STATE[STUB]
void generic_anomaly_core::activate( physics::world* world, survarium::scheduler& scheduler )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
	m_physics_world = world;
	m_scheduler = &scheduler;
	m_scheduler->register_on_frame( &m_scheduler_identifier, boost::bind( &generic_anomaly_core::tick, this, _1, _2 ), true ); // sushi@TODO: The symbols in target are insane.

	for ( u32 a = 0 ; a < m_artefact_containers.size( ) ; ++a )
	{
		ASSERT( UNKNOWN_EXPRESSION );
		// m_artefact_containers[a]->
	}

	// LOCALS
	// u32 							a<1>
	// ******

	// FUNCTION BODY
	// <0x59dc1a>|0x00a|+0x00c:'28'
	// <0x59dc26>|0x016|+0x00c:'29'
	// <0x59dc32>|0x022|+0x009:'30'
	// <0x59dc3b>|0x02b|+0x009:'31'
	// <0x59dc44>|0x034|+0x0ce:'32'
	// <0>
	// <0x59dd12>|0x102|+0x022|[1]:'34'
	// <0>
	// <0x59dd34>|0x124|+0x050:'36'
	// <0x59dd84>|0x174|+0x002:'37'
	// <0>
	// <0x59dd86>|0x176|+0x007:'39'
	// <0x59dd8d>|0x17d|+0x007:'40'
	// <0x59dd94>|0x184|+0x008:'41'
	// ******
}

// STATE[STUB]
// void survarium::generic_anomaly_core::deactivate()
void generic_anomaly_core::deactivate( )
{
	// LOCALS
	// u32 							a<1>
	// ******

	// CALL SITE INFO
	// <0x59d869> -> void <unknown>()
	// ******

	// FUNCTION BODY
	// <0x59d7c0>|0x000|+0x009:'45'	{
	// <0x59d7c9>|0x009|+0x00c:'46'
	// <0x59d7d5>|0x015|+0x012:'47'
	// <0x59d7e7>|0x027|+0x009:'48'
	// <0>
	// <0x59d7f0>|0x030|+0x00b:'50'
	// <0x59d7fb>|0x03b|+0x00a:'51'
	// <0>
	// <0x59d805>|0x045|+0x022|[1]:'53'
	// <0x59d827>|0x067|+0x046:'54'
	// <0>
	// <0x59d86d>|0x0ad|+0x00a:'56'
	// <0x59d877>|0x0b7|      :'57'	}
	// ******
}

// STATE[STUB]
// void survarium::generic_anomaly_core::inc_energy(float)
void generic_anomaly_core::inc_energy( float amount )
{
	// FUNCTION BODY
	// <0x59d710>|0x000|+0x009:'60'	{
	// <0x59d719>|0x009|+0x00c:'61'
	// <0x59d725>|0x015|+0x00b:'62'
	// <0>
	// <0x59d730>|0x020|+0x015:'64'
	// <0x59d745>|0x035|+0x00c:'65'
	// <0>
	// <0x59d751>|0x041|      :'67'	}
	// ******
}

// STATE[STUB]
// void survarium::generic_anomaly_core::dec_energy(float)
void generic_anomaly_core::dec_energy( float amount )
{
	// FUNCTION BODY
	// <0x59d760>|0x000|+0x009:'69'	{
	// <0x59d769>|0x009|+0x00c:'70'
	// <0x59d775>|0x015|+0x00b:'71'
	// <0>
	// <0x59d780>|0x020|+0x026:'73'
	// <0x59d7a6>|0x046|+0x00c:'74'
	// <0>
	// <0x59d7b2>|0x052|      :'76'	}
	// ******
}

// STATE[STUB]
// void survarium::generic_anomaly_core::tick(const unsigned int, const unsigned int)
void generic_anomaly_core::tick( u32 time_delta_ms, u32 current_time_ms )
{
	// LOCALS
	// anomaly_state* 				state
	// ******

	// FUNCTION BODY
	// <0x59db30>|0x000|+0x009:'79'	{
	// <0x59db39>|0x009|+0x009:'80'
	// <0x59db42>|0x012|+0x037:'81'
	// <0>
	// <0x59db79>|0x049|+0x00b:'83'
	// <0x59db84>|0x054|+0x00c:'84'
	// <0>
	// <0x59db90>|0x060|+0x00b:'86'
	// <0>
	// <0x59db9b>|0x06b|+0x009:'88'
	// <0x59dba4>|0x074|+0x00b:'89'
	// <0x59dbaf>|0x07f|+0x009:'90'
	// <0x59dbb8>|0x088|+0x00b:'91'
	// <0>
	// <0x59dbc3>|0x093|+0x013:'93'
	// <0>
	// <0x59dbd6>|0x0a6|+0x020:'95'
	// <0x59dbf6>|0x0c6|+0x008:'96'
	// <0x59dbfe>|0x0ce|      :'97'	}
	// ******
}

// STATE[STUB]
// void survarium::generic_anomaly_core::spawn_artefacts()
void generic_anomaly_core::spawn_artefacts( )
{
	// LOCALS
	// u32 							respawn_cnt
	// buffer_vector<artefact_container_core *> empty_artefact_containers
	// u32 							cont_total
	// u32 							artefacts_current
	// u32 							a<1>
	// u32 							i<2>
	// ******

	// FUNCTION BODY
	// <0x59d980>|0x000|+0x009:'100'	{
	// <0x59d989>|0x009|+0x00c:'101'
	// <0x59d995>|0x015|+0x00e:'102'
	// <0>
	// <0x59d9a3>|0x023|+0x036:'104'
	// <0>
	// <0x59d9d9>|0x059|+0x01e|[1]:'106'
	// <0>
	// <0x59d9f7>|0x077|+0x05a:'108'
	// <0x59da51>|0x0d1|+0x035:'109'
	// <0x59da86>|0x106|+0x005:'110'
	// <0>
	// <0x59da8b>|0x10b|+0x011:'112'
	// <0x59da9c>|0x11c|+0x00c:'113'
	// <0>
	// <0x59daa8>|0x128|+0x002:'115'
	// <0>
	// <0x59daaa>|0x12a|+0x016:'117'
	// <0>
	// <0x59dac0>|0x140|+0x01a|[2]:'119'
	// <0x59dada>|0x15a|+0x01c:'120'
	// <0>
	// <1>
	// <0x59daf6>|0x176|+0x00a:'123'
	// <0x59db00>|0x180|      :'124'	}
	// ******
}

// STATE[STUB]
// survarium::anomaly_state* survarium::generic_anomaly_core::select_state()
anomaly_state* generic_anomaly_core::select_state( )
{
	// LOCALS
	// anomaly_state* 				state
	// u32 							i<1>
	// anomaly_state* 				s<2>
	// ******

	// SKIPPED BLOCKS
	// <0x59d65b><2>
	// ******

	return NULL;
	// FUNCTION BODY
	// <0x59d600>|0x000|+0x009:'127'	{
	// <0x59d609>|0x009|+0x032:'128'
	// <0>
	// <0x59d63b>|0x03b|+0x026|[1]:'130'
	// <0>
	// <0x59d661>|0x061|+0x031:'132'
	// <0x59d692>|0x092|+0x00a:'133'
	// <0x59d69c>|0x09c|+0x002:'134'
	// <0>
	// <0x59d69e>|0x09e|+0x021:'136'
	// <0x59d6bf>|0x0bf|+0x002:'137'
	// <0x59d6c1>|0x0c1|+0x016:'138'
	// <0x59d6d7>|0x0d7|+0x005:'139'
	// <0x59d6dc>|0x0dc|+0x016:'140'
	// <0x59d6f2>|0x0f2|+0x005:'141'
	// <0>
	// <0x59d6f7>|0x0f7|+0x006:'143'
	// <0x59d6fd>|0x0fd|+0x005:'144'
	// <0>
	// <0x59d702>|0x102|+0x003:'146'
	// <0x59d705>|0x105|      :'147'	}
	// ******
}

// STATE[STUB]
// void survarium::anomaly_state::initialize()
void anomaly_state::initialize( )
{
	// LOCALS
	// u32 							g<1>
	// ******

	// FUNCTION BODY
	// <0x59d560>|0x000|+0x009:'150'	{
	// <0x59d569>|0x009|+0x022|[1]:'151'
	// <0x59d58b>|0x02b|+0x03b:'152'
	// <0>
	// <0x59d5c6>|0x066|+0x009:'154'
	// <0x59d5cf>|0x06f|+0x021:'155'
	// <0x59d5f0>|0x090|+0x002:'156'
	// <0x59d5f2>|0x092|+0x00a:'157'
	// <0x59d5fc>|0x09c|      :'158'	}
	// ******
}

// STATE[STUB]
// void survarium::anomaly_state::execute(const unsigned int, const unsigned int)
void anomaly_state::execute( u32 time_delta_ms, u32 current_time_ms )
{
	// LOCALS
	// u32 							g<1>
	// ******

	// FUNCTION BODY
	// <0x59d4a0>|0x000|+0x009:'161'	{
	// <0x59d4a9>|0x009|+0x022|[1]:'162'
	// <0x59d4cb>|0x02b|+0x043:'163'
	// <0>
	// <0x59d50e>|0x06e|+0x014:'165'
	// <0x59d522>|0x082|+0x02c:'166'
	// <0x59d54e>|0x0ae|      :'167'	}
	// ******
}

// STATE[STUB]
// void survarium::anomaly_state::finalize()
void anomaly_state::finalize( )
{
	// LOCALS
	// u32 							g<1>
	// ******

	// FUNCTION BODY
	// <0x59d430>|0x000|+0x009:'170'	{
	// <0x59d439>|0x009|+0x022|[1]:'171'
	// <0x59d45b>|0x02b|+0x03b:'172'
	// <0x59d496>|0x066|      :'173'	}
	// ******
}

// STATE[STUB]
// void survarium::zone_group::initialize()
void zone_group::initialize( )
{
	// FUNCTION BODY
	// <0x59d410>|0x000|+0x007:'176'	{
	// <0x59d417>|0x007|+0x00a:'177'
	// <0x59d421>|0x011|+0x008:'178'
	// <0x59d429>|0x019|      :'179'	}
	// ******
}

// STATE[STUB]
// void survarium::zone_group::execute(const unsigned int, const unsigned int)
void zone_group::execute( u32 time_delta_ms, u32 current_time_ms )
{
	// LOCALS
	// u32 							z<1>
	// ******

	// CALL SITE INFO
	// <0x59d3fb> -> void <unknown>(const u32, const u32)
	// ******

	// FUNCTION BODY
	// <0x59d350>|0x000|+0x009:'182'	{
	// <0x59d359>|0x009|+0x014:'183'
	// <0x59d36d>|0x01d|+0x008:'184'
	// <0>
	// <0x59d375>|0x025|+0x02e|[1]:'186'
	// <0>
	// <0x59d3a3>|0x053|+0x024:'188'
	// <0x59d3c7>|0x077|+0x036:'189'
	// <0x59d3fd>|0x0ad|+0x005:'190'
	// <0x59d402>|0x0b2|      :'191'	}
	// ******
}

// STATE[STUB]
void zone_group::finalize( )
{
	// LOCALS
	// u32 							z<1>
	// ******

	// CALL SITE INFO
	// <0x59d317> -> void <unknown>()
	// ******

	// FUNCTION BODY
	// <0x59d290>|0x000|+0x009:'194'	{
	// <0x59d299>|0x009|+0x02e|[1]:'195'
	// <0>
	// <0x59d2c7>|0x037|+0x024:'197'
	// <0>
	// <0x59d2eb>|0x05b|+0x02e:'199'
	// <0x59d319>|0x089|+0x020:'200'
	// <0x59d339>|0x0a9|+0x005:'201'
	// <0>
	// <0x59d33e>|0x0ae|      :'203'	}
	// ******
}

// STATE[STUB]
void zone_group::recharge( )
{
	// LOCALS
	// u32 							z<1>
	// ******

	// CALL SITE INFO
	// <0x59d255> -> void <unknown>(zone_group*, physics::world*, scheduler&)
	// ******

	// FUNCTION BODY
	// <0x59d190>|0x000|+0x009:'206'	{
	// <0x59d199>|0x009|+0x032|[1]:'207'
	// <0>
	// <0x59d1cb>|0x03b|+0x028:'209'
	// <0>
	// <0x59d1f3>|0x063|+0x064:'211'
	// <0x59d257>|0x0c7|+0x020:'212'
	// <0>
	// <1>
	// <0x59d277>|0x0e7|+0x005:'215'
	// <0x59d27c>|0x0ec|+0x00a:'216'
	// <0x59d286>|0x0f6|      :'217'	}
	// ******
}

// STATE[STUB]
// void survarium::zone_group::on_zone_act(survarium::damage_zone_core*, survarium::hit_receiver*)
void zone_group::on_zone_act( damage_zone_core* zone, hit_receiver* receiver )
{
	// LOCALS
	// u32 							z<1>
	// ******

	// CALL SITE INFO
	// <0x59d136> -> void <unknown>()
	// ******

	// FUNCTION BODY
	// <0x59d080>|0x000|+0x009:'220'	{
	// <0>
	// <0x59d089>|0x009|+0x01c:'222'
	// <0>
	// <0x59d0a5>|0x025|+0x032|[1]:'224'
	// <0>
	// <0x59d0d7>|0x057|+0x027:'226'
	// <0>
	// <0x59d0fe>|0x07e|+0x00c:'228'
	// <0x59d10a>|0x08a|+0x02e:'229'
	// <0x59d138>|0x0b8|+0x020:'230'
	// <0x59d158>|0x0d8|+0x02a:'231'
	// <0>
	// <0x59d182>|0x102|+0x002:'233'
	// <0x59d184>|0x104|+0x005:'234'
	// <0>
	// <1>
	// <0x59d189>|0x109|      :'237'	}
	// ******
}

} // namespace survarium
