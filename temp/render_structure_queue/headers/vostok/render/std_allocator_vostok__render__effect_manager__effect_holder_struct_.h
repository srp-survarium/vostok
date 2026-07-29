////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::effect_manager::effect_holder_struct;

namespace vostok {
namespace render {

class std_allocator< effect_manager::effect_holder_struct > {
public:
	inline				std_allocator<vostok::render::effect_manager::effect_holder_struct>( ) { /* no source */ }

	inline	effect_manager::effect_holder_struct*	address		( effect_manager::effect_holder_struct& arg_0 ) const { /* no source */ }
	inline	effect_manager::effect_holder_struct const*	address		( effect_manager::effect_holder_struct const& arg_0 ) const { /* no source */ }

	inline	effect_manager::effect_holder_struct*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	effect_manager::effect_holder_struct*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	effect_manager::effect_holder_struct*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	effect_manager::effect_holder_struct*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	( effect_manager::effect_holder_struct* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							effect_manager::effect_holder_struct*	arg_0,
							effect_manager::effect_holder_struct const&	arg_1
						) { /* no source */ }

	inline	void		destroy		( effect_manager::effect_holder_struct* arg_0 ) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< effect_manager::effect_holder_struct >

STATIC_SIZE_ASSERT(std_allocator< effect_manager::effect_holder_struct >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
