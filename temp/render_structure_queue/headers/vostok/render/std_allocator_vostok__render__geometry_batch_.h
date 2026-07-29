////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::geometry_batch;

namespace vostok {
namespace render {

class std_allocator< geometry_batch > {
public:
	inline								std_allocator<vostok::render::geometry_batch>( ) { /* no source */ }

	inline	geometry_batch*				address		( geometry_batch& arg_0 ) const { /* no source */ }
	inline	geometry_batch const*		address		( geometry_batch const& arg_0 ) const { /* no source */ }

	inline	geometry_batch*				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	geometry_batch*				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	geometry_batch*				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	geometry_batch*				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( geometry_batch* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( geometry_batch* arg_0, geometry_batch const& arg_1 ) { /* no source */ }

	inline	void						destroy		( geometry_batch* arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< geometry_batch >

STATIC_SIZE_ASSERT(std_allocator< geometry_batch >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
