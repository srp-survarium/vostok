////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "generic_anomaly_core.h"

namespace survarium {

// STATE[STUB]
// survarium::generic_anomaly_core::generic_anomaly_core()
generic_anomaly_core::generic_anomaly_core( )
{
}

// STATE[STUB]
// void survarium::generic_anomaly_core::~generic_anomaly_core()
void generic_anomaly_core::~generic_anomaly_core( )
{
}

// STATE[STUB]
// void survarium::generic_anomaly_core::activate(vostok::physics::world*, survarium::scheduler&)
void generic_anomaly_core::activate( vostok::physics::world* world, scheduler& scheduler )
{
	// LOCALS
	// u32 							a<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58cd82 }, type_index: TypeIndex(0x1ec7f) })
	// ******

	// FUNCTION BODY
	// <0x59dc1a>|0x000|0x000:'28'
	// <0x59dc26>|0x00c|0x00c:'29'
	// <0x59dc32>|0x018|0x00c:'30'
	// <0x59dc3b>|0x021|0x009:'31'
	// <0x59dc44>|0x02a|0x009:'32'
	// 1
	// <0x59dd12>|0x0f8|0x0ce|[1]:'34'
	// 1
	// <0x59dd34>|0x11a|0x022:'36'
	// <0x59dd84>|0x16a|0x050:'37'
	// 1
	// <0x59dd86>|0x16c|0x002:'39'
	// <0x59dd8d>|0x173|0x007:'40'
	// <0x59dd94>|0x17a|0x007:'41'
	// ******
}

// STATE[STUB]
// void survarium::generic_anomaly_core::deactivate()
void generic_anomaly_core::deactivate( )
{
	// LOCALS
	// u32 							a<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58c869 }, type_index: TypeIndex(0x1ec7d) })
	// ******

	// FUNCTION BODY
	// <0x59d7c9>|0x000|0x000:'46'
	// <0x59d7d5>|0x00c|0x00c:'47'
	// <0x59d7e7>|0x01e|0x012:'48'
	// 1
	// <0x59d7f0>|0x027|0x009:'50'
	// <0x59d7fb>|0x032|0x00b:'51'
	// 1
	// <0x59d805>|0x03c|0x00a|[1]:'53'
	// <0x59d827>|0x05e|0x022:'54'
	// 1
	// <0x59d86d>|0x0a4|0x046:'56'
	// ******
}

// STATE[STUB]
// void survarium::generic_anomaly_core::inc_energy(float)
void generic_anomaly_core::inc_energy( float amount )
{
	// FUNCTION BODY
	// <0x59d719>|0x000|0x000:'61'
	// <0x59d725>|0x00c|0x00c:'62'
	// 1
	// <0x59d730>|0x017|0x00b:'64'
	// <0x59d745>|0x02c|0x015:'65'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::generic_anomaly_core::dec_energy(float)
void generic_anomaly_core::dec_energy( float amount )
{
	// FUNCTION BODY
	// <0x59d769>|0x000|0x000:'70'
	// <0x59d775>|0x00c|0x00c:'71'
	// 1
	// <0x59d780>|0x017|0x00b:'73'
	// <0x59d7a6>|0x03d|0x026:'74'
	// 1
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
	// <0x59db39>|0x000|0x000:'80'
	// <0x59db42>|0x009|0x009:'81'
	// 1
	// <0x59db79>|0x040|0x037:'83'
	// <0x59db84>|0x04b|0x00b:'84'
	// 1
	// <0x59db90>|0x057|0x00c:'86'
	// 1
	// <0x59db9b>|0x062|0x00b:'88'
	// <0x59dba4>|0x06b|0x009:'89'
	// <0x59dbaf>|0x076|0x00b:'90'
	// <0x59dbb8>|0x07f|0x009:'91'
	// 1
	// <0x59dbc3>|0x08a|0x00b:'93'
	// 1
	// <0x59dbd6>|0x09d|0x013:'95'
	// <0x59dbf6>|0x0bd|0x020:'96'
	// ******
}

// STATE[STUB]
// void survarium::generic_anomaly_core::spawn_artefacts()
void generic_anomaly_core::spawn_artefacts( )
{
	// LOCALS
	// u32 							respawn_cnt
	// vostok::buffer_vector<artefact_container_core *> empty_artefact_containers
	// u32 							cont_total
	// u32 							artefacts_current
	// u32 							a<1>
	// u32 							i<2>
	// ******

	// FUNCTION BODY
	// <0x59d989>|0x000|0x000:'101'
	// <0x59d995>|0x00c|0x00c:'102'
	// 1
	// <0x59d9a3>|0x01a|0x00e:'104'
	// 1
	// <0x59d9d9>|0x050|0x036|[1]:'106'
	// 1
	// <0x59d9f7>|0x06e|0x01e:'108'
	// <0x59da51>|0x0c8|0x05a:'109'
	// <0x59da86>|0x0fd|0x035:'110'
	// 1
	// <0x59da8b>|0x102|0x005:'112'
	// <0x59da9c>|0x113|0x011:'113'
	// 1
	// <0x59daa8>|0x11f|0x00c:'115'
	// 1
	// <0x59daaa>|0x121|0x002:'117'
	// 1
	// <0x59dac0>|0x137|0x016|[2]:'119'
	// <0x59dada>|0x151|0x01a:'120'
	// 1
	// 2
	// <0x59daf6>|0x16d|0x01c:'123'
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
	// <0x59d609>|0x000|0x000:'128'
	// 1
	// <0x59d63b>|0x032|0x032|[1]:'130'
	// 1
	// <0x59d661>|0x058|0x026:'132'
	// <0x59d692>|0x089|0x031:'133'
	// <0x59d69c>|0x093|0x00a:'134'
	// 1
	// <0x59d69e>|0x095|0x002:'136'
	// <0x59d6bf>|0x0b6|0x021:'137'
	// <0x59d6c1>|0x0b8|0x002:'138'
	// <0x59d6d7>|0x0ce|0x016:'139'
	// <0x59d6dc>|0x0d3|0x005:'140'
	// <0x59d6f2>|0x0e9|0x016:'141'
	// 1
	// <0x59d6f7>|0x0ee|0x005:'143'
	// <0x59d6fd>|0x0f4|0x006:'144'
	// 1
	// <0x59d702>|0x0f9|0x005:'146'
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
	// <0x59d569>|0x000|0x000|[1]:'151'
	// <0x59d58b>|0x022|0x022:'152'
	// 1
	// <0x59d5c6>|0x05d|0x03b:'154'
	// <0x59d5cf>|0x066|0x009:'155'
	// <0x59d5f0>|0x087|0x021:'156'
	// <0x59d5f2>|0x089|0x002:'157'
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
	// <0x59d4a9>|0x000|0x000|[1]:'162'
	// <0x59d4cb>|0x022|0x022:'163'
	// 1
	// <0x59d50e>|0x065|0x043:'165'
	// <0x59d522>|0x079|0x014:'166'
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
	// <0x59d439>|0x000|0x000|[1]:'171'
	// <0x59d45b>|0x022|0x022:'172'
	// ******
}

// STATE[STUB]
// void survarium::zone_group::initialize()
void zone_group::initialize( )
{
	// FUNCTION BODY
	// <0x59d417>|0x000|0x000:'177'
	// <0x59d421>|0x00a|0x00a:'178'
	// ******
}

// STATE[STUB]
// void survarium::zone_group::execute(const unsigned int, const unsigned int)
void zone_group::execute( u32 time_delta_ms, u32 current_time_ms )
{
	// LOCALS
	// u32 							z<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58c3fb }, type_index: TypeIndex(0x105db) })
	// ******

	// FUNCTION BODY
	// <0x59d359>|0x000|0x000:'183'
	// <0x59d36d>|0x014|0x014:'184'
	// 1
	// <0x59d375>|0x01c|0x008|[1]:'186'
	// 1
	// <0x59d3a3>|0x04a|0x02e:'188'
	// <0x59d3c7>|0x06e|0x024:'189'
	// <0x59d3fd>|0x0a4|0x036:'190'
	// ******
}

// STATE[STUB]
// void survarium::zone_group::finalize()
void zone_group::finalize( )
{
	// LOCALS
	// u32 							z<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58c317 }, type_index: TypeIndex(0x105da) })
	// ******

	// FUNCTION BODY
	// <0x59d299>|0x000|0x000|[1]:'195'
	// 1
	// <0x59d2c7>|0x02e|0x02e:'197'
	// 1
	// <0x59d2eb>|0x052|0x024:'199'
	// <0x59d319>|0x080|0x02e:'200'
	// <0x59d339>|0x0a0|0x020:'201'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::zone_group::recharge()
void zone_group::recharge( )
{
	// LOCALS
	// u32 							z<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58c255 }, type_index: TypeIndex(0x105de) })
	// ******

	// FUNCTION BODY
	// <0x59d199>|0x000|0x000|[1]:'207'
	// 1
	// <0x59d1cb>|0x032|0x032:'209'
	// 1
	// <0x59d1f3>|0x05a|0x028:'211'
	// <0x59d257>|0x0be|0x064:'212'
	// 1
	// 2
	// <0x59d277>|0x0de|0x020:'215'
	// <0x59d27c>|0x0e3|0x005:'216'
	// ******
}

// STATE[STUB]
// void survarium::zone_group::on_zone_act(survarium::damage_zone_core*, survarium::hit_receiver*)
void zone_group::on_zone_act( damage_zone_core* zone, hit_receiver* receiver )
{
	// LOCALS
	// u32 							z<1>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58c136 }, type_index: TypeIndex(0x105da) })
	// ******

	// FUNCTION BODY
	// 1
	// <0x59d089>|0x000|0x000:'222'
	// 1
	// <0x59d0a5>|0x01c|0x01c|[1]:'224'
	// 1
	// <0x59d0d7>|0x04e|0x032:'226'
	// 1
	// <0x59d0fe>|0x075|0x027:'228'
	// <0x59d10a>|0x081|0x00c:'229'
	// <0x59d138>|0x0af|0x02e:'230'
	// <0x59d158>|0x0cf|0x020:'231'
	// 1
	// <0x59d182>|0x0f9|0x02a:'233'
	// <0x59d184>|0x0fb|0x002:'234'
	// 1
	// 2
	// ******
}

	/* TYPEDEFS

	typedef
		scheduler::record*
		iterator_type;

	typedef
		void**
		iterator_type;

	typedef
		zone_group::zone_wrapper*
		iterator_type;

} // namespace survarium
