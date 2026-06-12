////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_USER_DEAD_STATE_H_INCLUDED
#define WEAPON_USER_DEAD_STATE_H_INCLUDED

/* INCLUDES */
class stlp_std::pair<vostok::animation::mixing::expression,vostok::animation::mixing::animation_lexeme>;
class vostok::math::random32;
class survarium::player_logic_base_state;
class survarium::weapon;

/* FORWARD REFS */
class survarium::weapon_animation_parameters;

namespace survarium {

class weapon_user_dead_state : public player_logic_base_state {
public:
	inline	explicit	weapon_user_dead_state	( weapon& arg_0 ) { /* no source */ }

	virtual	std::pair< animation::mixing::expression, animation::mixing::animation_lexeme >	selected_animations		(
							mutable_buffer&							buffer,
							weapon_animation_parameters const&		weapon_parameters,
							const bool								is_third_view
						) const override;

	virtual	void		initialize				( ) override { /* no source */ }

	virtual	void		execute					( ) override { /* no source */ }

	virtual	void		finalize				( ) override { /* no source */ }

	virtual	bool		is_ready_for_transition	( ) const override;

	virtual				~weapon_user_dead_state	( ) { /* no source */ }

private:
	/* 0x0000 */	/* player_logic_base_state */
	/* 0x0028 */	weapon&				m_weapon;
	/* 0x002c */	math::random32		m_random;
}; // class weapon_user_dead_state

STATIC_SIZE_ASSERT(weapon_user_dead_state, 0x30);

} // namespace survarium

#endif // #ifndef WEAPON_USER_DEAD_STATE_H_INCLUDED
