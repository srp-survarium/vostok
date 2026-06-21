////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "step_manager.h"

namespace survarium {

 step_manager::step_manager( ) :
	m_decal_id( 0 )
{
}

// STATE[STUB]
// claude@NOTE: parked - casts a downward physics ray (physics::closest_ray_result) from
// position along direction to find the ground material, looks up the material_pair via
// game_world's game_material_manager::get_pair, and emits the footstep particle +
// sound_emitter::emit_and_play_once on the world sound_user; the static decal_id counter
// increments per step. Named locals: sound (sound_emitter_ptr), ray_result
// (closest_ray_result); static u32 decal_id. NEXT: recover the physics ray-cast ctor + the
// material/sound accessors (lines 34-67) - walled by physics::closest_ray_result + the
// game_material_manager wiring.
void step_manager::on_step(
	player const&		a,
	float3 const&		position,
	float3 const&		direction,
	game_world&			world
) const
{
}


} // namespace survarium
