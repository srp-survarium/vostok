// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GENERIC_ANOMALY_H_INCLUDED
#define GENERIC_ANOMALY_H_INCLUDED

#include <vostok/game_core/generic_anomaly_core.h>

namespace survarium {

class base_game_scene;

class generic_anomaly : public generic_anomaly_core {
	typedef generic_anomaly_core super;

public:
			explicit	generic_anomaly	( base_game_scene& w );

	virtual	void		tick			( const u32 time_delta_ms, const u32 current_time_ms ) override;

	// the core's load is NON-virtual; this virtual shadows it (PDB shape)
	virtual	void		load			( configs::binary_config_value const& config );

	// class-elaborated: the inherited scheduler() accessor hides the type name
	virtual	void		activate		( physics::world* world, class scheduler& scheduler ) override;

	virtual	void		deactivate		( ) override;


private:
	/* 0x0000 */	/* generic_anomaly_core */
	/* 0x007c */	base_game_scene&	m_game_scene;
}; // class generic_anomaly

STATIC_SIZE_ASSERT(generic_anomaly, 0x80);

} // namespace survarium

#endif // #ifndef GENERIC_ANOMALY_H_INCLUDED
