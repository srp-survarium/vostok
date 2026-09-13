// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include <vostok/game_core/game_net_defines.h>
#include "generic_anomaly.h"

namespace survarium {

generic_anomaly::generic_anomaly( base_game_scene& w ) :
	m_game_scene( w )
{
}

void generic_anomaly::load( configs::binary_config_value const& config )
{
	generic_anomaly_core::load( config );
}

// class-elaborated: the inherited scheduler() accessor hides the type name
void generic_anomaly::activate( physics::world* world, class scheduler& scheduler )
{
	generic_anomaly_core::activate( world, scheduler );
}

void generic_anomaly::deactivate( )
{
	generic_anomaly_core::deactivate( );
}

void generic_anomaly::tick( const u32 time_delta_ms, const u32 current_time_ms )
{
	generic_anomaly_core::tick( time_delta_ms, current_time_ms );
}


} // namespace survarium
