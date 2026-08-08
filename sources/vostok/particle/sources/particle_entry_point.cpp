////////////////////////////////////////////////////////////////////////////
//	Created		: 20.07.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/particle/api.h>
#include <vostok/particle/world.h>
#include "particle_world_cooker.h"
#include "particle_system_instance_impl.h"

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

bool vostok::particle::is_playing( particle_system_instance_ptr const& instance )
{
	particle_system_instance_impl* const instance_impl = static_cast_checked< particle_system_instance_impl* >( instance.c_ptr( ) );
	return instance_impl->is_playing( );
}
