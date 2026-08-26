////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BREATH_HOLDING_PARAMS_H_INCLUDED
#define BREATH_HOLDING_PARAMS_H_INCLUDED

namespace survarium {

struct breath_holding_params {
	/* 0x0000 */	float		max_breath_holding_time;
	/* 0x0004 */	float		shortbreathing_repair_time;
	/* 0x0008 */	float		breath_holding_multiplier;
	/* 0x000c */	float		shortbreathing_multiplier;
	/* 0x0010 */	float		multiplier_increase_speed;
	/* 0x0014 */	float		multiplier_decrease_speed;
	/* 0x0018 */	float		horizontal_amplitude;
	/* 0x001c */	float		vertical_amplitude;
	/* 0x0020 */	float		horizontal_peroid;
	/* 0x0024 */	float		vertical_peroid;

	inline	explicit	breath_holding_params	( ) :
		max_breath_holding_time		( 2.5f ),
		shortbreathing_repair_time	( 6.0f ),
		breath_holding_multiplier	( 0.5f ),
		shortbreathing_multiplier	( 3.0f ),
		multiplier_increase_speed	( 1.0f ),
		multiplier_decrease_speed	( 1.0f ),
		horizontal_amplitude		( 1.0f ),
		vertical_amplitude			( 1.0f ),
		horizontal_peroid			( 1.0f ),
		vertical_peroid				( 1.0f )
	{
	}

			void		load					( configs::binary_config_value const& cfg )
	{
		if ( cfg.value_exists( "max_breath_holding_time" ) )
			max_breath_holding_time = (float)cfg["max_breath_holding_time"];
		if ( cfg.value_exists( "shortbreathing_repair_time" ) )
			shortbreathing_repair_time = (float)cfg["shortbreathing_repair_time"];
		if ( cfg.value_exists( "breath_holding_multiplier" ) )
			breath_holding_multiplier = (float)cfg["breath_holding_multiplier"];
		if ( cfg.value_exists( "shortbreathing_multiplier" ) )
			shortbreathing_multiplier = (float)cfg["shortbreathing_multiplier"];
		if ( cfg.value_exists( "multiplier_increase_speed" ) )
			multiplier_increase_speed = (float)cfg["multiplier_increase_speed"];
		if ( cfg.value_exists( "multiplier_decrease_speed" ) )
			multiplier_decrease_speed = (float)cfg["multiplier_decrease_speed"];

		if ( cfg.value_exists( "horizontal_amplitude" ) )
			horizontal_amplitude = (float)cfg["horizontal_amplitude"];
		if ( cfg.value_exists( "vertical_amplitude" ) )
			vertical_amplitude = (float)cfg["vertical_amplitude"];
		if ( cfg.value_exists( "horizontal_peroid" ) )
			horizontal_peroid = (float)cfg["horizontal_peroid"];
		if ( cfg.value_exists( "vertical_peroid" ) )
			vertical_peroid = (float)cfg["vertical_peroid"];
	}
}; // struct breath_holding_params

STATIC_SIZE_ASSERT(breath_holding_params, 0x28);

} // namespace survarium

#endif // #ifndef BREATH_HOLDING_PARAMS_H_INCLUDED
