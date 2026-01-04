////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/material_pair.h>

#include <vostok/game_core/game_material_manager.h>

namespace survarium {

// STATE[99.29%|DONE]
material_pair::material_pair( ) :
	m_particles				( g_allocator ),
	m_current_particle_idx	( 0 )
{
}

// STATE[100%|DONE]
material_pair::~material_pair( )
{
}

// STATE[99.51%|DONE]
void material_pair::load_from_config( game_material_manager const& manager, configs::binary_config_value const& val )
{
	u16 first_mtrl_id		= (u16)val["mtrl_1_id"];
	u16 second_mtrl_id		= (u16)val["mtrl_2_id"];
	m_decal1_size			= (float)val["decal1_size"];
	m_decal2_size			= (float)val["decal2_size"];
	m_first_material		= manager.get_material( first_mtrl_id );
	m_second_material		= manager.get_material( second_mtrl_id );


	// FUNCTION BODY
	// <0x6fd499>|0x009|+0x016:'25'
	// <0x6fd4af>|0x01f|+0x016:'26'
	// <0x6fd4c5>|0x035|+0x01a:'27'
	// <0x6fd4df>|0x04f|+0x01a:'28'
	// <0x6fd4f9>|0x069|+0x013:'29'
	// <0x6fd50c>|0x07c|+0x013:'30'
	// ******
}

// STATE[100%|DONE]
void material_pair::add_particle( resources::unmanaged_resource_ptr particle )
{
	m_particles.push_back( particle );

	// FUNCTION BODY
	// <0x6fd539>|0x009|+0x011:'35'
	// ******
}

// STATE[97.14%|PARTIAL]
resources::unmanaged_resource_ptr const& material_pair::particle( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( !m_particles.empty( ) ) );
	if ( m_current_particle_idx == m_particles.size( ) )
		m_current_particle_idx = 0;

	return m_particles[m_current_particle_idx++]; // sushi@MATCH: Has some useless call at the end. Maybe 'at' artifact?

	// FUNCTION BODY
	// <0x6fd429>|0x009|+0x00c:'40'
	// <0x6fd435>|0x015|+0x016:'41'
	// <0x6fd44b>|0x02b|+0x00a:'42'
	// <0>
	// <0x6fd455>|0x035|+0x02e:'44'
	// ******
}

} // namespace survarium
