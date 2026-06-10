////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/material_pair.h>

#include <vostok/game_core/game_material_manager.h>

namespace survarium {

// STATE[95.07%|DONE]: vectora ctor inline-vs-call (LTCG), shape matches
// STRUCTURE DIFF:
// target: 0x6ed5a0            base: 0x516550
// ; survarium::material_pair::material_pair() ; target 0 stmts / base 0 stmts
// ; aligned 0, size-diffs 0, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - 0-stmt member-init; residual is m_particles vectora ctor inline-vs-call, non-steerable. trail: material_pair.md
material_pair::material_pair( ) :
	m_particles				( g_allocator ),
	m_current_particle_idx	( 0 )
{
}

// STATE[100%|DONE]
material_pair::~material_pair( )
{
}

// STATE[100%|DONE]
void material_pair::load_from_config( game_material_manager const& manager, configs::binary_config_value const& val )
{
	u16 first_mtrl_id		= (u16)val["mtrl_1_id"];
	u16 second_mtrl_id		= (u16)val["mtrl_2_id"];
	m_decal1_size			= (float)val["decal1_size"];
	m_decal2_size			= (float)val["decal2_size"];
	m_first_material		= manager.get_material( first_mtrl_id );
	m_second_material		= manager.get_material( second_mtrl_id );
}

// STATE[92.86%|DONE]: resource_ptr by-value copy ctor inline-vs-call (LTCG), shape matches
// STRUCTURE DIFF:
// target: 0x6ed530            base: 0x5165e0
// 0x009 <0x11> | 0x009 <0xc> | m_particles.push_back( particle );   SIZE
// ; aligned 0, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the resource_ptr arg copy-ctor out-of-line in target vs inlined in base, non-steerable. trail: material_pair.md
void material_pair::add_particle( resources::unmanaged_resource_ptr particle )
{
	m_particles.push_back( particle );
}

// STATE[97.14%|PARTIAL]: vectora operator[] inline-vs-call (LTCG), shape matches
// STRUCTURE DIFF:
// target: 0x6ed420            base: 0x5164a0
// 0x035 <0x2e> | 0x035 <0x29> | return m_particles[m_current_particle_idx++];   SIZE
// ; aligned 4, size-diffs 1, quantity-diffs 0
// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is m_particles operator[] out-of-line call in target vs inlined lea in base (sushi's "useless call"), non-steerable. trail: material_pair.md
resources::unmanaged_resource_ptr const& material_pair::particle( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( !m_particles.empty( ) ) );
	if ( m_current_particle_idx == m_particles.size( ) )
		m_current_particle_idx = 0;

	return m_particles[m_current_particle_idx++]; // sushi@MATCH: Has some useless call at the end. Maybe 'at' artifact?
}

} // namespace survarium
