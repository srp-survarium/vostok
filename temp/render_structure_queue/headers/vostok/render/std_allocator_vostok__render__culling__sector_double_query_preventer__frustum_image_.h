////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::culling::sector_double_query_preventer::frustum_image;

namespace vostok {
namespace render {

class std_allocator< culling::sector_double_query_preventer::frustum_image > {
public:
	inline				std_allocator<vostok::render::culling::sector_double_query_preventer::frustum_image>( ) { /* no source */ }

	inline	culling::sector_double_query_preventer::frustum_image*	address		( culling::sector_double_query_preventer::frustum_image& arg_0 ) const { /* no source */ }
	inline	culling::sector_double_query_preventer::frustum_image const*	address		( culling::sector_double_query_preventer::frustum_image const& arg_0 ) const { /* no source */ }

	inline	culling::sector_double_query_preventer::frustum_image*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	culling::sector_double_query_preventer::frustum_image*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	culling::sector_double_query_preventer::frustum_image*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	culling::sector_double_query_preventer::frustum_image*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	( culling::sector_double_query_preventer::frustum_image* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							culling::sector_double_query_preventer::frustum_image*	arg_0,
							culling::sector_double_query_preventer::frustum_image const&	arg_1
						) { /* no source */ }

	inline	void		destroy		( culling::sector_double_query_preventer::frustum_image* arg_0 ) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< culling::sector_double_query_preventer::frustum_image >

STATIC_SIZE_ASSERT(std_allocator< culling::sector_double_query_preventer::frustum_image >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
