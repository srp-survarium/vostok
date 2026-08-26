////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BREATH_VIBRATION_CALCULATOR_H_INCLUDED
#define BREATH_VIBRATION_CALCULATOR_H_INCLUDED

#include <vostok/ai/fsm.h>

namespace survarium {

struct base_player;
struct breath_holding_params;


class breath_vibration_calculator : private boost::noncopyable {
public:
			explicit	breath_vibration_calculator	( );
						~breath_vibration_calculator( );

			void		set_breath_holding_params	( breath_holding_params const* params );

			void		tick						( const u32 current_time_in_ms, const float time_scale );

			void		hold_breath					( bool value );

	inline	void		set_max_breath_delay_time	( const float arg_0 ) { /* no source */ }

	inline	void		set_user					( base_player const* const arg_0 ) { m_user = arg_0; }

	inline	float		get_horizontal_value		( ) const { return m_horizontal_value; }
	inline	float		get_vertical_value			( ) const { return m_vertical_value; }

	inline	void		set_character_multiplier	( const float arg_0 ) { m_character_multiplier = arg_0; }

private:
			void		initialize_logic			( );

			bool		hold_button_state_equals_to	( bool btn_state ) const { return m_is_breath_holded == btn_state; }

			bool		insufficient_breath			( ) const { return m_breath_holding_reserve <= 0.0f; }


private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	ai::fsm							m_logic;
	/* 0x0014 */	base_player const*				m_user;
	/* 0x0018 */	breath_holding_params const*	m_params;
	/* 0x001c */	u32								m_last_time_in_ms;
	/* 0x0020 */	float							m_current_multiplier;
	/* 0x0024 */	float							m_target_multiplier;
	/* 0x0028 */	float							m_vertical_value;
	/* 0x002c */	float							m_horizontal_value;
	/* 0x0030 */	float							m_character_multiplier;
	/* 0x0034 */	float							m_breath_holding_reserve;
	/* 0x0038 */	bool							m_is_breath_holded;
}; // class breath_vibration_calculator

STATIC_SIZE_ASSERT(breath_vibration_calculator, 0x3C);

} // namespace survarium

#endif // #ifndef BREATH_VIBRATION_CALCULATOR_H_INCLUDED
