#include "pch.h"
#include "update_model_vertex_buffer_command.h"
#include <vostok/render/engine/world.h>

namespace vostok {
namespace render {

update_model_vertex_buffer_command::~update_model_vertex_buffer_command( )
{
	fragments_type::iterator		it	= m_fragments.begin();

	for( ; it != m_fragments.end(); ++it)
	{
		VOSTOK_DELETE_ARRAY_IMPL( m_allocator, it->buffer);
	}
}

void update_model_vertex_buffer_command::execute( )
{
	ASSERT								( m_object );
	m_world.update_model_vertex_buffer	( m_object, m_fragments);
}

} // namespace render
} // namespace vostok
