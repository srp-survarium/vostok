// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ARTEFACT_CONTAINER_CORE_H_INCLUDED
#define ARTEFACT_CONTAINER_CORE_H_INCLUDED

#include <vostok/game_core/usable_object.h>
#include <vostok/game_core/artefact_base.h>

namespace vostok {
namespace physics {
	struct world;
}
}

namespace survarium {

class inventory_holder;

class generic_anomaly_core;
class scheduler;
struct usable_object_user_data;


class artefact_container_core : public usable_object {
public:
	typedef usable_object super;

						artefact_container_core		( );

	virtual	void		load						( configs::binary_config_value const& cfg ) override;

	virtual	bool		use_initialize				( usable_object_user_data* user ) override;
	virtual	bool		use_execute					( usable_object_user_data* user ) override;
	virtual	bool		use_finalize				( usable_object_user_data* user ) override;
	// claude@MATCH: target is FRAMELESS for this `this`-unused leaf (`mov eax,lit;ret 4`); /Od always frames -> unpairable. Literal correct.
	virtual	pcstr		use_info					( usable_object_user_data* user ) override { VOSTOK_UNREFERENCED_PARAMETER( user ); return "st_use_artefact_container"; }

	virtual	void		activate					( generic_anomaly_core* owner, physics::world* world, scheduler& __formal );
	virtual	void		deactivate					( );

public:
	inline	bool		has_artefact				( ) const { return m_artefact; }
			void		spawn_artefact				( );

protected:
			void		transfer_artefact			( inventory_holder* holder );
			void		artefact_spawned			( resources::queries_result& data );

public:
protected:
	/* 0x0000 */	/* usable_object */
	/* 0x0020 */	artefact_base_ptr			m_artefact;
	/* 0x0024 */	u32							m_artefact_search_time_ms;
	/* 0x0028 */	generic_anomaly_core*		m_owner;
}; // class artefact_container_core

STATIC_SIZE_ASSERT(artefact_container_core, 0x2C);

} // namespace survarium

#endif // #ifndef ARTEFACT_CONTAINER_CORE_H_INCLUDED
