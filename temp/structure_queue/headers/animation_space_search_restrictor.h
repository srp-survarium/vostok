////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_SPACE_SEARCH_RESTRICTOR_H_INCLUDED
#define ANIMATION_SPACE_SEARCH_RESTRICTOR_H_INCLUDED

/* INCLUDES */
class vostok::ai::search_restrictor::generic<survarium::animation_space_vertex_id,unsigned int,float>;
typedef vostok::resources::resource_ptr<survarium::animation_space_graph,vostok::resources::unmanaged_intrusive_base>
	survarium::animation_space_graph_ptr;
class survarium::animation_space_graph;

/* FORWARD REFS */
class survarium::animation_space_vertex_id;

namespace survarium {

class animation_space_search_restrictor : public ai::search_restrictor::generic< animation_space_vertex_id, u32, float > {
public:
	inline			animation_space_search_restrictor	(
						animation_space_graph_ptr const&	arg_0,
						animation_space_vertex_id const&	arg_1,
						animation_space_vertex_id const&	arg_2,
						float const&						arg_3,
						const u32							arg_4,
						const u32							arg_5
					) { /* no source */ }

	inline	u32		get_start_vertices_count			( ) const { /* no source */ }

	inline			~animation_space_search_restrictor	( ) { /* no source */ }

private:
	/* 0x0000 */	/* ai::search_restrictor::generic< animation_space_vertex_id, u32, float > */
	/* 0x0014 */	animation_space_graph_ptr const&	m_graph;
}; // class animation_space_search_restrictor

STATIC_SIZE_ASSERT(animation_space_search_restrictor, 0x18);

} // namespace survarium

#endif // #ifndef ANIMATION_SPACE_SEARCH_RESTRICTOR_H_INCLUDED
