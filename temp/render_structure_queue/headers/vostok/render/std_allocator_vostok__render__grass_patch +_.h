////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::grass_patch;
class vostok::render::grass_patch*;

namespace vostok {
namespace render {

class std_allocator< grass_patch* > {
public:
	inline							std_allocator<vostok::render::grass_patch *>( ) { /* no source */ }

	inline	grass_patch**			address		( grass_patch*& arg_0 ) const { /* no source */ }
	inline	grass_patch* const*		address		( grass_patch* const& arg_0 ) const { /* no source */ }

	inline	grass_patch**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	grass_patch**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	grass_patch**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	grass_patch**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*					__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void					deallocate	( grass_patch** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void					deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void					construct	( grass_patch** arg_0, grass_patch* const& arg_1 ) { /* no source */ }

	inline	void					destroy		( grass_patch** arg_0 ) { /* no source */ }

	inline	u32						max_size	( ) const { /* no source */ }
}; // class std_allocator< grass_patch* >

STATIC_SIZE_ASSERT(std_allocator< grass_patch* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
