////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\game\sources\booby_trap.h"

namespace survarium {

// STATE[STUB]
explicit booby_trap::booby_trap( game_world& gw )
{
	// FUNCTION BODY[0x5c10f0]: 1
	// <0x5c115d>|0x06d|+0x019:'23'
	// ******
}

// STATE[STUB]
 booby_trap::~booby_trap( )
{
	// FUNCTION BODY[0x5c1180]: 2
	// <0x5c1184>|0x004|+0x050:'28'
	// <0x5c11d4>|0x054|+0x01d:'29'
	// ******
}

// STATE[STUB]
void booby_trap::defuse_completed( )
{
	// CALL SITE INFO
	// <0x5c107a> -> bool < unknown >() const
	// ******

	// FUNCTION BODY[0x5c1060]: 4
	// <0x5c1063>|0x003|+0x01d:'34'
	// <0>
	// <0x5c1080>|0x020|+0x009:'36'
	// <0>
	// ******
}

// STATE[STUB]
void booby_trap::switch_to_state( booby_trap_state new_state )
{
	// CALL SITE INFO
	// <0x5c1441> -> bool < unknown >() const
	// ******

	// FUNCTION BODY[0x5c1420]: 13
	// <0>
	// <1>
	// <0x5c1423>|0x003|+0x024:'44'
	// <0>
	// <0x5c1447>|0x027|+0x00c:'46'
	// <0>
	// <0x5c1453>|0x033|+0x002:'48'
	// <0>
	// <0x5c1455>|0x035|+0x00a:'50'
	// <0>
	// <1>
	// <0x5c145f>|0x03f|+0x008:'53'
	// <0x5c1467>|0x047|+0x008:'54'
	// ******
}

// STATE[STUB]
void booby_trap::on_new_state( booby_trap_state old_state )
{
	// CALL SITE INFO
	// <0x5c13b5> -> bool < unknown >() const
	// <0x5c1402> -> bool < unknown >() const
	// ******

	// FUNCTION BODY[0x5c1310]: 35
	// <0>
	// <0x5c1310>|0x000|+0x013:'60'
	// <0>
	// <0x5c1323>|0x013|+0x014:'62'
	// <0x5c1337>|0x027|+0x025:'63'
	// <0>
	// <0x5c135c>|0x04c|+0x00f:'65'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5c136b>|0x05b|+0x02d:'71'
	// <0>
	// <1>
	// <0x5c1398>|0x088|+0x006:'74'
	// <0x5c139e>|0x08e|+0x01d:'75'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5c13bb>|0x0ab|+0x00b:'81'
	// <0x5c13c6>|0x0b6|+0x00b:'82'
	// <0x5c13d1>|0x0c1|+0x00f:'83'
	// <0x5c13e0>|0x0d0|+0x00b:'84'
	// <0>
	// <1>
	// <0x5c13eb>|0x0db|+0x01d:'87'
	// <0>
	// <0x5c1408>|0x0f8|+0x009:'89'
	// <0>
	// <0x5c1411>|0x101|+0x007:'91'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void booby_trap::on_trap_fired_message( )
{
	// CALL SITE INFO
	// <0x5c12fa> -> void < unknown >( booby_trap_state )
	// ******

	// FUNCTION BODY[0x5c12f0]: 3
	// <0x5c12f1>|0x001|+0x00b:'98'
	// <0>
	// <0x5c12fc>|0x00c|+0x008:'100'
	// ******
}

// STATE[STUB]
void booby_trap::on_trap_disarmed_message( )
{
	// CALL SITE INFO
	// <0x5c1057> -> void < unknown >( booby_trap_state )
	// ******

	// FUNCTION BODY[0x5c1050]: 1
	// <0x5c1050>|0x000|+0x009:'105'
	// ******
}

// STATE[STUB]
void booby_trap::register_tick( scheduler& scheduler )
{
	// CALL SITE INFO
	// <0x5c10da> -> bool < unknown >() const
	// ******

	// FUNCTION BODY[0x5c10c0]: 4
	// <0x5c10c3>|0x003|+0x01d:'110'
	// <0>
	// <0x5c10e0>|0x020|+0x00d:'112'
	// <0>
	// ******
}

// STATE[STUB]
void booby_trap::unregister_tick( scheduler& scheduler )
{
	// CALL SITE INFO
	// <0x5c10aa> -> bool < unknown >() const
	// ******

	// FUNCTION BODY[0x5c1090]: 4
	// <0x5c1093>|0x003|+0x01d:'118'
	// <0>
	// <0x5c10b0>|0x020|+0x00d:'120'
	// <0>
	// ******
}

// STATE[STUB]
void booby_trap::play_fired_effects( ) const
{
	// CALL SITE INFO
	// <0x5c12eb> -> void < unknown >( resources::unmanaged_resource_ptr const&, float3 const& )
	// ******

	// FUNCTION BODY[0x5c1290]: 2
	// <0x5c1293>|0x003|+0x03a:'126'
	// <0x5c12cd>|0x03d|+0x020:'127'
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
