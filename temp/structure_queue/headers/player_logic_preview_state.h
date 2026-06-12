////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PLAYER_LOGIC_PREVIEW_STATE_H_INCLUDED
#define PLAYER_LOGIC_PREVIEW_STATE_H_INCLUDED

/* INCLUDES */
class stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme>;
class vostok::math::random32;
class survarium::player_logic_base_state;
typedef vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>
	vostok::resources::managed_resource_ptr;
class vostok::resources::managed_resource;

/* FORWARD REFS */
class survarium::weapon_animation_parameters;
class survarium::weapon_user_animations_selector;

namespace survarium {

class player_logic_preview_state : public player_logic_base_state {
public:
					player_logic_preview_state	(
						resources::managed_resource_ptr*	animations,
						u32									animations_count,
						weapon_user_animations_selector&	owner
					);

	virtual	void	initialize					( ) override { /* no source */ }
	virtual	void	finalize					( ) override { /* no source */ }

	virtual	void	execute						( ) override { /* no source */ }

	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >	selected_animations			(
						mutable_buffer&							buffer,
						weapon_animation_parameters const&		weapon_parameters,
						const bool								is_third_view
					) const override;

	virtual			~player_logic_preview_state	( ) { /* no source */ }

private:
	/* 0x0000 */	/* player_logic_base_state */
	/* 0x0028 */	resources::managed_resource_ptr* const	m_animations;
	/* 0x002c */	const u32			m_animations_count;
	/* 0x0030 */	math::random32		m_random;
}; // class player_logic_preview_state

STATIC_SIZE_ASSERT(player_logic_preview_state, 0x34);

} // namespace survarium

#endif // #ifndef PLAYER_LOGIC_PREVIEW_STATE_H_INCLUDED
