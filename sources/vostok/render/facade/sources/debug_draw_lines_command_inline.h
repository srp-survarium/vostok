// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_FACADE_SOURCES_DEBUG_DRAW_LINES_COMMAND_INLINE_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_DEBUG_DRAW_LINES_COMMAND_INLINE_H_INCLUDED
namespace vostok {
namespace render {
namespace debug {

inline draw_lines_command::draw_lines_command(
	memory::base_allocator& allocator,
	base_scene_ptr const& scene,
	engine::world& renderer,
	vertices_type const& vertices,
	indices_type const& indices,
	bool use_depth
)	:
	super		( true, use_depth ),
	m_vertices	( allocator, vertices.begin( ), vertices.end( ) ),
	m_indices	( allocator, indices.begin( ), indices.end( ) ),
	m_scene		( scene ),
	m_renderer	( renderer )
{
	ASSERT		( m_indices.size( ) >= m_vertices.size( ) );
}

inline draw_lines_command::draw_lines_command(
	base_scene_ptr const& scene,
	engine::world& renderer,
	memory::base_allocator& allocator,
	vertices_type_iterator start_point,
	vertices_type_iterator end_point,
	indices_type const& indices,
	bool use_depth
)	:
	super		( true, use_depth ),
	m_vertices	( allocator, start_point, end_point ),
	m_indices	( allocator, indices.begin( ), indices.end( ) ),
	m_scene		( scene ),
	m_renderer	( renderer )
{
	ASSERT		( m_indices.size( ) >= m_vertices.size( ) );
}

template < int vertex_count, int index_count >
inline draw_lines_command::draw_lines_command(
	base_scene_ptr const& scene,
	engine::world& renderer,
	memory::base_allocator& allocator,
	vertex_colored const (&vertices)[vertex_count],
	u16 const (&indices)[index_count],
	bool use_depth
)	:
	super		( true, use_depth ),
	m_vertices	( allocator, &vertices[0], &vertices[0] + vertex_count ),
	m_indices	( allocator, &indices[0], &indices[0] + index_count ),
	m_scene		( scene ),
	m_renderer	( renderer )
{
	COMPILE_ASSERT( index_count >= vertex_count, please_remove_redundant_vertices );
}

template < template < typename > class container_type >
inline draw_lines_command::draw_lines_command(
	base_scene_ptr const& scene,
	engine::world& renderer,
	memory::base_allocator& allocator,
	container_type< vertex_colored > const& vertices,
	container_type< u16 > const& indices,
	bool use_depth
)	:
	super		( true, use_depth ),
	m_vertices	( allocator, vertices.begin( ), vertices.end( ) ),
	m_indices	( allocator, indices.begin( ), indices.end( ) ),
	m_scene		( scene ),
	m_renderer	( renderer )
{
}

} // namespace debug
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_DEBUG_DRAW_LINES_COMMAND_INLINE_H_INCLUDED
