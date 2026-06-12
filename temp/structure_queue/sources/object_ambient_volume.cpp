////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\object_ambient_volume.h"

namespace survarium {

// STATE[STUB]
explicit object_ambient_volume::object_ambient_volume( base_game_scene& w )
{
	// FUNCTION BODY[0x78ea70]: 1
	// <0x78ea83>|0x013|+0x01e:'25'
	// ******
}

// STATE[STUB]
 object_ambient_volume::~object_ambient_volume( )
{
	// FUNCTION BODY[0x78ea60]: 0
	// <0x78ea60>|0x000|+0x006:'29'	{
	// <0x78ea66>|0x006|      :'30'	}
	// ******
}

// STATE[STUB]
void object_ambient_volume::load(
	configs::binary_config_value const&		t,
	pcstr									__formal,
	boost::function< void( game_object_& ) >&	cb
)
{
	// FUNCTION BODY[0x78e9d0]: 9
	// <0x78e9da>|0x00a|+0x014:'34'
	// <0>
	// <0x78e9ee>|0x01e|+0x00f:'36'
	// <0x78e9fd>|0x02d|+0x026:'37'
	// <0>
	// <0x78ea23>|0x053|+0x01e:'39'
	// <0x78ea41>|0x071|+0x007:'40'
	// <0>
	// <0x78ea48>|0x078|+0x009:'42'
	// ******
}

// STATE[STUB]
void object_ambient_volume::insert( )
{
	// LOCALS
	// render::ambient_volume_properties properties
	// ******

	// FUNCTION BODY[0x78eab0]: 9
	// <0x78eabb>|0x00b|+0x00b:'47'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x78eac6>|0x016|+0x019:'52'
	// <0x78eadf>|0x02f|+0x00a:'53'
	// <0>
	// <0x78eae9>|0x039|+0x031:'55'
	// ******
}

// STATE[STUB]
void object_ambient_volume::remove( )
{
	// FUNCTION BODY[0x78eb20]: 4
	// <0x78eb20>|0x000|+0x009:'60'
	// <0>
	// <1>
	// <0x78eb29>|0x009|+0x026:'63'
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

	// ******

} // namespace survarium
