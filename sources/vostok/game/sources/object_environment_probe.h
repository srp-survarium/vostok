////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_ENVIRONMENT_PROBE_H_INCLUDED
#define OBJECT_ENVIRONMENT_PROBE_H_INCLUDED

#include "game_object_static.h"

namespace survarium {

class object_environment_probe : public game_object_static {
	typedef game_object_static super;

public:
			explicit	object_environment_probe	( base_game_scene& w );
	virtual				~object_environment_probe	( );

	virtual	void		load						(
							configs::binary_config_value const&		t,
							pcstr									__formal,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert						( ) override;
	virtual	void		remove						( ) override;

public:
	/* 0x0000 */	/* game_object_static */
	/* 0x0150 */	fixed_string< 260 >		m_texture_name;
	/* 0x0260 */	u32						m_cubemap_resolution;
	/* 0x0264 */	u32						m_probe_id;
	/* 0x0268 */	u32						m_geometry;
	/* 0x026c */	float					m_radius;
	/* 0x0270 */	float					m_diffuse_multiplier;
	/* 0x0274 */	float					m_specular_multiplier;
	/* 0x0278 */	bool					m_generated;
	/* 0x0279 */	bool					m_enabled;
	/* 0x027a */	bool					m_clip_by_normal;
	/* 0x027b */	bool					m_with_shadows;
}; // class object_environment_probe

STATIC_SIZE_ASSERT(object_environment_probe, 0x280);

} // namespace survarium

#endif // #ifndef OBJECT_ENVIRONMENT_PROBE_H_INCLUDED
