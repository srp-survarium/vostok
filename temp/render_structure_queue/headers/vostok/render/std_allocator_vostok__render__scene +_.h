////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::scene;
class vostok::render::scene*;

namespace vostok {
namespace render {

class std_allocator< scene* > {
public:
	inline						std_allocator<vostok::render::scene *>( ) { /* no source */ }

	inline	scene**				address		( scene*& arg_0 ) const { /* no source */ }
	inline	scene* const*		address		( scene* const& arg_0 ) const { /* no source */ }

	inline	scene**				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	scene**				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	scene**				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	scene**				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*				__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void				deallocate	( scene** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void				deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void				construct	( scene** arg_0, scene* const& arg_1 ) { /* no source */ }

	inline	void				destroy		( scene** arg_0 ) { /* no source */ }

	inline	u32					max_size	( ) const { /* no source */ }
}; // class std_allocator< scene* >

STATIC_SIZE_ASSERT(std_allocator< scene* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
