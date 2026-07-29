////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* INCLUDES */
class vostok::render::res_effect;

namespace vostok {
namespace render {

class std_allocator< res_effect_ptr* > {
public:
	inline								std_allocator<vostok::resources::resource_ptr<vostok::render::res_effect,vostok::resources::unmanaged_intrusive_base> *>( ) { /* no source */ }

	inline	res_effect_ptr**			address		( res_effect_ptr*& arg_0 ) const { /* no source */ }
	inline	res_effect_ptr* const*		address		( res_effect_ptr* const& arg_0 ) const { /* no source */ }

	inline	res_effect_ptr**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	res_effect_ptr**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	res_effect_ptr**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	res_effect_ptr**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*						__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void						deallocate	( res_effect_ptr** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void						deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void						construct	( res_effect_ptr** arg_0, res_effect_ptr* const& arg_1 ) { /* no source */ }

	inline	void						destroy		( res_effect_ptr** arg_0 ) { /* no source */ }

	inline	u32							max_size	( ) const { /* no source */ }
}; // class std_allocator< res_effect_ptr* >

STATIC_SIZE_ASSERT(std_allocator< res_effect_ptr* >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
