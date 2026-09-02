// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef STEP_MANAGER_H_INCLUDED
#define STEP_MANAGER_H_INCLUDED

namespace survarium {

class game_world;
class player;

class step_manager : private boost::noncopyable {
public:
					step_manager	( );

			void	on_step			(
						player const&		a,
						float3 const&		position,
						float3 const&		direction,
						game_world&			world
						) const;

	enum { max_decal_count = 32 };

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	mutable u32		m_decal_id;	// const on_step() bumps it per step
}; // class step_manager

STATIC_SIZE_ASSERT(step_manager, 0x4);

} // namespace survarium

#endif // #ifndef STEP_MANAGER_H_INCLUDED
