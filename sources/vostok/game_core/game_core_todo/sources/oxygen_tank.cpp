////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "oxygen_tank.h"

namespace survarium {

// STATE[STUB]
// survarium::oxygen_tank::oxygen_tank()
oxygen_tank::oxygen_tank( )
{
}

// STATE[STUB]
// void survarium::oxygen_tank::~oxygen_tank()
void oxygen_tank::~oxygen_tank( )
{
	// LOCALS
	// u32 							i<1>
	// oxygen_tank::item_influence& infl<2>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6e94d1 }, type_index: TypeIndex(0x9f5a) })
	// ******

	// FUNCTION BODY
	// <0x6fa492>|0x000|0x000|[1]:'24'
	// 1
	// <0x6fa4b3>|0x021|0x021|[2]:'26'
	// <0x6fa4c5>|0x033|0x012:'27'
	// <0x6fa4d3>|0x041|0x00e:'28'
	// <0x6fa4d5>|0x043|0x002:'29'
	// ******
}

// STATE[STUB]
// void survarium::oxygen_tank::load(vostok::configs::binary_config_value)
void oxygen_tank::load( vostok::configs::binary_config_value config )
{
	// LOCALS
	// vostok::configs::binary_config_value influences
	// u32 							i<1>
	// oxygen_tank::item_influence& infl<2>
	// ******

	// SKIPPED BLOCKS
	// <0x6fa2b1><2>
	// ******

	// FUNCTION BODY
	// <0x6fa1d0>|0x000|0x000:'34'
	// 1
	// <0x6fa204>|0x034|0x034:'36'
	// 1
	// <0x6fa21c>|0x04c|0x018:'38'
	// 1
	// <0x6fa24c>|0x07c|0x030:'40'
	// <0x6fa260>|0x090|0x014:'41'
	// 1
	// 2
	// <0x6fa28f>|0x0bf|0x02f|[1]:'44'
	// 1
	// <0x6fa2b7>|0x0e7|0x028:'46'
	// <0x6fa2cc>|0x0fc|0x015:'47'
	// <0x6fa2fd>|0x12d|0x031:'48'
	// <0x6fa3b8>|0x1e8|0x0bb:'49'
	// <0x6fa3f0>|0x220|0x038:'50'
	// <0x6fa428>|0x258|0x038:'51'
	// <0x6fa44c>|0x27c|0x024:'52'
	// 1
	// 2
	// ******
}

// STATE[STUB]
// void survarium::oxygen_tank::action(bool)
void oxygen_tank::action( bool key_down )
{
	// FUNCTION BODY
	// <0x6fa087>|0x000|0x000:'59'
	// <0x6fa08f>|0x008|0x008:'60'
	// 1
	// <0x6fa091>|0x00a|0x002:'62'
	// <0x6fa0a7>|0x020|0x016:'63'
	// ******
}

// STATE[STUB]
// void survarium::oxygen_tank::set_active(bool)
void oxygen_tank::set_active( bool bactive )
{
	// LOCALS
	// u32 							i<1>
	// oxygen_tank::item_influence const& infl<2>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6e8f60 }, type_index: TypeIndex(0xa828) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6e8fb2 }, type_index: TypeIndex(0xa828) })
	// ******

	// FUNCTION BODY
	// <0x6f9dd7>|0x000|0x000:'68'
	// 1
	// <0x6f9de6>|0x00f|0x00f:'70'
	// <0x6f9dfb>|0x024|0x015:'71'
	// <0x6f9ea4>|0x0cd|0x0a9:'72'
	// <0x6f9ea6>|0x0cf|0x002:'73'
	// 1
	// <0x6f9ed0>|0x0f9|0x02a|[1]:'75'
	// 1
	// <0x6f9ef8>|0x121|0x028|[2]:'77'
	// <0x6f9f0d>|0x136|0x015:'78'
	// <0x6f9f1e>|0x147|0x011:'79'
	// <0x6f9f6e>|0x197|0x050:'80'
	// <0x6f9f70>|0x199|0x002:'81'
	// <0x6f9fc0>|0x1e9|0x050:'82'
	// 1
	// <0x6f9fc5>|0x1ee|0x005:'84'
	// ******
}

// STATE[STUB]
// void survarium::oxygen_tank::active_tick(const unsigned int)
void oxygen_tank::active_tick( u32 frame_time_ms )
{
	// FUNCTION BODY
	// <0x6fa0e1>|0x000|0x000:'89'
	// 1
	// <0x6fa0ed>|0x00c|0x00c:'91'
	// <0x6fa112>|0x031|0x025:'92'
	// 1
	// <0x6fa193>|0x0b2|0x081:'94'
	// <0x6fa1a9>|0x0c8|0x016:'95'
	// ******
}

// STATE[STUB]
// survarium::oxygen_tank::item_influence const* survarium::oxygen_tank::find_influence(char const*, char const*)
oxygen_tank::item_influence const* oxygen_tank::find_influence( pcstr body_part_name, pcstr hit_type )
{
	// LOCALS
	// u32 							i<1>
	// oxygen_tank::item_influence const& infl<2>
	// ******

	return NULL;
	// FUNCTION BODY
	// <0x6f9cf9>|0x000|0x000|[1]:'100'
	// 1
	// <0x6f9d1a>|0x021|0x021|[2]:'102'
	// 1
	// <0x6f9d2c>|0x033|0x012:'104'
	// <0x6f9d56>|0x05d|0x02a:'105'
	// <0x6f9d5b>|0x062|0x005:'106'
	// <0x6f9d5d>|0x064|0x002:'107'
	// ******
}

// STATE[STUB]
// float survarium::oxygen_tank::reduce_damage(char const*, char const*, const float, const float)
float oxygen_tank::reduce_damage(
	pcstr		body_part_name,
	pcstr		damage_type,
	float		amount,
	float		armor_piercing)
{
	// LOCALS
	// oxygen_tank::item_influence const* infl
	// ******

	return 0.0f;
	// FUNCTION BODY
	// 1
	// 2
	// <0x6f9d79>|0x000|0x000:'117'
	// 1
	// <0x6f9d8c>|0x013|0x013:'119'
	// <0x6f9d92>|0x019|0x006:'120'
	// 1
	// <0x6f9d97>|0x01e|0x005:'122'
	// <0x6f9da5>|0x02c|0x00e:'123'
	// 1
	// <0x6f9da9>|0x030|0x004:'125'
	// ******
}

// STATE[STUB]
// bool survarium::oxygen_tank::get_item_props(survarium::inventory_item_props&)
bool oxygen_tank::get_item_props( inventory_item_props& props )
{
	return false;
	// FUNCTION BODY
	// <0x6f9c69>|0x000|0x000:'130'
	// 1
	// <0x6f9c75>|0x00c|0x00c:'132'
	// 1
	// <0x6f9c84>|0x01b|0x00f:'134'
	// 1
	// <0x6f9cd9>|0x070|0x055:'136'
	// ******
}

	/* TYPEDEFS

	typedef
		scheduler::record*
		iterator_type;

} // namespace survarium
