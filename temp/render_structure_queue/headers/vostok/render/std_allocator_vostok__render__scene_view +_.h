////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::scene_view;
class vostok::render::scene_view*;

namespace vostok {
namespace render {

class std_allocator< scene_view* > {
public:
	inline							std_allocator<vostok::render::scene_view *>( ) { /* no source */ }

	inline	scene_view**			address		( scene_view*& arg_0 ) const { /* no source */ }
	inline	scene_view* const*		address		( scene_view* const& arg_0 ) const { /* no source */ }

	inline	scene_view**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	scene_view**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	scene_view**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	scene_view**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*					__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void					deallocate	( scene_view** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void					deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void					construct	( scene_view** arg_0, scene_view* const& arg_1 ) { /* no source */ }

	inline	void					destroy		( scene_view** arg_0 ) { /* no source */ }

	inline	u32						max_size	( ) const { /* no source */ }
}; // class std_allocator< scene_view* >

STATIC_SIZE_ASSERT(std_allocator< scene_view* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
