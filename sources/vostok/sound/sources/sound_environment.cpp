// SPDX-License-Identifier: GPL-3.0-or-later

#include "pch.h"
#include "sound_environment.h"
#include <vostok/collision/api.h>

namespace vostok {
namespace sound {

sound_environment::sound_environment	( u32 env_params_id ) :
	m_env_params_id					( env_params_id )
{


	collision::geometry_instance* instance	= &*collision::new_box_geometry_instance( g_allocator, math::create_scale( float3( 1.f, 1.f, 1.f ) ) );
	m_collision								= &*collision::new_collision_object
														( g_allocator, 1, instance, this );
}

sound_environment::~sound_environment	( )
{
}

} // namespace sound
} // namespace vostok
