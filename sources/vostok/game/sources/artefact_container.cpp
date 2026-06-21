////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "artefact_container.h"

namespace survarium {

// claude@NOTE: structure correct (base-ctor + m_game_scene(w) ref init + compiler
// vtable stores). Residual is the anchor-sole-caller convention wall
// (anchor-sole-caller-convention.md): the only reachable construction is the
// /OPT:REF anchor (anchor_game_victory_item.cpp), so LTCG specialises the ctor to
// read the scene from the anchor's static `s_scene` (`mov edi,[s_scene]`, `ret`)
// instead of the real `[esp+4]` thiscall param (`ret 4`). Pairs at the real
// convention once create_game_objects (project_cooker_simple, parked) constructs it.
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
