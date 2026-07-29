////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::decal_instance;
class vostok::render::decal_instance*;

namespace vostok {
namespace render {

class std_allocator< decal_instance* > {
public:
	inline								std_allocator<vostok::render::decal_instance *>( ) { /* no source */ }

	inline	decal_instance**			address		( decal_instance*& arg_0 ) const { /* no source */ }
	inline	decal_instance* const*		address		( decal_instance* const& arg_0 ) const { /* no source */ }

	inline	decal_instance**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	decal_instance**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	decal_instance**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	decal_instance**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( decal_instance** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( decal_instance** arg_0, decal_instance* const& arg_1 ) { /* no source */ }

	inline	void						destroy		( decal_instance** arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< decal_instance* >

STATIC_SIZE_ASSERT(std_allocator< decal_instance* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
