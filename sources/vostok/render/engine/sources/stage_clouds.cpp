#include "pch.h"
#include "stage_clouds.h"

#include <vostok/render/core/res_effect.h>

namespace vostok {
namespace render {

// claude@NOTE: environment_temp / cloud_key_parameters / fill_cloud_texture have no legacy
// ancestor (legacy clouds_sliced_cube 3d-density design was replaced) - matcher-phase

environment_temp::environment_temp( ) :
	keys			( 0 ),
	key_time_step	( 0.0f ),
	num_keys		( 0 ),
	time			( 0.0f )
{
	// STATE[STUB]
	// FUNCTION BODY[0x611990]
}

environment_temp::~environment_temp( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x611970]
}

cloud_key_parameters environment_temp::get_next_key( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x611610]
	return cloud_key_parameters( );
}

cloud_key_parameters environment_temp::get_interp_key( float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x611730]
	return cloud_key_parameters( );
}

void stage_clouds::fill_cloud_texture( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x611670]
}

bool stage_clouds::is_effects_ready( ) const
{
	// FUNCTION BODY[0x611650]
	return m_clouds_effect.c_ptr() != NULL;
}

stage_clouds::~stage_clouds( )
{
	// FUNCTION BODY[0x611ae0]
}

void stage_clouds::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x611d30]
	// claude@NOTE: legacy execute drew the sliced-cube density volume; shipped execute is rewritten
	// around cloud_interp_textures/god-rays - legacy body kept in temp/render_legacy remainder
}

} // namespace render
} // namespace vostok
