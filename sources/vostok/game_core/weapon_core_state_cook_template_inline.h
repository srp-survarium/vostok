////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_STATE_COOK_TEMPLATE_INLINE_H_INCLUDED
#define WEAPON_CORE_STATE_COOK_TEMPLATE_INLINE_H_INCLUDED

namespace survarium {

// STATE[STUB]
template < typename T >
weapon_core_state_cook_template<T>::weapon_core_state_cook_template( )
{
	// FUNCTION BODY
	// <0x9acb8>|0x038|+0x026:'16'
	// ******
}

// STATE[STUB]
template < typename T >
mutable_buffer weapon_core_state_cook_template<T>::allocate_resource( resources::query_result_for_cook& in_query, const_buffer raw_file_data, bool file_exist )
{
	// FUNCTION BODY
	// <0>
	// <0x9af17>|0x007|+0x023:'29'
	// ******
}

// STATE[STUB]
template < typename T >
void weapon_core_state_cook_template<T>::create_resource( resources::query_result_for_cook& parent, const_buffer raw_file_data, mutable_buffer in_out_unmanaged_resource_buffer )
{
	// LOCALS
	// configs::binary_config_value cfg
	// weapon_state_creation_params const* params
	// fixed_vector<resources::request,4> requests
	// ******

	// TYPEDEFS
	// typedef
	// 	fixed_vector<resources::request,4>
	// 	requests_fixed_type;

	// ******

	// FUNCTION BODY
	// <0x9ad05>|0x015|+0x00c:'41'
	// <0x9ad11>|0x021|+0x01a:'42'
	// <0x9ad2b>|0x03b|+0x028:'43'
	// <0x9ad53>|0x063|+0x001:'44'
	// <0x9ad54>|0x064|+0x017:'45'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x9ad6b>|0x07b|+0x00e:'50'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x9ad79>|0x089|+0x034:'55'
	// <0>
	// <1>
	// <0x9adad>|0x0bd|+0x00e:'58'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <0x9adbb>|0x0cb|-0x05e:'75'
	// <0x9ad5d>|0x06d|+0x12d:'76'
	// <0x9ae8a>|0x19a|-0x128:'76'
	// <0x9ad62>|0x072|+0x132:'77'
	// ******
}

// STATE[STUB]
template < typename T >
void weapon_core_state_cook_template<T>::on_subresources_ready( resources::queries_result& data, mutable_buffer buffer, weapon_state_creation_params const* params )
{
	// LOCALS
	// fixed_vector<resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base>,4> animations
	// ******

	// TYPEDEFS
	// typedef
	// 	fixed_vector<resources::resource_ptr<resources::managed_resource,resources::managed_intrusive_base>,4>
	// 	skeleton_animations_fixed_type;

	// ******

	// FUNCTION BODY
	// <0>
	// <1>
	// <2>
	// <3>
	// <0xa065a>|0x00a|+0x0dd:'86'
	// <0>
	// <1>
	// <0xa0737>|0x0e7|+0x024:'89'
	// <0>
	// <0xa075b>|0x10b|+0x06f:'91'
	// <0xa07ca>|0x17a|+0x00e:'92'
	// ******
}

// STATE[STUB]
template < typename T >
void weapon_core_state_cook_template<T>::destroy_resource( resources::unmanaged_resource* resource )
{
	// CALL SITE INFO
	// <0x9d47b> -> void* <unknown>(u32)
	// ******

	// FUNCTION BODY
	// <0x9d460>|0x000|+0x00f:'98'
	// ******
}

} // namespace survarium

#endif // #ifndef WEAPON_CORE_STATE_COOK_TEMPLATE_INLINE_H_INCLUDED
