////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::environment_probe;
class vostok::render::environment_probe*;

namespace vostok {
namespace render {

class std_allocator< environment_probe* > {
public:
	inline									std_allocator<vostok::render::environment_probe *>( ) { /* no source */ }

	inline	environment_probe**				address		( environment_probe*& arg_0 ) const { /* no source */ }
	inline	environment_probe* const*		address		( environment_probe* const& arg_0 ) const { /* no source */ }

	inline	environment_probe**				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	environment_probe**				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	environment_probe**				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	environment_probe**				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*							__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void							deallocate	( environment_probe** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void							deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void							construct	( environment_probe** arg_0, environment_probe* const& arg_1 ) { /* no source */ }

	inline	void							destroy		( environment_probe** arg_0 ) { /* no source */ }

	inline	u32								max_size	( ) const { /* no source */ }
}; // class std_allocator< environment_probe* >

STATIC_SIZE_ASSERT(std_allocator< environment_probe* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
