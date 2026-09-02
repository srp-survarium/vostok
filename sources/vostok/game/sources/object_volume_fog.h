// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef OBJECT_VOLUME_FOG_H_INCLUDED
#define OBJECT_VOLUME_FOG_H_INCLUDED

#include "game_object_static.h"

namespace survarium {

class object_volume_fog : public game_object_static {
	typedef game_object_static super;

public:
			explicit	object_volume_fog	( base_game_scene& w );
	virtual				~object_volume_fog	( );

	virtual	void		load				(
							configs::binary_config_value const&		t,
							pcstr									__formal,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert				( ) override;
	virtual	void		remove				( ) override;

public:
	/* 0x0000 */	/* game_object_static */
	/* 0x0150 */	u32			m_volume_fog_id;
	/* 0x0154 */	float3		m_color;
	/* 0x0160 */	float		m_density;
	/* 0x0164 */	float		m_speed;
	/* 0x0168 */	float		m_noise_scale;
	/* 0x016c */	float		m_wave_length;
	/* 0x0170 */	float		m_near_density;
	/* 0x0174 */	float		m_transparency_multiplier;
	/* 0x0178 */	float		m_density_offset;
	/* 0x017c */	float		m_height_falloff_offset;
	/* 0x0180 */	float2		m_direction;
}; // class object_volume_fog

STATIC_SIZE_ASSERT(object_volume_fog, 0x188);

} // namespace survarium

#endif // #ifndef OBJECT_VOLUME_FOG_H_INCLUDED
