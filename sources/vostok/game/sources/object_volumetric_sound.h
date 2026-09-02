// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OBJECT_VOLUMETRIC_SOUND_H_INCLUDED
#define OBJECT_VOLUMETRIC_SOUND_H_INCLUDED

#include "object_sound.h"
#include <vostok/game_core/link_resolver.h>

namespace survarium {

class base_project;
class collision_geometry;

class object_volumetric_sound : public object_sound , public link_resolver {
	typedef object_sound super;

public:
			explicit	object_volumetric_sound	( base_game_scene& w );
	virtual				~object_volumetric_sound( );

	virtual	void		load					(
							configs::binary_config_value const&		t,
							pcstr									project_resources_path,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert					( ) override;

	virtual	void		resolve_links			( base_project* p, configs::binary_config_value config ) override;

private:
	/* 0x0000 */	/* object_sound */
	/* 0x0160 */	/* link_resolver */
	/* 0x0164 */	collision_geometry*		m_collision_geometry;
	/* 0x0168 */	float					m_radius;
}; // class object_volumetric_sound

STATIC_SIZE_ASSERT(object_volumetric_sound, 0x170);

} // namespace survarium

#endif // #ifndef OBJECT_VOLUMETRIC_SOUND_H_INCLUDED
