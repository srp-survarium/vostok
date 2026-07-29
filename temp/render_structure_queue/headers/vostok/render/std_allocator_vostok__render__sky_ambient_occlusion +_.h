////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::sky_ambient_occlusion;
class vostok::render::sky_ambient_occlusion*;

namespace vostok {
namespace render {

class std_allocator< sky_ambient_occlusion* > {
public:
	inline										std_allocator<vostok::render::sky_ambient_occlusion *>( ) { /* no source */ }

	inline	sky_ambient_occlusion**				address		( sky_ambient_occlusion*& arg_0 ) const { /* no source */ }
	inline	sky_ambient_occlusion* const*		address		( sky_ambient_occlusion* const& arg_0 ) const { /* no source */ }

	inline	sky_ambient_occlusion**				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	sky_ambient_occlusion**				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	sky_ambient_occlusion**				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	sky_ambient_occlusion**				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*								__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void								deallocate	( sky_ambient_occlusion** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void								deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void								construct	( sky_ambient_occlusion** arg_0, sky_ambient_occlusion* const& arg_1 ) { /* no source */ }

	inline	void								destroy		( sky_ambient_occlusion** arg_0 ) { /* no source */ }

	inline	u32									max_size	( ) const { /* no source */ }
}; // class std_allocator< sky_ambient_occlusion* >

STATIC_SIZE_ASSERT(std_allocator< sky_ambient_occlusion* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
