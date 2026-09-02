// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef WEAPON_USER_ANIMATIONS_CONTAINER_INLINE_H_INCLUDED
#define WEAPON_USER_ANIMATIONS_CONTAINER_INLINE_H_INCLUDED

namespace survarium {

// claude@NOTE: get_animation_impl<27,6> (mangled $0BL@$05 == <27,6>; $05 encodes 6) is the
// instantiation get_stand_animation forwards to, passing the [6] hands member. It is kept
// out-of-line in the target (standalone @0x0bcf90) because the *_lexeme callers in
// player_logic_*_state inline get_stand_animation but leave the nested template call as a
// `call`. ODR-using it from those callers (movement_lexeme et al.) emits the standalone here
// so objdiff pairs it; get_stand_animation itself is the inlined impl.
template < u32 move_count, u32 hands_count >
inline resources::managed_resource_ptr weapon_user_animations_container::get_animation_impl(
		resources::managed_resource_ptr const	(&move)[ move_count ],
		resources::managed_resource_ptr const	(&hands)[ hands_count ],
		const u32								index
	)
{
	ASSERT( UNKNOWN_EXPRESSION_T( index < move_count + hands_count ) );
	return index < move_count ? move[ index ] : hands[ index - move_count ];
}

inline resources::managed_resource_ptr weapon_user_animations_container::get_stand_animation( const bool aimed, const u32 index, const bool is_third_view ) const
{
	return get_animation_impl(
		( aimed ? m_aimed_stand_animations : m_stand_animations )[ is_third_view ],
		( aimed ? m_aimed_stand_hands_only_animations : m_stand_hands_only_animations )[ is_third_view ],
		index
	);
}

inline resources::managed_resource_ptr weapon_user_animations_container::get_crouch_animation( const bool aimed, const u32 index, const bool is_third_view ) const
{
	return get_animation_impl(
		( aimed ? m_aimed_crouch_animations : m_crouch_animations )[ is_third_view ],
		( aimed ? m_aimed_crouch_hands_only_animations : m_crouch_hands_only_animations )[ is_third_view ],
		index
	);
}

} // namespace survarium

#endif // #ifndef WEAPON_USER_ANIMATIONS_CONTAINER_INLINE_H_INCLUDED
