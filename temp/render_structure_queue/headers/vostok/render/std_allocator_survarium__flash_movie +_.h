////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class survarium::flash_movie;
class survarium::flash_movie*;

namespace vostok {
namespace render {

class std_allocator< survarium::flash_movie* > {
public:
	inline										std_allocator<survarium::flash_movie *>( ) { /* no source */ }

	inline	survarium::flash_movie**			address		( survarium::flash_movie*& arg_0 ) const { /* no source */ }
	inline	survarium::flash_movie* const*		address		( survarium::flash_movie* const& arg_0 ) const { /* no source */ }

	inline	survarium::flash_movie**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	survarium::flash_movie**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	survarium::flash_movie**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	survarium::flash_movie**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*								__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void								deallocate	( survarium::flash_movie** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void								deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void								construct	( survarium::flash_movie** arg_0, survarium::flash_movie* const& arg_1 ) { /* no source */ }

	inline	void								destroy		( survarium::flash_movie** arg_0 ) { /* no source */ }

	inline	u32									max_size	( ) const { /* no source */ }
}; // class std_allocator< survarium::flash_movie* >

STATIC_SIZE_ASSERT(std_allocator< survarium::flash_movie* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
