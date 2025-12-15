////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_CHAMBER_A_ROUND_AIMED_STATE_H_INCLUDED
#define WEAPON_CORE_CHAMBER_A_ROUND_AIMED_STATE_H_INCLUDED

namespace survarium {

/* survarium::weapon_core_chamber_a_round_aimed_state */

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class survarium::weapon_core_chamber_a_round_aimed_state_base;
class vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class weapon_core_chamber_a_round_aimed_state : public weapon_core_chamber_a_round_aimed_state_base {
public:
			weapon_core_chamber_a_round_aimed_state(
													weapon_core&						weapon,
													float								animation_time_scale,
													resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const*	animations,
													u32									animations_count
												);

	virtual	animation::mixing::expression		weapon_and_hands_expression		(
													mutable_buffer&						buffer,
													bool								is_third_view,
													weapon_user_state_enum				user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const override;

			weapon_lexeme_pair					get_weapon_lexeme_pair			( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const;

			animation::mixing::expression		get_user_hands_expression		(
													animation::mixing::animation_lexeme&	weapon_lexeme,
													mutable_buffer&						buffer,
													bool								is_third_view,
													weapon_user_state_enum				user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const;

	virtual	~weapon_core_chamber_a_round_aimed_state( ) { /* no source */ }


private:
	/* 0x0000 */	/* weapon_core_chamber_a_round_aimed_state_base */
	/* 0x0148 */	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base>[2][2]	m_weapon_animations;
	/* 0x0158 */	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base>[2][2]	m_user_animations;
}; // class weapon_core_chamber_a_round_aimed_state

STATIC_SIZE_ASSERT(weapon_core_chamber_a_round_aimed_state, 0x168);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_CHAMBER_A_ROUND_AIMED_STATE_H_INCLUDED
