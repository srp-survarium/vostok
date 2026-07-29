////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::shadow_batched_geometry::build::__l2::surface_set;

namespace vostok {
namespace render {

class std_allocator< `shadow_batched_geometry::build'::`2'::surface_set > {
public:
	inline				std_allocator<`vostok::render::shadow_batched_geometry::build'::`2'::surface_set>( ) { /* no source */ }

	inline	shadow_batched_geometry::build::__l2::surface_set*	address		( shadow_batched_geometry::build::__l2::surface_set& arg_0 ) const { /* no source */ }
	inline	shadow_batched_geometry::build::__l2::surface_set const*	address		( shadow_batched_geometry::build::__l2::surface_set const& arg_0 ) const { /* no source */ }

	inline	shadow_batched_geometry::build::__l2::surface_set*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	shadow_batched_geometry::build::__l2::surface_set*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	shadow_batched_geometry::build::__l2::surface_set*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	shadow_batched_geometry::build::__l2::surface_set*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	( shadow_batched_geometry::build::__l2::surface_set* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							shadow_batched_geometry::build::__l2::surface_set*	arg_0,
							shadow_batched_geometry::build::__l2::surface_set const&	arg_1
						) { /* no source */ }

	inline	void		destroy		( shadow_batched_geometry::build::__l2::surface_set* arg_0 ) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< `shadow_batched_geometry::build'::`2'::surface_set >

STATIC_SIZE_ASSERT(std_allocator< `shadow_batched_geometry::build'::`2'::surface_set >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
