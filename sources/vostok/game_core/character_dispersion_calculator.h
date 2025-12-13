////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef CHARACTER_DISPERSION_CALCULATOR_H_INCLUDED
#define CHARACTER_DISPERSION_CALCULATOR_H_INCLUDED

namespace survarium {

/* survarium::character_dispersion_calculator */

//////////////////////////
// FORWARD DECLARATIONS //
//////////////////////////

struct survarium::character_dispersion_params;
class vostok::core::noncopyable;

//////////////////////////
//     DEFINITIONS      //
//////////////////////////

class character_dispersion_calculator : public core::noncopyable {
public:
			character_dispersion_calculator	( );

			void		set_character_dispersion_params	( character_dispersion_params const* character_params );

			void		tick							(
							weapon_user_state_enum		character_state,
							bool						is_moving,
							bool						is_aiming,
							u8							broken_hands_count,
							bool						using_double_handed_weapon,
							u32							current_time_in_ms
						);

	inline	void		reset							( ) { /* no source */ }

	inline	float		get_value						( ) const { /* no source */ }

	inline	void		set_aiming_speed				( float arg_0 ) { /* no source */ }

			float		get_target_koef					( weapon_user_state_enum character_state, bool is_moving, bool is_aiming ) const;

			float		get_broken_hands_penalty		( u8 broken_hands_count, bool using_double_handed_weapon ) const;

	inline	~character_dispersion_calculator( ) { /* no source */ }


private:
	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	character_dispersion_params const*	m_params;
	/* 0x0004 */	float								m_target_value;
	/* 0x0008 */	float								m_current_value;
	/* 0x000c */	float								m_value;
	/* 0x0010 */	float								m_value_smoothing_speed;
	/* 0x0014 */	float								m_aiming_speed;
	/* 0x0018 */	u32									m_current_time;
	/* 0x001c */	bool								m_jumped;
}; // class character_dispersion_calculator

STATIC_SIZE_ASSERT(character_dispersion_calculator, 0x20);

} // namespace survarium

#endif // #ifndef CHARACTER_DISPERSION_CALCULATOR_H_INCLUDED
