////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GENERIC_ANOMALY_H_INCLUDED
#define GENERIC_ANOMALY_H_INCLUDED

/* INCLUDES */
class survarium::base_game_scene;
class survarium::generic_anomaly_core;

/* FORWARD REFS */
class vostok::configs::binary_config_value;
class vostok::physics::world;
class survarium::scheduler;

namespace survarium {

class generic_anomaly : public generic_anomaly_core {
public:
			explicit	generic_anomaly	( base_game_scene& w );

	virtual	void		tick			( const u32 time_delta_ms, const u32 current_time_ms ) override;

	virtual	void		load			( configs::binary_config_value const& config );

	virtual	void		activate		( physics::world* world, scheduler& scheduler ) override;

	virtual	void		deactivate		( ) override;

	virtual				~generic_anomaly( ) { /* no source */ }

private:
	/* 0x0000 */	/* generic_anomaly_core */
	/* 0x007c */	base_game_scene&	m_game_scene;
}; // class generic_anomaly

STATIC_SIZE_ASSERT(generic_anomaly, 0x80);

} // namespace survarium

#endif // #ifndef GENERIC_ANOMALY_H_INCLUDED
