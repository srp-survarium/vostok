////////////////////////////////////////////////////////////////////////////
//	Created		: 20.07.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/particle/api.h>
#include "particle_world_cooker.h"

namespace vostok {
namespace particle {

void register_particle_world_cooker		( );
void unregister_particle_world_cooker	( );

} // namespace particle
} // namespace vostok

void vostok::particle::initialize	( )
{
	register_particle_world_cooker	( );
}

void vostok::particle::finalize		( )
{
	unregister_particle_world_cooker( );
}