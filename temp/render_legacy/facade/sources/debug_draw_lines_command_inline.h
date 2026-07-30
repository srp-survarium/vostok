////////////////////////////////////////////////////////////////////////////
//	Created 	: 18.11.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef DEBUG_DRAW_LINES_COMMAND_INLINE_H_INCLUDED
#define DEBUG_DRAW_LINES_COMMAND_INLINE_H_INCLUDED

namespace vostok {
namespace render {
namespace debug {

inline draw_lines_command::draw_lines_command	(
		scene_ptr const& scene,
		engine::world& renderer,
		memory::base_allocator& allocator,
		vertices_type_iterator start_point,
		vertices_type_iterator end_point,
		indices_type const& indices,
		bool use_depth
	) :
	super		( true, use_depth ),
	m_scene		( scene ),
	m_renderer	( renderer ),
	m_vertices	( allocator, start_point, end_point ),
	m_indices	( allocator, indices.begin(), indices.end() )
{
	ASSERT		( m_indices.size( ) >= m_vertices.size( ) );
}

} // namespace debug
} // namespace render
} // namespace vostok

#endif // #ifndef DEBUG_DRAW_LINES_COMMAND_INLINE_H_INCLUDED
