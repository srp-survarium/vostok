////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_AMMUNITION_H_INCLUDED
#define WEAPON_AMMUNITION_H_INCLUDED

namespace survarium {

// STATE[STUB]
// void survarium::weapon_ammunition::activate(survarium::base_player&, survarium::engine&)
void weapon_ammunition::activate( base_player& user, engine& engine )
{
}

// STATE[STUB]
// void* survarium::weapon_ammunition::`scalar deleting destructor'(unsigned int)
void* weapon_ammunition::`scalar deleting destructor'( )
{
	return NULL;
}

// STATE[STUB]
// vostok::math::float4x4 survarium::weapon_ammunition::transform() const
vostok::math::float4x4 weapon_ammunition::transform( ) const
{
	return vostok::math::float4x4();
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::weapon_ammunition::selected_animations(vostok::mutable_buffer&, const bool) const
vostok::animation::mixing::expression weapon_ammunition::selected_animations( vostok::mutable_buffer& buffer, bool is_third_view ) const
{
}

// STATE[STUB]
// void survarium::weapon_ammunition::serialize(vostok::network_core::udp_match_packet&, unsigned int) const
void weapon_ammunition::serialize( vostok::network_core::udp_match_packet& packet, u32 client_offset ) const
{
}

// STATE[STUB]
// void survarium::weapon_ammunition::deserialize(vostok::network_core::packet_reader&)
void weapon_ammunition::deserialize( vostok::network_core::packet_reader& reader )
{
}

} // namespace survarium

#endif // #ifndef WEAPON_AMMUNITION_H_INCLUDED
