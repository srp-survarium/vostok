////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SINGLE_POSITION_ANIMATION_CONTROLLER_H_INCLUDED
#define SINGLE_POSITION_ANIMATION_CONTROLLER_H_INCLUDED

/* INCLUDES */
class vostok::vectora<vostok::math::float3>;
class survarium::animations_search_service;
class survarium::human_npc;
struct vostok::ai::navigation::world;
struct survarium::animation_space_vertex_id;
struct survarium::base_animation_controller;
struct survarium::movement_animation_controller_parameters;
class vostok::animation::mixing::expression;
class vostok::render::base_scene;
typedef vostok::resources::resource_ptr<survarium::animation_space_graph,vostok::resources::unmanaged_intrusive_base>
	survarium::animation_space_graph_ptr;
class survarium::animation_space_graph;

/* FORWARD REFS */
class vostok::render::game::renderer;
class survarium::animation_controller_parameters;

namespace survarium {

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

	virtual	void								debug_draw					( render::game::renderer& render, render::base_scene_ptr const& scene ) const override;

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
