////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_SPACE_GRAPH_WRAPPER_H_INCLUDED
#define ANIMATION_SPACE_GRAPH_WRAPPER_H_INCLUDED

/* INCLUDES */
struct survarium::animation_space_vertex_id;
typedef vostok::resources::resource_ptr<survarium::animation_space_graph,vostok::resources::unmanaged_intrusive_base>
	survarium::animation_space_graph_ptr;
class survarium::animation_space_graph;

namespace survarium {

struct animation_space_graph_wrapper : public boost::noncopyable {
	inline									animation_space_graph_wrapper	( animation_space_graph_ptr const& arg_0, animation_space_vertex_id const& arg_1 ) { /* no source */ }

	inline	void							on_before_search				( ) { /* no source */ }
	inline	void							on_after_search					( const bool arg_0 ) { /* no source */ }

	inline	animation_space_vertex_id		vertex_id						( animation_space_vertex_id const& vertex_id, const u32 iterator ) const { /* no source */ }
	inline	u32								edge_id							( animation_space_vertex_id const& arg_0, const u32 arg_1 ) const { /* no source */ }

	inline									~animation_space_graph_wrapper	( ) { /* no source */ }

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	animation_space_graph_ptr const&	m_graph;
	/* 0x0004 */	animation_space_vertex_id const&	m_start_vertex_id;
}; // struct animation_space_graph_wrapper

STATIC_SIZE_ASSERT(animation_space_graph_wrapper, 0x8);

} // namespace survarium

#endif // #ifndef ANIMATION_SPACE_GRAPH_WRAPPER_H_INCLUDED
