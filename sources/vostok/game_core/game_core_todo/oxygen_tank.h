////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef OXYGEN_TANK_H_INCLUDED
#define OXYGEN_TANK_H_INCLUDED

namespace survarium {

// STATE[STUB]
// void* survarium::oxygen_tank::`scalar deleting destructor'(unsigned int)
void* oxygen_tank::`scalar deleting destructor'( )
{
	return NULL;
}

// STATE[STUB]
// void survarium::oxygen_tank::deactivate()
void oxygen_tank::deactivate( )
{
}

// STATE[STUB]
// vostok::math::float4x4 survarium::oxygen_tank::transform() const
vostok::math::float4x4 oxygen_tank::transform( ) const
{
	return vostok::math::float4x4();
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::oxygen_tank::selected_animations(vostok::mutable_buffer&, const bool) const
vostok::animation::mixing::expression oxygen_tank::selected_animations( vostok::mutable_buffer& buffer, bool is_third_view ) const
{
}

// STATE[STUB]
// void survarium::oxygen_tank::update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
void oxygen_tank::update_bones_matrices(
	vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&	user_skeleton,
	vostok::math::float4x4*				user_matrices,
	u32									user_matrices_count,
	u32									current_time_in_ms,
	vostok::math::float4x4&				character_head_transform,
	vostok::math::float4x4&				character_transform,
	vostok::animation::animation_player const&	animation_player)
{
}

// STATE[STUB]
// void survarium::oxygen_tank::serialize(vostok::network_core::udp_match_packet&, unsigned int) const
void oxygen_tank::serialize( vostok::network_core::udp_match_packet& packet, u32 client_offset ) const
{
}

// STATE[STUB]
// void survarium::oxygen_tank::deserialize(vostok::network_core::packet_reader&)
void oxygen_tank::deserialize( vostok::network_core::packet_reader& reader )
{
}

	/* TYPEDEFS

	typedef
		scheduler::record*
		iterator_type;

} // namespace survarium

#endif // #ifndef OXYGEN_TANK_H_INCLUDED
