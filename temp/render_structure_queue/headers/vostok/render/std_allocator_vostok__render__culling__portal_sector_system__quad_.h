////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::culling::portal_sector_system::quad;

namespace vostok {
namespace render {

class std_allocator< culling::portal_sector_system::quad > {
public:
	inline				std_allocator<vostok::render::culling::portal_sector_system::quad>( ) { /* no source */ }

	inline	culling::portal_sector_system::quad*	address		( culling::portal_sector_system::quad& arg_0 ) const { /* no source */ }
	inline	culling::portal_sector_system::quad const*	address		( culling::portal_sector_system::quad const& arg_0 ) const { /* no source */ }

	inline	culling::portal_sector_system::quad*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	culling::portal_sector_system::quad*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	culling::portal_sector_system::quad*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	culling::portal_sector_system::quad*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	( culling::portal_sector_system::quad* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							culling::portal_sector_system::quad*	arg_0,
							culling::portal_sector_system::quad const&	arg_1
						) { /* no source */ }

	inline	void		destroy		( culling::portal_sector_system::quad* arg_0 ) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< culling::portal_sector_system::quad >

STATIC_SIZE_ASSERT(std_allocator< culling::portal_sector_system::quad >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
