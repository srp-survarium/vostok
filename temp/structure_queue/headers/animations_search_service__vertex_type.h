////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATIONS_SEARCH_SERVICE_VERTEX_TYPE_H_INCLUDED
#define ANIMATIONS_SEARCH_SERVICE_VERTEX_TYPE_H_INCLUDED

/* INCLUDES */
struct vostok::ai::a_star::vertex_helper<float>::vertex_impl;
struct vostok::ai::path_constructor::edge::helper<unsigned int>::vertex_impl<survarium::animations_search_service::vertex_type>;
struct vostok::ai::priority_queue::binary_heap::vertex_impl;
struct vostok::ai::vertex_allocator::fixed_count::vertex_impl;
struct vostok::ai::vertex_manager::fixed_count_hash::vertex_helper<survarium::animation_space_vertex_id>::vertex_impl;
struct survarium::animation_space_graph_wrapper::vertex_impl;

namespace survarium {

struct animations_search_service::vertex_type : public ai::a_star::vertex_helper< float >::vertex_impl , public animation_space_graph_wrapper::vertex_impl , public ai::path_constructor::edge::helper< u32 >::vertex_impl< animations_search_service::vertex_type > , public ai::priority_queue::binary_heap::vertex_impl , public ai::vertex_manager::fixed_count_hash::vertex_helper< animation_space_vertex_id >::vertex_impl , public ai::vertex_allocator::fixed_count::vertex_impl {
	inline		vertex_type	( ) { /* no source */ }
}; // struct animations_search_service::vertex_type

STATIC_SIZE_ASSERT(animations_search_service::vertex_type, 0x30);

} // namespace survarium

#endif // #ifndef ANIMATIONS_SEARCH_SERVICE_VERTEX_TYPE_H_INCLUDED
