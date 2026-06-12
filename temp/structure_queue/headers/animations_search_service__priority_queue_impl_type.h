////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATIONS_SEARCH_SERVICE_PRIORITY_QUEUE_IMPL_TYPE_H_INCLUDED
#define ANIMATIONS_SEARCH_SERVICE_PRIORITY_QUEUE_IMPL_TYPE_H_INCLUDED

/* INCLUDES */
class vostok::ai::priority_queue::binary_heap::impl<survarium::animations_search_service::vertex_manager_impl_type>;

/* FORWARD REFS */
class survarium::animations_search_service::vertex_manager_impl_type;

namespace survarium {

struct animations_search_service::priority_queue_impl_type : public ai::priority_queue::binary_heap::impl< animations_search_service::vertex_manager_impl_type > {
	inline		priority_queue_impl_type	( animations_search_service::vertex_manager_impl_type& arg_0, const u32 arg_1 ) { /* no source */ }
	inline		~priority_queue_impl_type	( ) { /* no source */ }
}; // struct animations_search_service::priority_queue_impl_type

STATIC_SIZE_ASSERT(animations_search_service::priority_queue_impl_type, 0x10);

} // namespace survarium

#endif // #ifndef ANIMATIONS_SEARCH_SERVICE_PRIORITY_QUEUE_IMPL_TYPE_H_INCLUDED
