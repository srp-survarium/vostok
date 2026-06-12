////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATIONS_SEARCH_SERVICE_VERTEX_ALLOCATOR_IMPL_TYPE_H_INCLUDED
#define ANIMATIONS_SEARCH_SERVICE_VERTEX_ALLOCATOR_IMPL_TYPE_H_INCLUDED

/* INCLUDES */
class vostok::ai::vertex_allocator::fixed_count::impl<survarium::animations_search_service::vertex_type>;

/* FORWARD REFS */
class vostok::memory::base_allocator;

namespace survarium {

struct animations_search_service::vertex_allocator_impl_type : public ai::vertex_allocator::fixed_count::impl< animations_search_service::vertex_type > {
	inline		vertex_allocator_impl_type	( memory::base_allocator& arg_0, const u32 arg_1 ) { /* no source */ }
	inline		~vertex_allocator_impl_type	( ) { /* no source */ }
}; // struct animations_search_service::vertex_allocator_impl_type

STATIC_SIZE_ASSERT(animations_search_service::vertex_allocator_impl_type, 0x10);

} // namespace survarium

#endif // #ifndef ANIMATIONS_SEARCH_SERVICE_VERTEX_ALLOCATOR_IMPL_TYPE_H_INCLUDED
