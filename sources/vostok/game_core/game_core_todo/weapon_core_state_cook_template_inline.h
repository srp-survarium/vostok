////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_CORE_STATE_COOK_TEMPLATE_INLINE_H_INCLUDED
#define WEAPON_CORE_STATE_COOK_TEMPLATE_INLINE_H_INCLUDED

namespace survarium {

// STATE[STUB]
// survarium::weapon_core_state_cook_template<survarium::weapon_core_idle_state>::weapon_core_state_cook_template<survarium::weapon_core_idle_state>()
weapon_core_state_cook_template<weapon_core_idle_state>::weapon_core_state_cook_template<weapon_core_idle_state>( )
{
	// FUNCTION BODY
	// <0x9acb8>|0x000|0x000:'16'
	// ******
}

// STATE[STUB]
// vostok::mutable_buffer survarium::weapon_core_state_cook_template<survarium::weapon_core_idle_state>::allocate_resource(vostok::resources::query_result_for_cook&, vostok::const_buffer, bool)
vostok::mutable_buffer weapon_core_state_cook_template<weapon_core_idle_state>::allocate_resource( vostok::resources::query_result_for_cook& in_query, vostok::const_buffer raw_file_data, bool file_exist )
{
	// FUNCTION BODY
	// 1
	// <0x9af17>|0x000|0x000:'29'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_state_cook_template<survarium::weapon_core_idle_state>::create_resource(vostok::resources::query_result_for_cook&, vostok::const_buffer, vostok::mutable_buffer)
void weapon_core_state_cook_template<weapon_core_idle_state>::create_resource( vostok::resources::query_result_for_cook& parent, vostok::const_buffer raw_file_data, vostok::mutable_buffer in_out_unmanaged_resource_buffer )
{
	// LOCALS
	// vostok::configs::binary_config_value cfg
	// weapon_state_creation_params const* params
	// vostok::fixed_vector<vostok::resources::request,4> requests
	// ******

	// TYPEDEFS
	// typedef
	// 	vostok::fixed_vector<vostok::resources::request,4>
	// 	requests_fixed_type;

	// ******

	// FUNCTION BODY
	// <0x9ad05>|0x000|0x000:'41'
	// <0x9ad11>|0x00c|0x00c:'42'
	// <0x9ad2b>|0x026|0x01a:'43'
	// <0x9ad53>|0x04e|0x028:'44'
	// <0x9ad54>|0x04f|0x001:'45'
	// 1
	// 2
	// 3
	// 4
	// <0x9ad6b>|0x066|0x017:'50'
	// 1
	// 2
	// 3
	// 4
	// <0x9ad79>|0x074|0x00e:'55'
	// 1
	// 2
	// <0x9adad>|0x0a8|0x034:'58'
	// 1
	// 2
	// 3
	// 4
	// 5
	// 6
	// 7
	// 8
	// 9
	// 10
	// 11
	// 12
	// 13
	// 14
	// 15
	// 16
	// <0x9adbb>|0x0b6|0x00e:'75'
	// <0x9ad5d>|0x058|-0x05e:'76'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_state_cook_template<survarium::weapon_core_idle_state>::on_subresources_ready(vostok::resources::queries_result&, vostok::mutable_buffer, survarium::weapon_state_creation_params const*)
void weapon_core_state_cook_template<weapon_core_idle_state>::on_subresources_ready( vostok::resources::queries_result& data, vostok::mutable_buffer buffer, weapon_state_creation_params const* params )
{
	// LOCALS
	// vostok::fixed_vector<vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>,4> animations
	// ******

	// TYPEDEFS
	// typedef
	// 	vostok::fixed_vector<vostok::resources::resource_ptr<vostok::resources::managed_resource,vostok::resources::managed_intrusive_base>,4>
	// 	skeleton_animations_fixed_type;

	// ******

	// FUNCTION BODY
	// 1
	// 2
	// 3
	// 4
	// <0xa065a>|0x000|0x000:'86'
	// 1
	// 2
	// <0xa0737>|0x0dd|0x0dd:'89'
	// 1
	// <0xa075b>|0x101|0x024:'91'
	// <0xa07ca>|0x170|0x06f:'92'
	// ******
}

// STATE[STUB]
// void survarium::weapon_core_state_cook_template<survarium::weapon_core_idle_state>::destroy_resource(vostok::resources::unmanaged_resource*)
void weapon_core_state_cook_template<weapon_core_idle_state>::destroy_resource( vostok::resources::unmanaged_resource* resource )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x8c47b }, type_index: TypeIndex(0x4b8ab) })
	// ******

	// FUNCTION BODY
	// <0x9d460>|0x000|0x000:'98'
	// ******
}

	/* TYPEDEFS

	typedef
		base_project::resolve_link_object*
		iterator_type;

	typedef
		scheduler::record*
		iterator_type;

	typedef
		vostok::collision::bone_collision_data const*
		iterator_type;

	typedef
		vostok::collision::bone_collision_data*
		iterator_type;

} // namespace survarium

#endif // #ifndef WEAPON_CORE_STATE_COOK_TEMPLATE_INLINE_H_INCLUDED
