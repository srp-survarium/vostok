////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<survarium::game_material_manager_cook::query_ext_data,survarium::std_allocator<survarium::game_material_manager_cook::query_ext_data> >
	stlp_std::vector<survarium::game_material_manager_cook::query_ext_data >;
class survarium::game_material_manager_cook::query_ext_data;

namespace survarium {

class vector< game_material_manager_cook::query_ext_data > : public std::vector< game_material_manager_cook::query_ext_data > {
public:
	inline				vector<survarium::game_material_manager_cook::query_ext_data>( ) { /* no source */ }
	inline				vector<survarium::game_material_manager_cook::query_ext_data>( u32 arg_0, game_material_manager_cook::query_ext_data const& arg_1 ) { /* no source */ }
	inline	explicit	vector<survarium::game_material_manager_cook::query_ext_data>( u32 arg_0 ) { /* no source */ }

	inline	game_material_manager_cook::query_ext_data&	operator[]	( u32 arg_0 ) { /* no source */ }
	inline	game_material_manager_cook::query_ext_data const&	operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32			size		( ) const { /* no source */ }

	inline				~vector<survarium::game_material_manager_cook::query_ext_data>( ) { /* no source */ }
}; // class vector< game_material_manager_cook::query_ext_data >

STATIC_SIZE_ASSERT(vector< game_material_manager_cook::query_ext_data >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
