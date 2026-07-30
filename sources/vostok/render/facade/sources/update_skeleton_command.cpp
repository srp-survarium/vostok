#include "pch.h"
#include "update_skeleton_command.h"

namespace vostok {
namespace render {

// STATE[STUB]
update_skeleton_command::update_skeleton_command(
	engine::world& world,
	render_model_instance_ptr instance,
	float4x4 const*,
	u32 count
)
	: m_model_instance( instance ),
	  m_world( world ),
	  m_matrices_count( count )
{
	// FUNCTION BODY[0x757290]
}

// STATE[STUB]
void update_skeleton_command::execute( )
{
	// FUNCTION BODY[0x757270]
}

} // namespace render
} // namespace vostok
