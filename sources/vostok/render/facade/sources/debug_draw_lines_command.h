#ifndef VOSTOK_RENDER_FACADE_SOURCES_DEBUG_DRAW_LINES_COMMAND_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_DEBUG_DRAW_LINES_COMMAND_H_INCLUDED

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

class draw_lines_command : public base_command, private boost::noncopyable {
public:
	typedef base_command super;
	typedef vectora< vertex_colored > vertices_type;
	typedef vectora< u16 > indices_type;

	inline draw_lines_command(
		memory::base_allocator& allocator,
		base_scene_ptr const& scene,
		engine::world& renderer,
		vertices_type const& vertices,
		indices_type const& indices,
		bool use_depth = true
	)
		: super( true, use_depth ),
		  m_vertices( allocator, vertices.begin( ), vertices.end( ) ),
		  m_indices( allocator, indices.begin( ), indices.end( ) ),
		  m_scene( scene ),
		  m_renderer( renderer )
	{
		// FUNCTION BODY[0x16a70]
		ASSERT		( m_indices.size( ) >= m_vertices.size( ) );
	}

	template < int vertex_count, int index_count >
	inline draw_lines_command(
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
		// FUNCTION BODY[0x16d50]
		COMPILE_ASSERT( index_count >= vertex_count, please_remove_redundant_vertices );
	}

	template < template < typename > class container_type >
	inline draw_lines_command(
		base_scene_ptr const& scene,
		engine::world& renderer,
		memory::base_allocator& allocator,
		container_type< vertex_colored > const& vertices,
		container_type< u16 > const& indices,
		bool use_depth = true
	)
		: super( true, use_depth ),
		  m_vertices( allocator, vertices.begin( ), vertices.end( ) ),
		  m_indices( allocator, indices.begin( ), indices.end( ) ),
		  m_scene( scene ),
		  m_renderer( renderer )
	{
		// FUNCTION BODY[0x16f70]
	}

	virtual void execute( );
	virtual void defer_execution( );

private:
	vertices_type m_vertices;
	indices_type m_indices;
	base_scene_ptr m_scene;
	engine::world& m_renderer;
};

STATIC_SIZE_ASSERT( draw_lines_command, 0x7c );

} // namespace debug
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_DEBUG_DRAW_LINES_COMMAND_H_INCLUDED
