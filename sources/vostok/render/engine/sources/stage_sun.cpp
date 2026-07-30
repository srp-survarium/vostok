#include "pch.h"
#include "convex_volume.h"
#include "stage_sun.h"

namespace vostok {
namespace render {

stage_sun::stage_sun(
	renderer*				in_renderer,
	renderer_context*		in_context,
	cloud_interp_textures&	cloud_interp_textures,
	cloud_simulation&		simulation
) :
	stage						( in_renderer, in_context ),
	m_cloud_interp_textures		( cloud_interp_textures ),
	m_simulation				( simulation )
{
	// STATE[STUB]
	// FUNCTION BODY[0x626a70]
}

bool stage_sun::is_effects_ready( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x626370]
	return false;
}

void stage_sun::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x626380]
}

} // namespace render
} // namespace vostok
