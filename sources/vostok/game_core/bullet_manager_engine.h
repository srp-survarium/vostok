// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef BULLET_MANAGER_ENGINE_H_INCLUDED
#define BULLET_MANAGER_ENGINE_H_INCLUDED

namespace survarium {

// the tracer pure virtuals take bullet* - without this decl an includer that
// hasn't seen bullet yet (game_world.h) parses the param away to (void), so the
// override never matches (C3668 -> abstract game_world).
class bullet;

class bullet_manager_engine {
public:
	virtual	void	play_sound				(
						resources::unmanaged_resource_ptr const&	sound,
						float3 const&								position
					) = 0;
	virtual	void	add_decal				(
						resources::unmanaged_resource_ptr const&	decal,
						u32											decal_id,
						float										size,
						float										depth,
						float3 const&								position,
						float3 const&								direction,
						float3 const&								normal,
						bool										is_front_face
					) = 0;
	virtual	void	play_particle			(
						resources::unmanaged_resource_ptr const&	particle,
						float3 const&								position,
						float3 const&								direction,
						float3 const&								normal
					) = 0;
	virtual	void	remove_decal			( u32 decal_id ) = 0;

	virtual	bool	attach_tracer			( bullet* bullet ) = 0;
	virtual	bool	detach_tracer			( bullet* bullet ) = 0;
	virtual	void	update_tracer			(
						const u16			tracer_idx,
						float3 const&		position,
						float3 const&		direction,
						const float			length
					) = 0;
}; // class bullet_manager_engine

STATIC_SIZE_ASSERT(bullet_manager_engine, 0x4);

} // namespace survarium

#endif // #ifndef BULLET_MANAGER_ENGINE_H_INCLUDED
