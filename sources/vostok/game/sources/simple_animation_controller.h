// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef SIMPLE_ANIMATION_CONTROLLER_H_INCLUDED
#define SIMPLE_ANIMATION_CONTROLLER_H_INCLUDED

#include "base_animation_controller.h"
#include "animation_controller_parameters.h"

namespace survarium {

class human_npc;

class simple_animation_controller : public base_animation_controller {
public:
			explicit							simple_animation_controller	( human_npc& owner );
	virtual										~simple_animation_controller( );

	virtual	void								initialize					( ) override;

	virtual	animation::mixing::expression		try_finalize				( base_animation_controller& next_controller, mutable_buffer& buffer ) override;

	virtual	animation::mixing::expression		selected_animations			( mutable_buffer& buffer ) override;

	virtual	void								set_target					( animation_controller_parameters const& target ) override;

	virtual	void								query_new_target_if_needed	( ) override;

	virtual	void								debug_draw					( render::game::renderer& render, render::scene_ptr const& scene ) const override;

private:
	/* 0x0000 */	/* base_animation_controller */
	/* 0x0004 */	simple_animation_controller_parameters	m_current_parameters;
	/* 0x000c */	simple_animation_controller_parameters	m_target_parameters;
	/* 0x0014 */	human_npc&		m_owner;
	/* 0x0018 */	bool			m_last_animation_emitted;
}; // class simple_animation_controller

STATIC_SIZE_ASSERT(simple_animation_controller, 0x1C);

} // namespace survarium

#endif // #ifndef SIMPLE_ANIMATION_CONTROLLER_H_INCLUDED
