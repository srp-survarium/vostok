////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_RELOAD_STATE_BASE_H_INCLUDED
#define WEAPON_CORE_RELOAD_STATE_BASE_H_INCLUDED

#include <vostok/game_core/weapon_core_animation_end_aware_state.h>

namespace survarium {

class weapon_core_reload_state_base : public weapon_core_animation_end_aware_state {
public:
	explicit		weapon_core_reload_state_base	( weapon_core& weapon, float animation_time_scale );

	virtual	void	initialize						( ) override;
	virtual	void	serialize						( network_core::udp_match_packet& packet ) const override;
	virtual	void	deserialize						( network_core::packet_reader& reader ) override;
	virtual	void	on_animation_end_impl			( bool& animation_player_tick_result ) override;

private:
	/* 0x0000 */	/* weapon_core_animation_end_aware_state */
	/* 0x0140 */	float		m_animation_timescale;
}; // class weapon_core_reload_state_base

STATIC_SIZE_ASSERT(weapon_core_reload_state_base, 0x148);

} // namespace survarium

#endif // #ifndef WEAPON_CORE_RELOAD_STATE_BASE_H_INCLUDED
