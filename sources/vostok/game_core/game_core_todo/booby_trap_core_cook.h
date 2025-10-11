////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BOOBY_TRAP_CORE_COOK_H_INCLUDED
#define BOOBY_TRAP_CORE_COOK_H_INCLUDED

namespace survarium {

// STATE[STUB]
// survarium::booby_trap_core* survarium::booby_trap_core_cook::new_derived_resource()
booby_trap_core* booby_trap_core_cook::new_derived_resource( )
{
	return NULL;
}

// STATE[STUB]
// unsigned int survarium::booby_trap_core_cook::get_derived_resource_size()
u32 booby_trap_core_cook::get_derived_resource_size( )
{
	return 0;
}

// STATE[STUB]
// void survarium::booby_trap_core_cook::query_for_derived_resources(vostok::resources::query_result_for_cook*, survarium::booby_trap_core*, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>)
void booby_trap_core_cook::query_for_derived_resources( vostok::resources::query_result_for_cook* parent, booby_trap_core* resource, vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base> config )
{
}

	/* TYPEDEFS

	typedef
		scheduler::record*
		iterator_type;

} // namespace survarium

#endif // #ifndef BOOBY_TRAP_CORE_COOK_H_INCLUDED
