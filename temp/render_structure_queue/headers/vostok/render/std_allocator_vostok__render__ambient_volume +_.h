////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::ambient_volume;
class vostok::render::ambient_volume*;

namespace vostok {
namespace render {

class std_allocator< ambient_volume* > {
public:
	inline								std_allocator<vostok::render::ambient_volume *>( ) { /* no source */ }

	inline	ambient_volume**			address		( ambient_volume*& arg_0 ) const { /* no source */ }
	inline	ambient_volume* const*		address		( ambient_volume* const& arg_0 ) const { /* no source */ }

	inline	ambient_volume**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	ambient_volume**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	ambient_volume**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	ambient_volume**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( ambient_volume** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( ambient_volume** arg_0, ambient_volume* const& arg_1 ) { /* no source */ }

	inline	void						destroy		( ambient_volume** arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< ambient_volume* >

STATIC_SIZE_ASSERT(std_allocator< ambient_volume* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
