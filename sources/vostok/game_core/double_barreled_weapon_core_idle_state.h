////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef DOUBLE_BARRELED_WEAPON_CORE_IDLE_STATE_H_INCLUDED
#define DOUBLE_BARRELED_WEAPON_CORE_IDLE_STATE_H_INCLUDED

namespace survarium {

/* survarium::double_barreled_weapon_core_idle_state */

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

class survarium::weapon_core_idle_state_base;
class vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class double_barreled_weapon_core_idle_state : public weapon_core_idle_state_base {
public:
			double_barreled_weapon_core_idle_state( weapon_core& weapon, resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base> const* animations, u32 animations_count );

	virtual	animation::mixing::expression		weapon_and_hands_expression		(
													mutable_buffer&						buffer,
													bool								is_third_view,
													weapon_user_state_enum				user_state_id,
													animation::mixing::animation_lexeme&	weight_driving_animation
												) const override;

			weapon_lexeme_pair					get_weapon_lexeme_pair			( mutable_buffer& buffer, bool is_third_view, weapon_user_state_enum user_state_id ) const;

	virtual	~double_barreled_weapon_core_idle_state( ) { /* no source */ }


private:
	/* 0x0000 */	/* weapon_core_idle_state_base */
	/* 0x0138 */	resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base>[2][2][3]	m_weapon_animations;
}; // class double_barreled_weapon_core_idle_state

STATIC_SIZE_ASSERT(double_barreled_weapon_core_idle_state, 0x168);

} // namespace survarium

#endif // #ifndef DOUBLE_BARRELED_WEAPON_CORE_IDLE_STATE_H_INCLUDED
