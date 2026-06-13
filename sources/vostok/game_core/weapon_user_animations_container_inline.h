////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_USER_ANIMATIONS_CONTAINER_INLINE_H_INCLUDED
#define WEAPON_USER_ANIMATIONS_CONTAINER_INLINE_H_INCLUDED

namespace survarium {

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

} // namespace survarium

#endif // #ifndef WEAPON_USER_ANIMATIONS_CONTAINER_INLINE_H_INCLUDED
