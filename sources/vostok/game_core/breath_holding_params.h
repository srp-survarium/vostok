////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BREATH_HOLDING_PARAMS_H_INCLUDED
#define BREATH_HOLDING_PARAMS_H_INCLUDED

namespace survarium {

struct breath_holding_params {
	inline	explicit	breath_holding_params	( ) { }

			void		load					( configs::binary_config_value const& cfg )
	{
		// FUNCTION BODY[0x90c80]: 21
		// <0x90c86>|0x006|+0x015:'41'
		// <0x90c9b>|0x01b|+0x020:'42'
		// <0x90cbb>|0x03b|+0x010:'43'
		// <0x90ccb>|0x04b|+0x021:'44'
		// <0x90cec>|0x06c|+0x010:'45'
		// <0x90cfc>|0x07c|+0x021:'46'
		// <0x90d1d>|0x09d|+0x010:'47'
		// <0x90d2d>|0x0ad|+0x021:'48'
		// <0x90d4e>|0x0ce|+0x010:'49'
		// <0x90d5e>|0x0de|+0x021:'50'
		// <0x90d7f>|0x0ff|+0x010:'51'
		// <0x90d8f>|0x10f|+0x021:'52'
		// <0>
		// <0x90db0>|0x130|+0x010:'54'
		// <0x90dc0>|0x140|+0x021:'55'
		// <0x90de1>|0x161|+0x010:'56'
		// <0x90df1>|0x171|+0x021:'57'
		// <0x90e12>|0x192|+0x010:'58'
		// <0x90e22>|0x1a2|+0x021:'59'
		// <0x90e43>|0x1c3|+0x010:'60'
		// <0x90e53>|0x1d3|+0x020:'61'
		// <0x90e73>|0x1f3|-0x003:'61'
		// <0x90e70>|0x1f0|+0x00e:'62'
		// ******
	}


public:
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
}; // struct breath_holding_params

STATIC_SIZE_ASSERT(breath_holding_params, 0x28);

} // namespace survarium

#endif // #ifndef BREATH_HOLDING_PARAMS_H_INCLUDED
