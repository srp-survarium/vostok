////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_LOGIC_PREVIEW_STATE_H_INCLUDED
#define PLAYER_LOGIC_PREVIEW_STATE_H_INCLUDED

#include <vostok/game_core/player_logic_base_state.h>
#include <vostok/math_randoms_generator.h>
#include <vostok/resources_managed_resource.h>

namespace survarium {

struct weapon_animation_parameters;
class weapon_user_animations_selector;

class player_logic_preview_state : public player_logic_base_state {
public:
					player_logic_preview_state	(
						resources::managed_resource_ptr*	animations,
						u32									animations_count,
						weapon_user_animations_selector&	owner
					);

private:
	// PDB mangles these overrides E* (private virtual), unlike the public ctor/dtor.
	virtual	void	initialize					( ) override { }
	virtual	void	finalize					( ) override { /* no source */ }

	virtual	void	execute						( ) override { /* no source */ }

	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >	selected_animations			(
						mutable_buffer&							buffer,
						weapon_animation_parameters const&		weapon_parameters,
						const bool								is_third_view
					) const override;

public:
	virtual			~player_logic_preview_state	( ) { /* no source */ }

private:
	/* 0x0000 */	/* player_logic_base_state */
	/* 0x0028 */	resources::managed_resource_ptr* const	m_animations;
	/* 0x002c */	const u32			m_animations_count;
	// selected_animations() is const but advances the generator (the random
	// animation pick), so the seed member is mutable.
	/* 0x0030 */	mutable math::random32	m_random;
}; // class player_logic_preview_state

STATIC_SIZE_ASSERT(player_logic_preview_state, 0x34);

} // namespace survarium

#endif // #ifndef PLAYER_LOGIC_PREVIEW_STATE_H_INCLUDED
