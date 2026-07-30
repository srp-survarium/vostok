#include "pch.h"
#include "clouds.h"

namespace vostok {
namespace render {

clouds::clouds( ) :
	m_num_keys			( 0 ),
	m_is_updated		( false ),
	m_interp_alpha		( 0.0f ),
	m_tasks_type		( 0 ),
	m_key_time_step	( 0.0f ),
	m_time				( 0.0f ),
	m_current_key_0	( 0 ),
	m_current_key_1	( 0 ),
	m_invalid			( false ),
	m_sun_direction	( 0.0f, 0.0f, 0.0f ),
	m_is_editor_mode	( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63b3a0]
}

void clouds::generate_cloud_right( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63b330]
}

void clouds::set_editor_mode( bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63b260]
}

void clouds::initialize( cloud_parameters const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63bc70]
}

u32 clouds::get_next_index_of( u32 ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x63b290]
	return 0;
}

void clouds::invalidate( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63b250]
}

void clouds::set_sun_direction( float3 const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63b2a0]
}

void clouds::set_time( float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63b570]
}

void clouds::set_num_keys( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63b240]
}

void clouds::set_key( u32, cloud_key_parameters const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x63b270]
}

} // namespace render
} // namespace vostok
