////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/material_pair.h>

namespace survarium {

// STATE[STUB]
// survarium::material_pair::material_pair()
material_pair::material_pair( ) : m_particles( g_allocator )
{
	// FUNCTION BODY
	// <0x6fd5a0>|0x000|+0x088:'16'	{
	// <0>
	// <0x6fd628>|0x088|      :'18'	}
	// ******
}

// STATE[STUB]
// void survarium::material_pair::~material_pair()
material_pair::~material_pair( )
{
	// FUNCTION BODY
	// <0x6fd560>|0x000|      :'21'	{
	// ******
}

// STATE[STUB]
// void survarium::material_pair::load_from_config(survarium::game_material_manager const&, vostok::configs::binary_config_value const&)
void material_pair::load_from_config( game_material_manager const& manager, configs::binary_config_value const& val )
{
	// LOCALS
	// u16 							first_mtrl_id
	// u16 							second_mtrl_id
	// ******

	// FUNCTION BODY
	// <0x6fd499>|0x009|+0x016:'25'
	// <0x6fd4af>|0x01f|+0x016:'26'
	// <0x6fd4c5>|0x035|+0x01a:'27'
	// <0x6fd4df>|0x04f|+0x01a:'28'
	// <0x6fd4f9>|0x069|+0x013:'29'
	// <0x6fd50c>|0x07c|+0x013:'30'
	// ******
}

// STATE[STUB]
// void survarium::material_pair::add_particle(vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>)
void material_pair::add_particle( resources::unmanaged_resource_ptr particle )
{
	// FUNCTION BODY
	// <0x6fd539>|0x009|+0x011:'35'
	// ******
}

// STATE[STUB]
// vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base> const& survarium::material_pair::particle() const
resources::unmanaged_resource_ptr const& material_pair::particle( ) const
{
	// FUNCTION BODY
	// <0x6fd429>|0x009|+0x00c:'40'
	// <0x6fd435>|0x015|+0x016:'41'
	// <0x6fd44b>|0x02b|+0x00a:'42'
	// <0>
	// <0x6fd455>|0x035|+0x02e:'44'
	// ******
}

} // namespace survarium
