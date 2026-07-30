#include "pch.h"
#include "stage_rain.h"

namespace vostok {
namespace render {

stage_rain::stage_rain(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage( in_renderer, context ),
	m_rain_geometry( 16, 16 ),
	m_rain_offsets( 0 ),
	m_camera_offset_view( 0.0f ),
	m_camera_offset_right( 0.0f ),
	m_shadow_map_size( 0 ),
	m_previous_view_position( 0.0f, 0.0f, 0.0f ),
	m_moving_direction( 0.0f, 0.0f, 0.0f ),
	m_radius_parameter( 0 ),
	m_rain_speed_parameter( 0 ),
	m_rain_density_parameter( 0 ),
	m_rain_uv_scales_parameter( 0 ),
	m_view_to_shadow_parameter( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x60def0]
}

bool stage_rain::is_effects_ready( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x60d430]
	return false;
}

stage_rain::~stage_rain( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x60d480]
}

void stage_rain::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x60d510]
}

} // namespace render
} // namespace vostok
