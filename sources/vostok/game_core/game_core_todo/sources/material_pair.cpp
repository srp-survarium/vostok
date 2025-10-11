////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "material_pair.h"

namespace survarium {

// STATE[STUB]
// survarium::material_pair::material_pair()
material_pair::material_pair( )
{
	// FUNCTION BODY
	// 1
	// ******
}

// STATE[STUB]
// void survarium::material_pair::~material_pair()
void material_pair::~material_pair( )
{
}

// STATE[STUB]
// void survarium::material_pair::load_from_config(survarium::game_material_manager const&, vostok::configs::binary_config_value const&)
void material_pair::load_from_config( game_material_manager const& manager, vostok::configs::binary_config_value const& val )
{
	// LOCALS
	// u16 							first_mtrl_id
	// u16 							second_mtrl_id
	// ******

	// FUNCTION BODY
	// <0x6fd499>|0x000|0x000:'25'
	// <0x6fd4af>|0x016|0x016:'26'
	// <0x6fd4c5>|0x02c|0x016:'27'
	// <0x6fd4df>|0x046|0x01a:'28'
	// <0x6fd4f9>|0x060|0x01a:'29'
	// <0x6fd50c>|0x073|0x013:'30'
	// ******
}

// STATE[STUB]
// void survarium::material_pair::add_particle(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>)
void material_pair::add_particle( vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> particle )
{
	// FUNCTION BODY
	// <0x6fd539>|0x000|0x000:'35'
	// ******
}

// STATE[STUB]
// vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const& survarium::material_pair::particle() const
vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const& material_pair::particle( ) const
{
	// FUNCTION BODY
	// <0x6fd429>|0x000|0x000:'40'
	// <0x6fd435>|0x00c|0x00c:'41'
	// <0x6fd44b>|0x022|0x016:'42'
	// 1
	// <0x6fd455>|0x02c|0x00a:'44'
	// ******
}

	/* TYPEDEFS

	typedef
		vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>*
		iterator_type;

} // namespace survarium
