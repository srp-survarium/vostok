////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_INACTIVE_STATE_H_INCLUDED
#define WEAPON_CORE_INACTIVE_STATE_H_INCLUDED

namespace survarium {

// STATE[STUB]
// bool survarium::weapon_core_inactive_state::is_ready_for_transition() const
bool weapon_core_inactive_state::is_ready_for_transition( ) const
{
	return false;
	// FUNCTION BODY
	// <0xe23e7>|0x000|0x000:'34'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_inactive_state::on_animation_end(vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const&, const unsigned int)
void weapon_core_inactive_state::on_animation_end( vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base> const& animation, u32 callback_time_in_ms )
{
	// FUNCTION BODY
	// <0xc40e9>|0x000|0x000:'44'
	// 1
	// ******
}

// STATE[STUB]
// void* survarium::weapon_core_inactive_state::`scalar deleting destructor'(unsigned int)
void* weapon_core_inactive_state::`scalar deleting destructor'( )
{
	return NULL;
	// FUNCTION BODY
	// <0xc4119>|0x000|0x000:'56'
	// 1
	// ******
}

} // namespace survarium

#endif // #ifndef WEAPON_CORE_INACTIVE_STATE_H_INCLUDED
