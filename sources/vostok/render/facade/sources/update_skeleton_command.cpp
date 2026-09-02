// SPDX-License-Identifier: GPL-3.0-or-later
#include "pch.h"
#include "update_skeleton_command.h"
#include <vostok/render/engine/world.h>
namespace vostok {
namespace render {

update_skeleton_command::update_skeleton_command(
	engine::world& world,
	render_model_instance_ptr instance,
	float4x4 const* matrices,
	u32 count
)
	: m_model_instance( instance ),
	  m_world( world ),
	  m_matrices_count( count )
{
	u32 const buff_size		= sizeof(float4x4)*count;
	memory::copy			( m_matrices, buff_size, matrices, buff_size);
}

void update_skeleton_command::execute( )
{
	m_world.update_skeleton	( m_model_instance, m_matrices, m_matrices_count );
}

} // namespace render
} // namespace vostok
