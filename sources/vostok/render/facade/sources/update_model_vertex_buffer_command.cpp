#include "pch.h"
#include "update_model_vertex_buffer_command.h"
#include <vostok/render/engine/world.h>

namespace vostok {
namespace render {

update_model_vertex_buffer_command::~update_model_vertex_buffer_command( )
{
	// FUNCTION BODY[0x757200]
	fragments_type::iterator		it	= m_fragments.begin();
	fragments_type::const_iterator	end	= m_fragments.end();

	for( ; it != end; ++it)
	{
		VOSTOK_DELETE_ARRAY_IMPL( m_allocator, it->buffer);
	}
}

void update_model_vertex_buffer_command::execute( )
{
	// FUNCTION BODY[0x7571f0]
	ASSERT								( m_object );
	m_world.update_model_vertex_buffer	( m_object, m_fragments);
}

} // namespace render
} // namespace vostok
