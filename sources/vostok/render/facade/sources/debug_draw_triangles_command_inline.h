// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_FACADE_SOURCES_DEBUG_DRAW_TRIANGLES_COMMAND_INLINE_H_INCLUDED
#define VOSTOK_RENDER_FACADE_SOURCES_DEBUG_DRAW_TRIANGLES_COMMAND_INLINE_H_INCLUDED
namespace vostok {
namespace render {
namespace debug {

inline draw_triangles_command::draw_triangles_command(
	base_scene_ptr const& scene,
	engine::world& renderer,
	vertices_type const& vertices,
	indices_type const& indices,
	bool use_depth
)	:
	super		( true, use_depth ),
	m_vertices	( vertices ),
	m_indices	( indices ),
	m_scene		( scene ),
	m_renderer	( renderer )
{
	ASSERT		( m_indices.size( ) >= m_vertices.size( ) );
}

template < int vertex_count, int index_count >
inline draw_triangles_command::draw_triangles_command(
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

} // namespace debug
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_SOURCES_DEBUG_DRAW_TRIANGLES_COMMAND_INLINE_H_INCLUDED
