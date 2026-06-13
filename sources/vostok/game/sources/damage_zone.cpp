////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "damage_zone.h"

namespace survarium {

// STATE[STUB]
damage_zone::damage_zone( game_world& game_world ) :
	// ref member; the same-named param is the obvious source - a matcher
	// confirms when this TU is enabled
	m_game_world( game_world )
{
	// FUNCTION BODY[0x5bcf30]
	// <0x5bcf30>|0x000|      :'18'	{
	// ******
}

// STATE[STUB]
 damage_zone::~damage_zone( )
{
	// FUNCTION BODY[0x5bcea0]: 0
	// <0x5bcea0>|0x000|+0x033:'21'	{
	// <0x5bced3>|0x033|      :'22'	}
	// ******
}

// STATE[STUB]
void damage_zone::load(
	configs::binary_config_value const&		cfg_val,
	vector< resources::unmanaged_resource_ptr >&	resources,
	vector< render::light_props >&		,	// PDB: __formal, genuinely unused
	u32										// PDB: __formal, genuinely unused (two collide - left unnamed)
)
{
	// FUNCTION BODY[0x5bce20]: 9
	// <0x5bce2a>|0x00a|+0x00c:'29'
	// <0>
	// <0x5bce36>|0x016|+0x006:'31'
	// <0x5bce3c>|0x01c|+0x014:'32'
	// <0x5bce50>|0x030|+0x045:'33'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
void damage_zone::play_particles( vector< resources::unmanaged_resource_ptr > const& particles ) const
{
	// LOCALS
	// vectora< float3 > 				shapes_centers
	// ******

	// FUNCTION BODY[0x5bd030]: 10
	// <0x5bd039>|0x009|+0x049:'62'
	// <0x5bd082>|0x052|-0x041:'62'
	// <0x5bd041>|0x011|+0x025:'63'
	// <0x5bd066>|0x036|+0x01a:'64'
	// <0>
	// <0x5bd080>|0x050|+0x070:'66'
	// <0x5bd0f0>|0x0c0|-0x06a:'66'
	// <0>
	// <0x5bd086>|0x056|+0x06c:'68'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void damage_zone::stop_particles( vector< resources::unmanaged_resource_ptr > const& particles ) const
{
	// FUNCTION BODY[0x5bcfa0]: 3
	// <0x5bcfac>|0x00c|+0x009:'76'
	// <0x5bcfb5>|0x015|+0x00d:'77'
	// <0x5bcfc2>|0x022|+0x02b:'78'
	// ******
}

// STATE[STUB]
void damage_zone::activate( zone_group* owner, physics::world* p_world, scheduler& scheduler )
{
	// FUNCTION BODY[0x5bd110]: 5
	// <0x5bd110>|0x000|+0x019:'106'
	// <0x5bd129>|0x019|+0x014:'107'
	// <0x5bd13d>|0x02d|+0x00e:'108'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void damage_zone::deactivate( )
{
	// FUNCTION BODY[0x5bd000]: 5
	// <0x5bd003>|0x003|+0x005:'115'
	// <0x5bd008>|0x008|+0x014:'116'
	// <0x5bd01c>|0x01c|+0x00e:'117'
	// <0>
	// <1>
	// ******
}


} // namespace survarium
