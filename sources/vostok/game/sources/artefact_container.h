// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ARTEFACT_CONTAINER_H_INCLUDED
#define ARTEFACT_CONTAINER_H_INCLUDED

#include <vostok/game_core/artefact_container_core.h>

namespace survarium {

class base_game_scene;

class artefact_container : public artefact_container_core {
	typedef artefact_container_core super;

public:
			explicit	artefact_container	( base_game_scene& w );

	inline	void		tick				( const u32 arg_0, const u32 arg_1 ) { /* no source */ }

	virtual	void		load				( configs::binary_config_value const& cfg ) override;

	virtual	void		activate			( generic_anomaly_core* owner, physics::world* world, scheduler& scheduler ) override;

	virtual	void		deactivate			( ) override;


private:
	/* 0x0000 */	/* artefact_container_core */
	/* 0x002c */	base_game_scene&	m_game_scene;
}; // class artefact_container

STATIC_SIZE_ASSERT(artefact_container, 0x30);

} // namespace survarium

#endif // #ifndef ARTEFACT_CONTAINER_H_INCLUDED
