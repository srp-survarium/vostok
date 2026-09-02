// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PLAYER_STEALTH_H_INCLUDED
#define PLAYER_STEALTH_H_INCLUDED

namespace survarium {

struct player_input;


class player_stealth {
public:
								player_stealth				( );
			explicit			player_stealth				( player_stealth const& other );

			player_stealth&		operator=					( player_stealth const& other );

			void				load						( configs::binary_config_value const& config );
	inline	bool				can_be_detected				( player_stealth const& arg_0, player_input const& arg_1, const float arg_2 ) const { /* no source */ }

	inline	float				get_armor_value				( ) const { /* no source */ }
	inline	float				get_optics_value			( ) const { /* no source */ }
	inline	float				get_armor_sound_value		( ) const { /* no source */ }
	inline	float				get_detection_level			( ) const { /* no source */ }
	inline	float				get_always_visible_distance	( ) const { /* no source */ }


private:
	/* 0x0000 */	float		m_default_value;
	/* 0x0004 */	float		m_default_sound_value;
	/* 0x0008 */	float		m_stand_factor;
	/* 0x000c */	float		m_crouch_factor;
	/* 0x0010 */	float		m_crouch_sound_factor;
	/* 0x0014 */	float		m_walk_factor;
	/* 0x0018 */	float		m_walk_sound_factor;
	/* 0x001c */	float		m_sprint_factor;
	/* 0x0020 */	float		m_sprint_sound_factor;
	/* 0x0024 */	float		m_detection_level;
	/* 0x0028 */	float		m_always_visible_distance;
}; // class player_stealth

STATIC_SIZE_ASSERT(player_stealth, 0x2C);

} // namespace survarium

#endif // #ifndef PLAYER_STEALTH_H_INCLUDED
