// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "artefact_container.h"

namespace survarium {

artefact_container::artefact_container( base_game_scene& w ) :
	m_game_scene( w )
{
}

void artefact_container::load( configs::binary_config_value const& cfg )
{
	artefact_container_core::load( cfg );
}

void artefact_container::activate( generic_anomaly_core* owner, physics::world* world, scheduler& scheduler )
{
	artefact_container_core::activate( owner, world, scheduler );
}

void artefact_container::deactivate( )
{
	artefact_container_core::deactivate( );
}


} // namespace survarium
