////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATIONS_SEARCH_SERVICE_LOOK_UP_CELL_TYPE_H_INCLUDED
#define ANIMATIONS_SEARCH_SERVICE_LOOK_UP_CELL_TYPE_H_INCLUDED

/* INCLUDES */
struct vostok::ai::a_star::look_up_cell_impl;
struct vostok::ai::path_constructor::base::look_up_cell_impl;
struct vostok::ai::priority_queue::binary_heap::look_up_cell_impl;
struct vostok::ai::vertex_allocator::fixed_count::look_up_cell_impl;
struct vostok::ai::vertex_manager::fixed_count_hash::look_up_cell_helper<survarium::animations_search_service::vertex_type>::look_up_cell_impl<survarium::animations_search_service::look_up_cell_type>;
struct survarium::animation_space_graph_wrapper::look_up_cell_impl;

namespace survarium {

struct animations_search_service::look_up_cell_type : public ai::a_star::look_up_cell_impl , public animation_space_graph_wrapper::look_up_cell_impl , public ai::path_constructor::base::look_up_cell_impl , public ai::priority_queue::binary_heap::look_up_cell_impl , public ai::vertex_manager::fixed_count_hash::look_up_cell_helper< animations_search_service::vertex_type >::look_up_cell_impl< animations_search_service::look_up_cell_type > , public ai::vertex_allocator::fixed_count::look_up_cell_impl {
}; // struct animations_search_service::look_up_cell_type

STATIC_SIZE_ASSERT(animations_search_service::look_up_cell_type, 0x18);

} // namespace survarium

#endif // #ifndef ANIMATIONS_SEARCH_SERVICE_LOOK_UP_CELL_TYPE_H_INCLUDED
