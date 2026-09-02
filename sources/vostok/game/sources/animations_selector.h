// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef ANIMATIONS_SELECTOR_H_INCLUDED
#define ANIMATIONS_SELECTOR_H_INCLUDED

#include "animation_controller_parameters.h"
#include "single_position_animation_controller.h"
#include "simple_animation_controller.h"
// callback_return_type_enum cannot be forward-declared (C++03 enum)
#include <vostok/animation/animation_callback.h>
#include <vostok/resources_managed_resource.h>

namespace vostok {
namespace ai {
	struct animation_item;
	struct movement_target;
} // namespace ai
namespace animation {
	class animation_player;
} // namespace animation
} // namespace vostok

namespace survarium {

class game_world;

class animations_selector : private boost::noncopyable {
public:
					animations_selector			(
						animation::animation_player&		player,
						animation_space_graph_ptr const&	space_graph,
						resources::managed_resource_ptr const&	default_animation,
						ai::navigation::world const&		ai_navigation_world,
						game_world const&					world,
						human_npc&							owner
					);

			void	set_target					( ai::animation_item const& animation_emitter );
			void	set_target					( ai::movement_target const& target_position );

			void	debug_draw					( render::game::renderer& render, render::scene_ptr const& scene ) const;

private:
			animation::callback_return_type_enum	on_animation_interval_end	( animation::animation_callback_params& params );

			void	set_animation_player_target	( animation::mixing::expression const& target_expression, u32 time_in_ms );

			void	reset_animation_controller	( u32 time_in_ms );

			void	on_set_target				( );

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	single_position_animation_controller	m_single_position_animation_controller;
	/* 0x0084 */	simple_animation_controller			m_simple_animation_controller;
	/* 0x00a0 */	simple_animation_controller_parameters	m_simple_animation_parameters;
	/* 0x00a8 */	movement_animation_controller_parameters	m_movement_animation_parameters;
	/* 0x00d4 */	animation::animation_player&		m_animation_player;
	/* 0x00d8 */	base_animation_controller*			m_current_controller;
	/* 0x00dc */	base_animation_controller*			m_target_controller;
	/* 0x00e0 */	animation_controller_parameters*	m_target_controller_parameters;
	/* 0x00e4 */	game_world const&					m_game_world;
	/* 0x00e8 */	human_npc&							m_owner;
	/* 0x00ec */	resources::managed_resource_ptr		m_default_animation;

}; // class animations_selector

STATIC_SIZE_ASSERT(animations_selector, 0xF0);

} // namespace survarium

#endif // #ifndef ANIMATIONS_SELECTOR_H_INCLUDED
