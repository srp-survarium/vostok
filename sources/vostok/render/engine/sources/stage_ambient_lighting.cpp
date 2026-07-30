#include "pch.h"
#include "stage_ambient_lighting.h"

#include "effect_environment_probe_lighting.h"
#include "effect_sky_ambient_occlusion.h"

namespace vostok {
namespace render {

stage_ambient_lighting::stage_ambient_lighting(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage					( in_renderer, context ),
	m_ambient_multiplier	( 0.0f ),
	m_use_probes			( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62a800]
}

bool stage_ambient_lighting::is_effects_ready( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x629a20]
	return false;
}

void stage_ambient_lighting::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x629a80]
}

} // namespace render
} // namespace vostok
