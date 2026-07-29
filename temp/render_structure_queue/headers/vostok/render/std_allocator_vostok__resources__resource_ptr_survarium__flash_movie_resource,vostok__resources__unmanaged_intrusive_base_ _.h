////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* INCLUDES */
class survarium::flash_movie_resource;

namespace vostok {
namespace render {

class std_allocator< survarium::flash_movie_resource_ptr > {
public:
	inline				std_allocator<vostok::resources::resource_ptr<survarium::flash_movie_resource,vostok::resources::unmanaged_intrusive_base> >( ) { /* no source */ }

	inline	survarium::flash_movie_resource_ptr*	address		( survarium::flash_movie_resource_ptr& arg_0 ) const { /* no source */ }
	inline	survarium::flash_movie_resource_ptr const*	address		( survarium::flash_movie_resource_ptr const& arg_0 ) const { /* no source */ }

	inline	survarium::flash_movie_resource_ptr*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	survarium::flash_movie_resource_ptr*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	survarium::flash_movie_resource_ptr*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	survarium::flash_movie_resource_ptr*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	( survarium::flash_movie_resource_ptr* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							survarium::flash_movie_resource_ptr*	arg_0,
							survarium::flash_movie_resource_ptr const&	arg_1
						) { /* no source */ }

	inline	void		destroy		( survarium::flash_movie_resource_ptr* arg_0 ) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< survarium::flash_movie_resource_ptr >

STATIC_SIZE_ASSERT(std_allocator< survarium::flash_movie_resource_ptr >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
