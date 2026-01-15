////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/oxygen_tank.h>

namespace survarium {

// STATE[UNCHECKED]
oxygen_tank::oxygen_tank( ) :
	inventory_item	( use_silent ),
	m_active		( false ),
	m_amount_ms		( 0 ),
	m_max_amount	( 0 ),
	m_influences	( NULL ) // sushi@NOTE: Target forgot m_influences_count
{
}

// STATE[STUB]
 oxygen_tank::~oxygen_tank( )
{
	// LOCALS
	// u32 								i<1>
	// oxygen_tank::item_influence& 	infl<2>
	// ******

	// CALL SITE INFO
	// <0x6fa4d1> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x6fa480]: 6
	// <0x6fa492>|0x012|+0x021|[1]:'24'
	// <0>
	// <0x6fa4b3>|0x033|+0x012|[2]:'26'
	// <0x6fa4c5>|0x045|+0x00e:'27'
	// <0x6fa4d3>|0x053|+0x002:'28'
	// <0x6fa4d5>|0x055|+0x040:'29'
	// ******
}

// STATE[STUB]
void oxygen_tank::load( configs::binary_config_value config )
{
	// LOCALS
	// configs::binary_config_value 	influences
	// u32 								i<1>
	// oxygen_tank::item_influence& 	infl<2>
	// ******

	// SKIPPED BLOCKS
	// <0x6fa2b1><2>
	// ******

	// FUNCTION BODY[0x6fa1c0]: 21
	// <0x6fa1d0>|0x010|+0x034:'34'
	// <0>
	// <0x6fa204>|0x044|+0x018:'36'
	// <0>
	// <0x6fa21c>|0x05c|+0x030:'38'
	// <0>
	// <0x6fa24c>|0x08c|+0x014:'40'
	// <0x6fa260>|0x0a0|+0x02f:'41'
	// <0>
	// <1>
	// <0x6fa28f>|0x0cf|+0x028|[1]:'44'
	// <0>
	// <0x6fa2b7>|0x0f7|+0x015:'46'
	// <0x6fa2cc>|0x10c|+0x031:'47'
	// <0x6fa2fd>|0x13d|+0x0bb:'48'
	// <0x6fa3b8>|0x1f8|+0x038:'49'
	// <0x6fa3f0>|0x230|+0x038:'50'
	// <0x6fa428>|0x268|+0x024:'51'
	// <0x6fa44c>|0x28c|+0x029:'52'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void oxygen_tank::action( bool key_down )
{
	// FUNCTION BODY[0x6fa080]: 5
	// <0x6fa087>|0x007|+0x008:'59'
	// <0x6fa08f>|0x00f|+0x002:'60'
	// <0>
	// <0x6fa091>|0x011|+0x016:'62'
	// <0x6fa0a7>|0x027|+0x01b:'63'
	// ******
}

// STATE[STUB]
void oxygen_tank::set_active( bool bactive )
{
	// LOCALS
	// u32 								i<1>
	// oxygen_tank::item_influence const& infl<2>
	// ******

	// CALL SITE INFO
	// <0x6f9f60> -> damage_model_ptr const& < unknown >() const
	// <0x6f9fb2> -> damage_model_ptr const& < unknown >() const
	// ******

	// FUNCTION BODY[0x6f9dc0]: 17
	// <0x6f9dd7>|0x017|+0x00f:'68'
	// <0>
	// <0x6f9de6>|0x026|+0x015:'70'
	// <0x6f9dfb>|0x03b|+0x0a9:'71'
	// <0x6f9ea4>|0x0e4|+0x002:'72'
	// <0x6f9ea6>|0x0e6|+0x02a:'73'
	// <0>
	// <0x6f9ed0>|0x110|+0x028|[1]:'75'
	// <0>
	// <0x6f9ef8>|0x138|+0x015|[2]:'77'
	// <0x6f9f0d>|0x14d|+0x011:'78'
	// <0x6f9f1e>|0x15e|+0x050:'79'
	// <0x6f9f6e>|0x1ae|+0x002:'80'
	// <0x6f9f70>|0x1b0|+0x050:'81'
	// <0x6f9fc0>|0x200|+0x005:'82'
	// <0>
	// <0x6f9fc5>|0x205|+0x0a8:'84'
	// ******
}

// STATE[STUB]
void oxygen_tank::active_tick( const u32 frame_time_ms )
{
	// FUNCTION BODY[0x6fa0d0]: 7
	// <0x6fa0e1>|0x011|+0x00c:'89'
	// <0>
	// <0x6fa0ed>|0x01d|+0x025:'91'
	// <0x6fa112>|0x042|+0x081:'92'
	// <0>
	// <0x6fa193>|0x0c3|+0x016:'94'
	// <0x6fa1a9>|0x0d9|+0x00a:'95'
	// ******
}

// STATE[STUB]
oxygen_tank::item_influence const* oxygen_tank::find_influence( pcstr body_part_name, pcstr hit_type )
{
	// LOCALS
	// u32 								i<1>
	// oxygen_tank::item_influence const& infl<2>
	// ******

	return NULL;

	// FUNCTION BODY[0x6f9cf0]: 8
	// <0x6f9cf9>|0x009|+0x021|[1]:'100'
	// <0>
	// <0x6f9d1a>|0x02a|+0x012|[2]:'102'
	// <0>
	// <0x6f9d2c>|0x03c|+0x02a:'104'
	// <0x6f9d56>|0x066|+0x005:'105'
	// <0x6f9d5b>|0x06b|+0x002:'106'
	// <0x6f9d5d>|0x06d|+0x002:'107'
	// ******
}

// STATE[STUB]
float oxygen_tank::reduce_damage(
	pcstr			body_part_name,
	pcstr			damage_type,
	const float		amount,
	const float		armor_piercing
)
{
	// LOCALS
	// oxygen_tank::item_influence const* infl
	// ******

	return 0.0f;

	// FUNCTION BODY[0x6f9d70]: 11
	// <0>
	// <1>
	// <0x6f9d79>|0x009|+0x013:'117'
	// <0>
	// <0x6f9d8c>|0x01c|+0x006:'119'
	// <0x6f9d92>|0x022|+0x005:'120'
	// <0>
	// <0x6f9d97>|0x027|+0x00e:'122'
	// <0x6f9da5>|0x035|+0x004:'123'
	// <0>
	// <0x6f9da9>|0x039|+0x00f:'125'
	// ******
}

// STATE[STUB]
bool oxygen_tank::get_item_props( inventory_item_props& props )
{
	return false;

	// FUNCTION BODY[0x6f9c60]: 7
	// <0x6f9c69>|0x009|+0x00c:'130'
	// <0>
	// <0x6f9c75>|0x015|+0x00f:'132'
	// <0>
	// <0x6f9c84>|0x024|+0x055:'134'
	// <0>
	// <0x6f9cd9>|0x079|+0x009:'136'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	survarium::scheduler::record*
	// 	iterator_type;

	// ******

} // namespace survarium