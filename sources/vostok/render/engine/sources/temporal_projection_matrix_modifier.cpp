#include "pch.h"
// claude@NOTE: legacy-harvest disposition: no temp/render_legacy ancestor (temporal jitter is new-in-target) - matcher-phase work.
#include "temporal_projection_matrix_modifier.h"

namespace vostok {
namespace render {

temporal_projection_matrix_modifier::temporal_projection_matrix_modifier(
	renderer_context* const context,
	u32 const window_size_x,
	u32 const window_size_y,
	bool const need_modify
) :
	m_renderer_context( context ),
	m_window_size_x( window_size_x ),
	m_window_size_y( window_size_y ),
	m_need_modify( need_modify ),
	m_jittered( false )
{
	// STATE[STUB]
	// FUNCTION BODY[0x602b30]
}

void temporal_projection_matrix_modifier::push_jittering( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x602b80]
}

void temporal_projection_matrix_modifier::pop_jittering( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x602b50]
}

} // namespace render
} // namespace vostok
