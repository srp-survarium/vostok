// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef SINGLE_POSITION_ANIMATION_CONTROLLER_H_INCLUDED
#define SINGLE_POSITION_ANIMATION_CONTROLLER_H_INCLUDED

#include "base_animation_controller.h"
#include "animation_controller_parameters.h"
#include <vostok/resources_unmanaged_resource.h>

namespace vostok {
namespace ai {
namespace navigation {
	struct world;
} // namespace navigation
} // namespace ai
} // namespace vostok

namespace survarium {

class animations_search_service;
class human_npc;
struct animation_space_vertex_id;

class animation_space_graph;
typedef resources::resource_ptr<
	animation_space_graph,
	resources::unmanaged_intrusive_base
> animation_space_graph_ptr;

class single_position_animation_controller : public base_animation_controller {
public:
												single_position_animation_controller(
													animation_space_graph_ptr const&	graph,
													ai::navigation::world const&		ai_navigation_world,
													human_npc&							owner
												);
	virtual										~single_position_animation_controller( );

	virtual	void								initialize					( ) override;

	virtual	animation::mixing::expression		try_finalize				( base_animation_controller& next_controller, mutable_buffer& buffer ) override;

	virtual	animation::mixing::expression		selected_animations			( mutable_buffer& buffer ) override;

	virtual	void								set_target					( animation_controller_parameters const& target ) override;

	virtual	void								query_new_target_if_needed	( ) override;

	virtual	void								debug_draw					( render::game::renderer& render, render::scene_ptr const& scene ) const override;

private:
	/* 0x0000 */	/* base_animation_controller */
	/* 0x0004 */	movement_animation_controller_parameters	m_current_parameters;
	/* 0x0030 */	movement_animation_controller_parameters	m_target_parameters;
	/* 0x005c */	animation_space_graph_ptr		m_animation_space_graph;
	/* 0x0060 */	animations_search_service*		m_search_service;
	/* 0x0064 */	animation_space_vertex_id*		m_target_vertex;
	/* 0x0068 */	ai::navigation::world const&	m_ai_navigation_world;
	/* 0x006c */	human_npc&						m_owner;
	/* 0x0070 */	vectora< float3 >				m_navigation_path;
	/* 0x0080 */	u32								m_next_key_point;
}; // class single_position_animation_controller

STATIC_SIZE_ASSERT(single_position_animation_controller, 0x84);

} // namespace survarium

#endif // #ifndef SINGLE_POSITION_ANIMATION_CONTROLLER_H_INCLUDED
