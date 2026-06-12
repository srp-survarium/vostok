////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATIONS_SEARCH_SERVICE_VERTEX_MANAGER_IMPL_TYPE_H_INCLUDED
#define ANIMATIONS_SEARCH_SERVICE_VERTEX_MANAGER_IMPL_TYPE_H_INCLUDED

/* INCLUDES */
class vostok::ai::vertex_manager::fixed_count_hash::impl<survarium::animations_search_service::vertex_allocator_impl_type,survarium::animations_search_service::look_up_cell_type>;

/* FORWARD REFS */
class survarium::animations_search_service::vertex_allocator_impl_type;

namespace survarium {

struct animations_search_service::vertex_manager_impl_type : public ai::vertex_manager::fixed_count_hash::impl< animations_search_service::vertex_allocator_impl_type, animations_search_service::look_up_cell_type > {
	inline		vertex_manager_impl_type	(
					animations_search_service::vertex_allocator_impl_type&	arg_0,
					const u32		arg_1,
					const u32		arg_2
				) { /* no source */ }
	inline		~vertex_manager_impl_type	( ) { /* no source */ }
}; // struct animations_search_service::vertex_manager_impl_type

STATIC_SIZE_ASSERT(animations_search_service::vertex_manager_impl_type, 0x1C);

} // namespace survarium

#endif // #ifndef ANIMATIONS_SEARCH_SERVICE_VERTEX_MANAGER_IMPL_TYPE_H_INCLUDED
