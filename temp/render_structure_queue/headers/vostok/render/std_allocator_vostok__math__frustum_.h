////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::math::frustum;

namespace vostok {
namespace render {

class std_allocator< math::frustum > {
public:
	inline							std_allocator<vostok::math::frustum>( ) { /* no source */ }

	inline	math::frustum*			address		( math::frustum& arg_0 ) const { /* no source */ }
	inline	math::frustum const*	address		( math::frustum const& arg_0 ) const { /* no source */ }

	inline	math::frustum*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	math::frustum*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	math::frustum*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	math::frustum*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*					__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void					deallocate	( math::frustum* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void					deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void					construct	( math::frustum* arg_0, math::frustum const& arg_1 ) { /* no source */ }

	inline	void					destroy		( math::frustum* arg_0 ) { /* no source */ }

	inline	u32						max_size	( ) const { /* no source */ }
}; // class std_allocator< math::frustum >

STATIC_SIZE_ASSERT(std_allocator< math::frustum >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
