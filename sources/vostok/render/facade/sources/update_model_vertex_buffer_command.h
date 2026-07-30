#ifndef VOSTOK_RENDER_FACADE_SOURCES_UPDATE_MODEL_VERTEX_BUFFER_COMMAND_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_UPDATE_MODEL_VERTEX_BUFFER_COMMAND_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/render/facade/base_command.h>
#include <vostok/render/facade/common_types.h>
#include <vostok/render/facade/model.h>
#include <vostok/vectora.h>

namespace vostok {

namespace memory {
class base_allocator;
}

namespace render {

namespace engine {
class world;
}

class update_model_vertex_buffer_command :
	public base_command,
	private boost::noncopyable
{
public:
	typedef vectora< buffer_fragment > fragments_type;

	inline update_model_vertex_buffer_command(
		engine::world& world,
		render_model_instance_ptr object,
		fragments_type const& fragments,
		memory::base_allocator& allocator
	)
		: m_fragments( allocator ),
		  m_object( object ),
		  m_world( world ),
		  m_allocator( allocator )
	{
		ASSERT				( m_object );
		m_fragments.resize	( fragments.size() );
		fragments_type::iterator		it	= m_fragments.begin();
		fragments_type::const_iterator	end	= m_fragments.end();

		fragments_type::const_iterator it_o	= fragments.begin();

		for( ; it != end; ++it, ++it_o)
		{
			*it = *it_o;
			it->buffer = VOSTOK_NEW_ARRAY_IMPL( m_allocator, char, it_o->size);
			memory::copy( it->buffer, it_o->size, it_o->buffer, it_o->size);
		}
	}

	virtual ~update_model_vertex_buffer_command( );
	virtual void execute( );

private:
	fragments_type m_fragments;
	render_model_instance_ptr m_object;
	engine::world& m_world;
	memory::base_allocator& m_allocator;
};

STATIC_SIZE_ASSERT( update_model_vertex_buffer_command, 0x70 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_UPDATE_MODEL_VERTEX_BUFFER_COMMAND_H_INCLUDED
