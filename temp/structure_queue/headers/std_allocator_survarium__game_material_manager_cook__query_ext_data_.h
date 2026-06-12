////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_H_INCLUDED
#define STD_ALLOCATOR_H_INCLUDED

/* FORWARD REFS */
class survarium::game_material_manager_cook::query_ext_data;

namespace survarium {

class std_allocator< game_material_manager_cook::query_ext_data > {
public:
	inline				std_allocator<survarium::game_material_manager_cook::query_ext_data>( ) { /* no source */ }

	inline	game_material_manager_cook::query_ext_data*	address		( game_material_manager_cook::query_ext_data& arg_0 ) const { /* no source */ }
	inline	game_material_manager_cook::query_ext_data const*	address		( game_material_manager_cook::query_ext_data const& arg_0 ) const { /* no source */ }

	inline	game_material_manager_cook::query_ext_data*	allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	game_material_manager_cook::query_ext_data*	allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	game_material_manager_cook::query_ext_data*	_M_allocate	( u32 arg_0, void* arg_1 ) const { /* no source */ }
	inline	game_material_manager_cook::query_ext_data*	_M_allocate	( u32 arg_0, u32& arg_1 ) const { /* no source */ }

	inline	char*		__charalloc	( u32 arg_0 ) { /* no source */ }

	inline	void		deallocate	( game_material_manager_cook::query_ext_data* arg_0, u32 arg_1 ) const { /* no source */ }
	inline	void		deallocate	( void* arg_0, u32 arg_1 ) const { /* no source */ }

	inline	void		construct	(
							game_material_manager_cook::query_ext_data*	arg_0,
							game_material_manager_cook::query_ext_data const&	arg_1
						) { /* no source */ }

	inline	void		destroy		( game_material_manager_cook::query_ext_data* arg_0 ) { /* no source */ }

	inline	u32			max_size	( ) const { /* no source */ }
}; // class std_allocator< game_material_manager_cook::query_ext_data >

STATIC_SIZE_ASSERT(std_allocator< game_material_manager_cook::query_ext_data >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_H_INCLUDED
