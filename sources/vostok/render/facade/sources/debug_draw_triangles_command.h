#ifndef VOSTOK_RENDER_FACADE_SOURCES_DEBUG_DRAW_TRIANGLES_COMMAND_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_DEBUG_DRAW_TRIANGLES_COMMAND_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <vostok/render/engine/vertex_colored.h>
#include <vostok/render/facade/base_command.h>
#include <vostok/vectora.h>

namespace vostok {

namespace memory {
class base_allocator;
}

namespace render {

namespace engine {
class world;
}

namespace debug {

class draw_triangles_command : public base_command, private boost::noncopyable {
public:
	typedef base_command super;
	typedef vectora< vertex_colored > vertices_type;
	typedef vectora< u16 > indices_type;

	// STATE[STUB]
	inline draw_triangles_command(
		base_scene_ptr const& scene,
		engine::world& renderer,
		vertices_type const& vertices,
		indices_type const& indices,
		bool use_depth = true
	)
		: super( true, use_depth ),
		  m_vertices( vertices ),
		  m_indices( indices ),
		  m_scene( scene ),
		  m_renderer( renderer )
	{
		// FUNCTION BODY[0x16b80]
	}

	template < int vertex_count, int index_count >
	// STATE[STUB]
	inline draw_triangles_command(
		base_scene_ptr const& scene,
		engine::world& renderer,
		memory::base_allocator& allocator,
		vertex_colored const (&vertices)[vertex_count],
		u16 const (&indices)[index_count],
		bool use_depth = true
	)
		: super( true, use_depth ),
		  m_vertices( allocator, &vertices[0], &vertices[0] + vertex_count ),
		  m_indices( allocator, &indices[0], &indices[0] + index_count ),
		  m_scene( scene ),
		  m_renderer( renderer )
	{
		// FUNCTION BODY[0x16e70]
		COMPILE_ASSERT( index_count >= vertex_count, please_remove_redundant_vertices );
	}

	virtual ~draw_triangles_command( );
	virtual void execute( );
	virtual void defer_execution( );

private:
	vertices_type m_vertices;
	indices_type m_indices;
	base_scene_ptr m_scene;
	engine::world& m_renderer;
};

STATIC_SIZE_ASSERT( draw_triangles_command, 0x7c );

} // namespace debug
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_DEBUG_DRAW_TRIANGLES_COMMAND_H_INCLUDED
