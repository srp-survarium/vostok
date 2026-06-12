////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATIONS_SELECTOR_H_INCLUDED
#define ANIMATIONS_SELECTOR_H_INCLUDED

/* INCLUDES */
class vostok::animation::animation_player;
class survarium::game_world;
class survarium::human_npc;
class survarium::simple_animation_controller;
class survarium::single_position_animation_controller;
struct survarium::animation_controller_parameters;
struct survarium::base_animation_controller;
struct survarium::movement_animation_controller_parameters;
struct survarium::simple_animation_controller_parameters;
enum vostok::animation::callback_return_type_enum;
class vostok::render::base_scene;
class survarium::animation_space_graph;
typedef vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>
	vostok::resources::managed_resource_ptr;
class vostok::resources::managed_resource;

/* FORWARD REFS */
class vostok::ai::animation_item;
class vostok::ai::movement_target;
class vostok::ai::navigation::world;
class vostok::animation::animation_callback_params;
class vostok::animation::mixing::expression;
class vostok::render::game::renderer;

namespace survarium {

class animations_selector : public boost::noncopyable {
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

			void	debug_draw					( render::game::renderer& render, render::base_scene_ptr const& scene ) const;

			animation::callback_return_type_enum	on_animation_interval_end	( animation::animation_callback_params& params );

			void	set_animation_player_target	( animation::mixing::expression const& target_expression, u32 time_in_ms );

			void	reset_animation_controller	( u32 time_in_ms );

			void	on_set_target				( );

	inline			~animations_selector		( ) { /* no source */ }

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
