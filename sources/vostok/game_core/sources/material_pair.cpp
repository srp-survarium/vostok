////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/material_pair.h>

#include <vostok/game_core/game_material_manager.h>

namespace survarium {

material_pair::material_pair( ) :
	m_particles				( g_allocator ),
	m_current_particle_idx	( 0 )
{
}

material_pair::~material_pair( )
{
}

void material_pair::load_from_config( game_material_manager const& manager, configs::binary_config_value const& val )
{
	u16 first_mtrl_id		= (u16)val["mtrl_1_id"];
	u16 second_mtrl_id		= (u16)val["mtrl_2_id"];
	m_decal1_size			= (float)val["decal1_size"];
	m_decal2_size			= (float)val["decal2_size"];
	m_first_material		= manager.get_material( first_mtrl_id );
	m_second_material		= manager.get_material( second_mtrl_id );
}

void material_pair::add_particle( resources::unmanaged_resource_ptr particle )
{
	m_particles.push_back( particle );
}

resources::unmanaged_resource_ptr const& material_pair::particle( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( !m_particles.empty( ) ) );
	if ( m_current_particle_idx == m_particles.size( ) )
		m_current_particle_idx = 0;

	return m_particles[m_current_particle_idx++]; // sushi@MATCH: Has some useless call at the end. Maybe 'at' artifact?
}

} // namespace survarium
