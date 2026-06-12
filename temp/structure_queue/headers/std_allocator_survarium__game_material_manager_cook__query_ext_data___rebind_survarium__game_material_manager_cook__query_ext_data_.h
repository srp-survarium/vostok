////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
#define STD_ALLOCATOR_REBIND_H_INCLUDED

namespace survarium {

struct std_allocator< game_material_manager_cook::query_ext_data >::rebind< game_material_manager_cook::query_ext_data > {
}; // struct std_allocator< game_material_manager_cook::query_ext_data >::rebind< game_material_manager_cook::query_ext_data >

STATIC_SIZE_ASSERT(std_allocator< game_material_manager_cook::query_ext_data >::rebind< game_material_manager_cook::query_ext_data >, 0x1);

} // namespace survarium

#endif // #ifndef STD_ALLOCATOR_REBIND_H_INCLUDED
