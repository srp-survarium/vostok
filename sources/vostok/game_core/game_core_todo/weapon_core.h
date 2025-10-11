////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_H_INCLUDED
#define WEAPON_CORE_H_INCLUDED

namespace survarium {

// STATE[STUB]
// bool survarium::weapon_core::ready_to_reload() const
bool weapon_core::ready_to_reload( ) const
{
	return false;
}

// STATE[STUB]
// void survarium::weapon_core::on_ammo_empty()
void weapon_core::on_ammo_empty( )
{
}

// STATE[STUB]
// bool survarium::weapon_core::target_predicate(const survarium::weapon_targets) const
bool weapon_core::target_predicate( weapon_targets target ) const
{
	return false;
}

// STATE[STUB]
// vostok::animation::callback_return_type_enum survarium::weapon_core::fake_callback(vostok::animation::animation_callback_params&)
vostok::animation::callback_return_type_enum weapon_core::fake_callback( vostok::animation::animation_callback_params& __formal )
{
}

	/* TYPEDEFS

	typedef
		long
		counter_type;

	typedef
		vostok::memory::multi_threading_single_size_allocator_policy<vostok::memory::single_size_buffer_allocator<128,vostok::threading::simple_lock>::node>::free_list_type
		free_list_type;

} // namespace survarium

#endif // #ifndef WEAPON_CORE_H_INCLUDED
