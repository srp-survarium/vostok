#include "pch.h"
#include "stage_ambient_occlusion.h"

#include "render_target.h"

namespace vostok {
namespace render {

stage_ambient_occlusion::stage_ambient_occlusion(
	renderer*			in_renderer,
	renderer_context*	context
) :
	stage						( in_renderer, context ),
	m_ao_parameters				( 0 ),
	m_c_eye_ray_corner			( 0 ),
	m_prev_view_parameter		( 0 ),
	m_prev_ssao_valid_parameter	( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70c950]
}

bool stage_ambient_occlusion::is_effects_ready( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x70be40]
	return false;
}

static void fill_surface(
	render_target_ptr,
	renderer_context*,
	bool
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x70bf50]
}

void stage_ambient_occlusion::execute_disabled( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70be60]
}

void stage_ambient_occlusion::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70c260]
}

} // namespace render
} // namespace vostok
