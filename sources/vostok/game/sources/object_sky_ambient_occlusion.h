// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef OBJECT_SKY_AMBIENT_OCCLUSION_H_INCLUDED
#define OBJECT_SKY_AMBIENT_OCCLUSION_H_INCLUDED

#include "game_object_static.h"

namespace survarium {

class object_sky_ambient_occlusion : public game_object_static {
	typedef game_object_static super;

public:
			explicit	object_sky_ambient_occlusion	( base_game_scene& w );
	virtual				~object_sky_ambient_occlusion	( );

	virtual	void		load							(
							configs::binary_config_value const&		t,
							pcstr									__formal,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert							( ) override;
	virtual	void		remove							( ) override;

public:
	/* 0x0000 */	/* game_object_static */
	/* 0x0150 */	fixed_string< 260 >		m_texture_name;
	/* 0x0260 */	bool					m_enabled;
	/* 0x0261 */	bool					m_generated;
	/* 0x0264 */	s32						m_width;
	/* 0x0268 */	s32						m_depth;
	/* 0x026c */	s32						m_height;
	/* 0x0270 */	s32						m_resolution_x;
	/* 0x0274 */	s32						m_resolution_y;
	/* 0x0278 */	s32						m_sky_ao_volume_id;
}; // class object_sky_ambient_occlusion

STATIC_SIZE_ASSERT(object_sky_ambient_occlusion, 0x280);

} // namespace survarium

#endif // #ifndef OBJECT_SKY_AMBIENT_OCCLUSION_H_INCLUDED
