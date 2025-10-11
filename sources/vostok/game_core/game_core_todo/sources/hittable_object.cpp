////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "hittable_object.h"

namespace survarium {

// STATE[STUB]
// survarium::hittable_object::hittable_object()
hittable_object::hittable_object( )
{
}

// STATE[STUB]
// void survarium::hittable_object::~hittable_object()
void hittable_object::~hittable_object( )
{
	// FUNCTION BODY
	// <0x599ef3>|0x000|0x000:'24'
	// <0x599eff>|0x00c|0x00c:'25'
	// 1
	// <0x599f0b>|0x018|0x00c:'27'
	// ******
}

// STATE[STUB]
// void survarium::hittable_object::load(vostok::configs::binary_config_value const&)
void hittable_object::load( vostok::configs::binary_config_value const& cfg_val )
{
	// LOCALS
	// vostok::math::float4x4 		transform
	// vostok::resources::resource_ptr<vostok::physics::bt_collision_shape,vostok::resources::unmanaged_intrusive_base> shape
	// vostok::physics::bt_rigid_body_construction_info info
	// vostok::configs::binary_config_value meshes
	// pcstr 						name
	// vostok::math::float3 const& 	rotation
	// vostok::math::float3 const& 	scale
	// vostok::math::float3 const& 	position
	// ******

	// FUNCTION BODY
	// <0x599f91>|0x000|0x000:'32'
	// <0x599fa3>|0x012|0x012:'33'
	// <0x599fb5>|0x024|0x012:'34'
	// <0x599fc7>|0x036|0x012:'35'
	// <0x599fd9>|0x048|0x012:'36'
	// <0x599feb>|0x05a|0x012:'37'
	// <0x599ffd>|0x06c|0x012:'38'
	// 1
	// <0x59a00f>|0x07e|0x012:'40'
	// <0x59a024>|0x093|0x015:'41'
	// <0x59a039>|0x0a8|0x015:'42'
	// <0x59a04e>|0x0bd|0x015:'43'
	// <0x59a063>|0x0d2|0x015:'44'
	// 1
	// <0x59a0b9>|0x128|0x056:'46'
	// <0x59a0e9>|0x158|0x030:'47'
	// <0x59a11b>|0x18a|0x032:'48'
	// 1
	// 2
	// <0x59a12a>|0x199|0x00f:'51'
	// <0x59a132>|0x1a1|0x008:'52'
	// 1
	// <0x59a13e>|0x1ad|0x00c:'54'
	// 1
	// <0x59a14f>|0x1be|0x011:'56'
	// <0x59a16b>|0x1da|0x01c:'57'
	// 1
	// <0x59a187>|0x1f6|0x01c:'59'
	// <0x59a199>|0x208|0x012:'60'
	// <0x59a1ab>|0x21a|0x012:'61'
	// ******
}

// STATE[STUB]
// void survarium::hittable_object::set_transform(vostok::math::float4x4 const&)
void hittable_object::set_transform( vostok::math::float4x4 const& transform )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x588e1c }, type_index: TypeIndex(0x94e2) })
	// ******

	// FUNCTION BODY
	// <0x599e07>|0x000|0x000:'66'
	// ******
}

// STATE[STUB]
// void survarium::hittable_object::insert(vostok::physics::world*)
void hittable_object::insert( vostok::physics::world* world )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x588ed8 }, type_index: TypeIndex(0x9464) })
	// ******

	// FUNCTION BODY
	// <0x599e89>|0x000|0x000:'76'
	// <0x599e95>|0x00c|0x00c:'77'
	// 1
	// <0x599ea1>|0x018|0x00c:'79'
	// <0x599eaa>|0x021|0x009:'80'
	// <0x599eb6>|0x02d|0x00c:'81'
	// ******
}

// STATE[STUB]
// void survarium::hittable_object::remove()
void hittable_object::remove( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x588e69 }, type_index: TypeIndex(0x9467) })
	// ******

	// FUNCTION BODY
	// <0x599e39>|0x000|0x000:'86'
	// <0x599e45>|0x00c|0x00c:'87'
	// 1
	// <0x599e51>|0x018|0x00c:'89'
	// <0x599e6b>|0x032|0x01a:'90'
	// ******
}

} // namespace survarium
