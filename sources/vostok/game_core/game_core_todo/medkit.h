////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef MEDKIT_H_INCLUDED
#define MEDKIT_H_INCLUDED

namespace survarium {

// STATE[STUB]
// void survarium::medkit::activate(survarium::base_player&, survarium::engine&)
void medkit::activate( base_player& user, engine& engine )
{
}

// STATE[STUB]
// void survarium::medkit::deactivate()
void medkit::deactivate( )
{
}

// STATE[STUB]
// vostok::math::float4x4 survarium::medkit::transform() const
vostok::math::float4x4 medkit::transform( ) const
{
	return vostok::math::float4x4();
}

// STATE[STUB]
// vostok::animation::mixing::expression survarium::medkit::selected_animations(vostok::mutable_buffer&, const bool) const
vostok::animation::mixing::expression medkit::selected_animations( vostok::mutable_buffer& buffer, bool is_third_view ) const
{
}

// STATE[STUB]
// void survarium::medkit::update_bones_matrices(vostok::resources::resource_ptr<vostok::animation::skeleton,vostok::resources::unmanaged_intrusive_base> const&, vostok::math::float4x4* const, const unsigned int, const unsigned int, vostok::math::float4x4&, vostok::math::float4x4&, vostok::animation::animation_player const&)
void medkit::update_bones_matrices(
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
// void survarium::medkit::serialize(vostok::network_core::udp_match_packet&, unsigned int) const
void medkit::serialize( vostok::network_core::udp_match_packet& packet, u32 client_offset ) const
{
}

// STATE[STUB]
// void survarium::medkit::deserialize(vostok::network_core::packet_reader&)
void medkit::deserialize( vostok::network_core::packet_reader& reader )
{
}

	/* TYPEDEFS

	typedef
		scheduler::record*
		iterator_type;

} // namespace survarium

#endif // #ifndef MEDKIT_H_INCLUDED
