////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/hittable_object.h>

namespace survarium {

// STATE[STUB]
// survarium::hittable_object::hittable_object()
hittable_object::hittable_object( )
{
	// FUNCTION BODY
	// <0x599f30>|0x000|+0x041:'19'	{
	// <0x599f71>|0x041|      :'20'	}
	// ******
}

// STATE[STUB]
// void survarium::hittable_object::~hittable_object()
hittable_object::~hittable_object( )
{
	// FUNCTION BODY
	// <0x599ef3>|0x013|+0x00c:'24'
	// <0x599eff>|0x01f|+0x00c:'25'
	// <0>
	// <0x599f0b>|0x02b|+0x00b:'27'
	// ******
}

// STATE[STUB]
// void survarium::hittable_object::load(vostok::configs::binary_config_value const&)
void hittable_object::load( configs::binary_config_value const& cfg_val )
{
	// LOCALS
	// float4x4 					transform
	// resources::resource_ptr<physics::bt_collision_shape,resources::unmanaged_intrusive_base> shape
	// physics::bt_rigid_body_construction_info info
	// configs::binary_config_value meshes
	// pcstr 						name
	// float3 const& 				rotation
	// float3 const& 				scale
	// float3 const& 				position
	// ******

	// FUNCTION BODY
	// <0x599f91>|0x011|+0x012:'32'
	// <0x599fa3>|0x023|+0x012:'33'
	// <0x599fb5>|0x035|+0x012:'34'
	// <0x599fc7>|0x047|+0x012:'35'
	// <0x599fd9>|0x059|+0x012:'36'
	// <0x599feb>|0x06b|+0x012:'37'
	// <0x599ffd>|0x07d|+0x012:'38'
	// <0>
	// <0x59a00f>|0x08f|+0x015:'40'
	// <0x59a024>|0x0a4|+0x015:'41'
	// <0x59a039>|0x0b9|+0x015:'42'
	// <0x59a04e>|0x0ce|+0x015:'43'
	// <0x59a063>|0x0e3|+0x056:'44'
	// <0>
	// <0x59a0b9>|0x139|+0x030:'46'
	// <0x59a0e9>|0x169|+0x032:'47'
	// <0x59a11b>|0x19b|+0x00f:'48'
	// <0>
	// <1>
	// <0x59a12a>|0x1aa|+0x008:'51'
	// <0x59a132>|0x1b2|+0x00c:'52'
	// <0>
	// <0x59a13e>|0x1be|+0x011:'54'
	// <0>
	// <0x59a14f>|0x1cf|+0x01c:'56'
	// <0x59a16b>|0x1eb|+0x01c:'57'
	// <0>
	// <0x59a187>|0x207|+0x012:'59'
	// <0x59a199>|0x219|+0x012:'60'
	// <0x59a1ab>|0x22b|+0x012:'61'
	// ******
}

// STATE[STUB]
// void survarium::hittable_object::set_transform(vostok::math::float4x4 const&)
void hittable_object::set_transform( float4x4 const& transform )
{
	// CALL SITE INFO
	// <0x599e1c> -> void <unknown>(float4x4 const&)
	// ******

	// FUNCTION BODY
	// <0x599e07>|0x007|+0x017:'66'
	// ******
}

// STATE[STUB]
// void survarium::hittable_object::insert(vostok::physics::world*)
void hittable_object::insert( physics::world* world )
{
	// CALL SITE INFO
	// <0x599ed8> -> void <unknown>(physics::bt_rigid_body_base*, u16, u16)
	// ******

	// FUNCTION BODY
	// <0x599e89>|0x009|+0x00c:'76'
	// <0x599e95>|0x015|+0x00c:'77'
	// <0>
	// <0x599ea1>|0x021|+0x009:'79'
	// <0x599eaa>|0x02a|+0x00c:'80'
	// <0x599eb6>|0x036|+0x024:'81'
	// ******
}

// STATE[STUB]
// void survarium::hittable_object::remove()
void hittable_object::remove( )
{
	// CALL SITE INFO
	// <0x599e69> -> void <unknown>(physics::bt_rigid_body_base*)
	// ******

	// FUNCTION BODY
	// <0x599e39>|0x009|+0x00c:'86'
	// <0x599e45>|0x015|+0x00c:'87'
	// <0>
	// <0x599e51>|0x021|+0x01a:'89'
	// <0x599e6b>|0x03b|+0x00a:'90'
	// ******
}

} // namespace survarium
