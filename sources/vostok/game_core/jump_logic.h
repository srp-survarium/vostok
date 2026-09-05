// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef JUMP_LOGIC_H_INCLUDED
#define JUMP_LOGIC_H_INCLUDED

#include <vostok/game_core/move_direction_enum.h>

#include <vostok/animation/mixing_animation_lexeme.h>
#include <vostok/animation/mixing_expression.h>

namespace vostok {
namespace ai {
	class fsm;
}
}

namespace survarium {

class weapon_user_animations_selector;	// sushi@TODO
struct weapon_animation_parameters;

struct base_player;
struct player_input;

enum jump_animation_parts {
	jump_animations_part_start			= 0x0000,
	jump_animations_part_start_look		= 0x0001,
	jump_animations_part_land			= 0x0002,
	jump_animations_part_land_look		= 0x0003,
	jump_animations_part_land_run		= 0x0004,
	jump_animations_part_land_run_look	= 0x0005,
	jump_animation_parts_count			= 0x0006,
};

u32 get_jump_animation_index( const move_direction_enum move_direction, const bool jump_from_right_leg, const jump_animation_parts animation_part );

class jump_logic : private core::noncopyable {
public:
	typedef std::pair< animation::mixing::expression, animation::mixing::animation_lexeme > selected_animations_result_type;
	typedef boost::function< void( resources::managed_resource_ptr const&, pcstr ) > animation_callback_type;

			explicit							jump_logic				( weapon_user_animations_selector& owner );
												~jump_logic				( );

	inline	move_direction_enum					get_jump_direction		( ) const { return m_jumping_direction; }
	inline	bool								is_jump_from_right_leg	( ) const { return m_is_jump_from_right_leg; }

	inline	weapon_user_animations_selector&	owner					( ) const { return m_owner; }

			selected_animations_result_type
												selected_animations		( mutable_buffer& buffer, weapon_animation_parameters const& weapon_parameters, const bool is_third_view ) const;

			void								tick					( );

			void								set_user				( base_player& user );

			resources::managed_resource_ptr		get_animation			( const jump_animation_parts anim_part, const bool is_third_view ) const;
			pcstr								get_animation_caption	( const jump_animation_parts anim_part ) const;

			resources::managed_resource_ptr		get_move_animation		( const bool is_third_view ) const;
			resources::managed_resource_ptr		get_move_look_animation	( const bool is_third_view ) const;

			pcstr								get_move_look_caption	( ) const;
			bool								does_need_land_and_run	( ) const;

	inline	pcvoid								get_animated_object		( ) const { return m_animated_object; }

			void								activate				( );
			void								deactivate				( );

			bool								is_jump_finished		( ) const;
			float								look_time_factor		( ) const;

private:	// claude@MATCH: mangled access - landing_predicate ABE, initialize_logic AAE
			bool								landing_predicate		( ) const;

			void								initialize_logic		( );


private:
	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	weapon_user_animations_selector&	m_owner;
	/* 0x0004 */	base_player*						m_user;
	/* 0x0008 */	ai::fsm*							m_logic;
	/* 0x000c */	pcvoid const						m_animated_object;
	/* 0x0010 */	move_direction_enum					m_jumping_direction;
	/* 0x0014 */	bool								m_is_jump_from_right_leg;
}; // class jump_logic

STATIC_SIZE_ASSERT(jump_logic, 0x18);

} // namespace survarium

#endif // #ifndef JUMP_LOGIC_H_INCLUDED
