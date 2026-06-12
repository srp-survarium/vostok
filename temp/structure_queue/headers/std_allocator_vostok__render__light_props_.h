////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::light_props;

namespace survarium {

class std_allocator< render::light_props > {
public:
	inline									std_allocator<vostok::render::light_props>( ) { /* no source */ }

	inline	render::light_props*			address		( render::light_props& arg_0 ) const { /* no source */ }
	inline	render::light_props const*		address		( render::light_props const& arg_0 ) const { /* no source */ }

	inline	render::light_props*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	render::light_props*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	render::light_props*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	render::light_props*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*							__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void							deallocate	( render::light_props* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void							deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void							construct	( render::light_props* arg_0, render::light_props const& arg_1 ) { /* no source */ }

	inline	void							destroy		( render::light_props* arg_0 ) { /* no source */ }

	inline	u32								max_size	( ) const { /* no source */ }
}; // class std_allocator< render::light_props >

STATIC_SIZE_ASSERT(std_allocator< render::light_props >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED
