////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
#define RENDER_STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class vostok::render::material_effects_entry;

namespace vostok {
namespace render {

class std_allocator< material_effects_entry > {
public:
	inline										std_allocator<vostok::render::material_effects_entry>( ) { /* no source */ }

	inline	material_effects_entry*				address		( material_effects_entry& arg_0 ) const { /* no source */ }
	inline	material_effects_entry const*		address		( material_effects_entry const& arg_0 ) const { /* no source */ }

	inline	material_effects_entry*				allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	material_effects_entry*				allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	material_effects_entry*				_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	material_effects_entry*				_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*								__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void								deallocate	( material_effects_entry* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void								deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void								construct	( material_effects_entry* arg_0, material_effects_entry const& arg_1 ) { /* no source */ }

	inline	void								destroy		( material_effects_entry* arg_0 ) { /* no source */ }

	inline	u32									max_size	( ) const { /* no source */ }
}; // class std_allocator< material_effects_entry >

STATIC_SIZE_ASSERT(std_allocator< material_effects_entry >, 0x1);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_STD_ALLOCATOR_H_INCLUDED
