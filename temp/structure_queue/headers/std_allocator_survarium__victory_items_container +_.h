////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class survarium::victory_items_container;
class survarium::victory_items_container*;

namespace survarium {

class std_allocator< victory_items_container* > {
public:
	inline										std_allocator<survarium::victory_items_container *>( ) { /* no source */ }

	inline	victory_items_container**			address		( victory_items_container*& arg_0 ) const { /* no source */ }
	inline	victory_items_container* const*		address		( victory_items_container* const& arg_0 ) const { /* no source */ }

	inline	victory_items_container**			allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	victory_items_container**			allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	victory_items_container**			_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	victory_items_container**			_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*								__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void								deallocate	( victory_items_container** arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void								deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void								construct	( victory_items_container** arg_0, victory_items_container* const& arg_1 ) { /* no source */ }

	inline	void								destroy		( victory_items_container** arg_0 ) { /* no source */ }

	inline	u32									max_size	( ) const { /* no source */ }
}; // class std_allocator< victory_items_container* >

STATIC_SIZE_ASSERT(std_allocator< victory_items_container* >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED
