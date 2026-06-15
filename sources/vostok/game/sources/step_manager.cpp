////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "step_manager.h"

namespace survarium {

// STATE[STUB]
 step_manager::step_manager( )
{
	// FUNCTION BODY[0x7025a0]: 0
	// <0x7025a0>|0x000|+0x006:'22'	{
	// <0x7025a6>|0x006|      :'23'	}
	// ******
}

// STATE[STUB]
void step_manager::on_step(
	player const&		a,
	float3 const&		position,
	float3 const&		direction,
	game_world&			world
) const
{
	// LOCALS
	// sound::sound_emitter_ptr 		sound
	// physics::closest_ray_result 		ray_result
	// ******

	// STATICS
	// static u32 						decal_id = <0x10000>;
	// ******

	// CALL SITE INFO
	// <0x70262e> -> physics::closest_ray_result < unknown >( float3 const&, float3 const&, const float, u16, u16 )
	// <0x702698> -> u16 < unknown >( const int, const bool ) const
	// <0x7026ff> -> sound::world_user& < unknown >() const
	// <0x702760> -> void < unknown >( resources::unmanaged_resource_ptr const&, u32, float, float, float3 const&, float3 const&, float3 const&, bool )
	// <0x7027d2> -> void < unknown >( resources::unmanaged_resource_ptr const&, float3 const&, float3 const&, float3 const& )
	// ******

	// FUNCTION BODY[0x7025b0]: 41
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x7025b9>|0x009|+0x077:'34'
	// <0>
	// <0x702630>|0x080|+0x00c:'36'
	// <0>
	// <1>
	// <0x70263c>|0x08c|+0x038:'39'
	// <0x702674>|0x0c4|+0x015:'40'
	// <0>
	// <1>
	// <2>
	// <0x702689>|0x0d9|+0x011:'44'
	// <0x70269a>|0x0ea|+0x009:'45'
	// <0x7026a3>|0x0f3|+0x010:'46'
	// <0x7026b3>|0x103|+0x015:'47'
	// <0x7026c8>|0x118|+0x011:'48'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x7026d9>|0x129|+0x037:'54'
	// <0>
	// <1>
	// <0x702710>|0x160|+0x00f:'57'
	// <0>
	// <0x70271f>|0x16f|+0x043:'59'
	// <0x702762>|0x1b2|+0x006:'60'
	// <0x702768>|0x1b8|+0x00c:'61'
	// <0>
	// <1>
	// <0x702774>|0x1c4|+0x007:'64'
	// <0>
	// <0x70277b>|0x1cb|+0x007:'66'
	// <0x702782>|0x1d2|+0x052:'67'
	// <0>
	// ******
}


} // namespace survarium
