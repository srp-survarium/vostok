#include "pch.h"
#include "stage_pre_lighting.h"
#include <vostok/render/core/backend.h>
#include "renderer_context.h"
#include "renderer_context_targets.h"

namespace vostok {
namespace render {

// claude@NOTE: the defining header of stencil_light_marker_shift did not
// survive in the legacy corpus (older render_dx10 generation only had
// stencil_light_marker_start = 0x05); shift value is a buildability
// placeholder - confirm the immediate from target asm at 0x7b21e0.
const u32 stencil_light_marker_shift = 5;

void stage_pre_lighting::execute( )
{
	// FUNCTION BODY[0x7b21e0]
	if (!is_enabled())
	{
		execute_disabled();
		return;
	}

	m_context->m_light_marker_id = 1<<stencil_light_marker_shift;
	backend::ref().set_render_targets( &*m_context->m_targets->m_family[rt_accumulator_diffuse].target, &*m_context->m_targets->m_family[rt_accumulator_specular].target, 0, 0);
	backend::ref().reset_depth_stencil_target();

	backend::ref().clear_render_targets( math::color( 0.0f, 0.0f, 0.0f, 0.0f));
//	backend::ref().clear_depth_stencil( D3D_CLEAR_DEPTH | D3D_CLEAR_STENCIL, 1.0f, 0);

	m_context->set_w					( float4x4().identity() );

	backend::ref().reset_render_targets();
	backend::ref().reset_depth_stencil_target();
}

} // namespace render
} // namespace vostok
