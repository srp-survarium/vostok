////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_USER_ANIMATIONS_CONTAINER_H_INCLUDED
#define WEAPON_USER_ANIMATIONS_CONTAINER_H_INCLUDED

namespace survarium {

class weapon_user_animations_container_cook;

// claude@MATCH: extern caption arrays - single .rdata copy each in the target exe
// (external `?..._captions@survarium@@3QBQBDB` symbols); stand_animations_captions is
// defined in player_logic_stand_state.cpp, crouch_animations_captions in
// player_logic_crouch_state.cpp, jump_animations_captions in jump_logic.cpp.
extern pcstr const stand_animations_captions[];
extern pcstr const crouch_animations_captions[];
extern pcstr const sprint_animations_captions[];
extern pcstr const jump_animations_captions[];

class weapon_user_animations_container : public resources::unmanaged_resource , private core::noncopyable {
	friend class weapon_user_animations_container_cook;
public:
			resources::managed_resource_ptr	get_stand_animation			( const bool aimed, const u32 index, const bool is_third_view ) const;
			resources::managed_resource_ptr	get_crouch_animation		( const bool aimed, const u32 index, const bool is_third_view ) const;
	inline	resources::managed_resource_ptr get_sprint_animation		( const u32 index, const bool is_third_view ) const { ASSERT( UNKNOWN_EXPRESSION_T( index < sprint_animations_count ) ); return m_sprint_animations[is_third_view][index]; }
	inline	resources::managed_resource_ptr	get_jump_animation			( const u32 index, const bool is_third_view ) const { ASSERT( UNKNOWN_EXPRESSION_T( index < jump_animations_count ) ); return m_jump_animations[is_third_view][index]; }

	inline	pcstr		get_stand_animation_caption		( const bool aimed, const u32 index ) const { ASSERT( UNKNOWN_EXPRESSION_T( index < 33 ) ); return stand_animations_captions[ index ]; }
	inline	pcstr		get_crouch_animation_caption	( const bool aimed, const u32 index ) const { ASSERT( UNKNOWN_EXPRESSION_T( index < 33 ) ); return crouch_animations_captions[ index ]; }
	inline	pcstr		get_sprint_animation_caption	( const u32 index ) const { ASSERT( UNKNOWN_EXPRESSION_T( index < sprint_animations_count ) ); return sprint_animations_captions[ index ]; }
	inline	pcstr		get_jump_animation_caption		( const u32 index ) const { ASSERT( UNKNOWN_EXPRESSION_T( index < jump_animations_count ) ); return jump_animations_captions[ index ]; }

	enum {
		move_directions_count = 9,
		animations_per_one_move_direction = 3,
		move_animations_count = 27,
		hands_only_animations_count = 6,
		sprint_animations_count = 2,
		simple_jump_directions_count = 1,
		tho_legged_jump_directions_count = 8,
		animations_per_simple_jump_direction = 4,
		animations_per_two_legged_jump_direction = 6,
		jump_animations_count = 100,
		total_animations_count = 468,
	};

private:
	template < u32 move_count, u32 hands_count >
	static	resources::managed_resource_ptr	get_animation_impl			(
				resources::managed_resource_ptr const	(&move)[ move_count ],
				resources::managed_resource_ptr const	(&hands)[ hands_count ],
				const u32								index
			);

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	resources::managed_resource_ptr		m_stand_animations[2][move_animations_count];
	/* 0x01e0 */	resources::managed_resource_ptr		m_stand_hands_only_animations[2][hands_only_animations_count];
	/* 0x0210 */	resources::managed_resource_ptr		m_aimed_stand_animations[2][move_animations_count];
	/* 0x02e8 */	resources::managed_resource_ptr		m_aimed_stand_hands_only_animations[2][hands_only_animations_count];
	/* 0x0318 */	resources::managed_resource_ptr		m_crouch_animations[2][move_animations_count];
	/* 0x03f0 */	resources::managed_resource_ptr		m_crouch_hands_only_animations[2][hands_only_animations_count];
	/* 0x0420 */	resources::managed_resource_ptr		m_aimed_crouch_animations[2][move_animations_count];
	/* 0x04f8 */	resources::managed_resource_ptr		m_aimed_crouch_hands_only_animations[2][hands_only_animations_count];
	/* 0x0528 */	resources::managed_resource_ptr		m_sprint_animations[2][sprint_animations_count];
	/* 0x0538 */	resources::managed_resource_ptr		m_jump_animations[2][jump_animations_count];
}; // class weapon_user_animations_container

STATIC_SIZE_ASSERT(weapon_user_animations_container, 0x858);

typedef resources::resource_ptr< weapon_user_animations_container, resources::unmanaged_intrusive_base > weapon_user_animations_container_ptr;

} // namespace survarium

#include <vostok/game_core/weapon_user_animations_container_inline.h>

#endif // #ifndef WEAPON_USER_ANIMATIONS_CONTAINER_H_INCLUDED
