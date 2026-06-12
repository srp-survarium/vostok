////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATIONS_SEARCH_SERVICE_H_INCLUDED
#define ANIMATIONS_SEARCH_SERVICE_H_INCLUDED

/* INCLUDES */
class const survarium::animation_space_vertex_id;
struct survarium::animations_search_service::priority_queue_impl_type;
struct survarium::animations_search_service::vertex_allocator_impl_type;
struct survarium::animations_search_service::vertex_manager_impl_type;
class survarium::animation_space_graph;

/* FORWARD REFS */
class survarium::vector<unsigned int>;

namespace survarium {

class animations_search_service {
public:
	inline			animations_search_service	( ) { /* no source */ }

			bool	search						(
						animation_space_graph_ptr const&	graph,
						vector< u32 >*						path,
						const animation_space_vertex_id		arg_2 /* animation_space_vertex_id start_vertex_id */,
						const animation_space_vertex_id		arg_3 /* animation_space_vertex_id target_vertex_id */
					);

	inline			~animations_search_service	( ) { /* no source */ }

private:
	/* 0x0000 */	animations_search_service::vertex_allocator_impl_type	m_vertex_allocator;
	/* 0x0010 */	animations_search_service::vertex_manager_impl_type	m_vertex_manager;
	/* 0x002c */	animations_search_service::priority_queue_impl_type	m_priority_queue;
}; // class animations_search_service

STATIC_SIZE_ASSERT(animations_search_service, 0x3C);

} // namespace survarium

#endif // #ifndef ANIMATIONS_SEARCH_SERVICE_H_INCLUDED
