////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.08.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/physics/character_controller.h>

namespace vostok {
namespace physics {

// STATE[STUB]
bt_character_controller* create_character_controller( vostok::memory::base_allocator& allocator, world* w )
{
	return NULL;
	// FUNCTION BODY
	// <0x586866>|0x000|0x000:'23'
	// ******
}

// STATE[STUB]
bt_character_controller::bt_character_controller( world* w )
{
	// FUNCTION BODY
	// <0x586854>|0x000|0x000:'29'
	// ******
}

// STATE[STUB]
bt_character_controller::~bt_character_controller( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x575831 }, type_index: TypeIndex(0x20fb5) })
	// ******

	// FUNCTION BODY

	// <0x586810>|0x000|0x000:'35'
	// ******
}


// STATE[STUB]
// vostok::memory::base_allocator& vostok::physics::bt_character_controller::allocator()
memory::base_allocator& bt_character_controller::allocator( )
{
	// FUNCTION BODY
	// <0x586800>|0x000|0x000:'40'
	// ******
}


// STATE[STUB]
void bt_character_controller::initialize( )
{
	// FUNCTION BODY






	// <0x5869d8>|0x000|0x000:'51'


	// <0x5869fd>|0x025|0x025:'54'






	// <0x586a17>|0x03f|0x01a:'61'
	// ******
}

// STATE[STUB]
void bt_character_controller::activate( float4x4 const& t )
{
	// FUNCTION BODY

	// <0x586acd>|0x000|0x000:'68'
	// <0x586adb>|0x00e|0x00e:'69'
	// ******
}

// STATE[STUB]
void bt_character_controller::deactivate( )
{
	// FUNCTION BODY

	// <0x586930>|0x000|0x000:'75'
	// ******
}

// STATE[STUB]
float4x4 bt_character_controller::get_transform( )
{
	float4x4 result = float4x4();
	return result;

	// FUNCTION BODY
	// <0x5869a6>|0x000|0x000:'80'
	// ******
}

// STATE[STUB]
void bt_character_controller::set_transform( float4x4 const& transform )
{
	// FUNCTION BODY
	// <0x586a9a>|0x000|0x000:'85'
	// ******
}

// STATE[STUB]
void bt_character_controller::set_walk_direction( float3 const& direction )
{
	// FUNCTION BODY
	// <0x586959>|0x000|0x000:'90'
	// ******
}

// STATE[STUB]
bool bt_character_controller::has_updates( ) const
{
	return false;

	// FUNCTION BODY
	// <0x5867f0>|0x000|0x000:'95'
	// ******
}

// STATE[STUB]
void bt_character_controller::jump( )
{
}

// STATE[STUB]
void bt_character_controller::end_jump( )
{
	// FUNCTION BODY
	// <0x5867b0>|0x000|0x000:'105'
	// ******
}

// STATE[STUB]
// bool vostok::physics::bt_character_controller::adjust_foot_transform(vostok::math::float3 const&, vostok::math::float3 const&, vostok::math::float3 const&, float, float, vostok::math::float4x4&)
bool bt_character_controller::adjust_foot_transform(
	float3 const&                      half_size,
	float3 const&                      start,
	float3 const&                      finish,
	float                              rotation_koef0,
	float4x4&                          transform)
{
	return false;

	// LOCALS
	// float                           __formal
	// ******

	// FUNCTION BODY
	// <0x586b00>|0x000|0x000:'110'
	// ******
}

// STATE[STUB]
void bt_character_controller::update_action( u32 time_delta_in_ms )
{
	// FUNCTION BODY
	// <0x5867c1>|0x000|0x000:'121'
	// ******
}

// STATE[STUB]
bool bt_character_controller::can_jump( ) const
{
	return false;

	// FUNCTION BODY
	// <0x5868d1>|0x000|0x000:'126'
	// ******
}

// STATE[STUB]
bool bt_character_controller::on_ground( ) const
{
	return false;

	// FUNCTION BODY
	// <0x586891>|0x000|0x000:'131'
	// ******
}

// STATE[STUB]
void bt_character_controller::set_crouch( bool crouch )
{
}

// STATE[STUB]
bool bt_character_controller::can_crouch( ) const
{
	return false;

	// FUNCTION BODY
	// <0x5867a0>|0x000|0x000:'146'
	// ******
}

// STATE[STUB]
bool bt_character_controller::can_stand( ) const
{
	return false;

	// FUNCTION BODY
	// <0x586790>|0x000|0x000:'151'
	// ******
}

} // namespace physics
} // namespace vostok