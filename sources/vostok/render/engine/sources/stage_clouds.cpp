#include "pch.h"
#include "stage_clouds.h"

namespace vostok {
namespace render {

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
	// STATE[STUB]
	// FUNCTION BODY[0x611650]
	return false;
}

stage_clouds::~stage_clouds( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x611ae0]
}

void stage_clouds::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x611d30]
}

} // namespace render
} // namespace vostok
