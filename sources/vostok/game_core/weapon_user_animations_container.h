////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_USER_ANIMATIONS_CONTAINER_H_INCLUDED
#define WEAPON_USER_ANIMATIONS_CONTAINER_H_INCLUDED

namespace survarium {

class weapon_user_animations_container : public resources::unmanaged_resource , public core::noncopyable {
public:
	// sushi@TODO: Where are you defined?
			resources::managed_resource_ptr	get_stand_animation			( bool aimed, u32 index, bool is_third_view ) const;
	inline	resources::managed_resource_ptr	get_crouch_animation		( bool arg_0, u32 arg_1, bool arg_2 ) const { /* no source */ }
	inline	resources::managed_resource_ptr get_sprint_animation		( u32 arg_0, bool arg_1 ) const { /* no source */ }
	inline	resources::managed_resource_ptr	get_jump_animation			( u32 index, bool is_third_view ) const { ASSERT( UNKNOWN_EXPRESSION_T( index < 100 ) ); return m_jump_animations[is_third_view][index]; }

	inline	pcstr		get_stand_animation_caption		( bool arg_0, u32 arg_1 ) const { /* no source */ }
	inline	pcstr		get_crouch_animation_caption	( bool arg_0, u32 arg_1 ) const { /* no source */ }
	inline	pcstr		get_sprint_animation_caption	( u32 arg_0 ) const { /* no source */ }
	inline	pcstr		get_jump_animation_caption		( u32 arg_0 ) const { /* no source */ }

public:
	// sushi@TODO: Where are you defined?
	explicit									weapon_user_animations_container( );

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	resources::managed_resource_ptr		m_stand_animations[2][27];
	/* 0x01e0 */	resources::managed_resource_ptr		m_stand_hands_only_animations[2][6];
	/* 0x0210 */	resources::managed_resource_ptr		m_aimed_stand_animations[2][27];
	/* 0x02e8 */	resources::managed_resource_ptr		m_aimed_stand_hands_only_animations[2][6];
	/* 0x0318 */	resources::managed_resource_ptr		m_crouch_animations[2][27];
	/* 0x03f0 */	resources::managed_resource_ptr		m_crouch_hands_only_animations[2][6];
	/* 0x0420 */	resources::managed_resource_ptr		m_aimed_crouch_animations[2][27];
	/* 0x04f8 */	resources::managed_resource_ptr		m_aimed_crouch_hands_only_animations[2][6];
	/* 0x0528 */	resources::managed_resource_ptr		m_sprint_animations[2][2];
	/* 0x0538 */	resources::managed_resource_ptr		m_jump_animations[2][100];
}; // class weapon_user_animations_container

STATIC_SIZE_ASSERT(weapon_user_animations_container, 0x858);

typedef resources::resource_ptr< weapon_user_animations_container, resources::unmanaged_intrusive_base > weapon_user_animations_container_ptr;

} // namespace survarium

#include <vostok/game_core/weapon_user_animations_container_inline.h>

#endif // #ifndef WEAPON_USER_ANIMATIONS_CONTAINER_H_INCLUDED
