////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class survarium::relocate_item_descr;

namespace survarium {

class std_allocator< relocate_item_descr > {
public:
	inline									std_allocator<survarium::relocate_item_descr>( ) { /* no source */ }

	inline	relocate_item_descr*			address		( relocate_item_descr& arg_0 ) const { /* no source */ }
	inline	relocate_item_descr const*		address		( relocate_item_descr const& arg_0 ) const { /* no source */ }

	inline	relocate_item_descr*			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	relocate_item_descr*			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	relocate_item_descr*			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	relocate_item_descr*			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*							__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void							deallocate	( relocate_item_descr* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void							deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void							construct	( relocate_item_descr* arg_0, relocate_item_descr const& arg_1 ) { /* no source */ }

	inline	void							destroy		( relocate_item_descr* arg_0 ) { /* no source */ }

	inline	u32								max_size	( ) const { /* no source */ }
}; // class std_allocator< relocate_item_descr >

STATIC_SIZE_ASSERT(std_allocator< relocate_item_descr >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED
