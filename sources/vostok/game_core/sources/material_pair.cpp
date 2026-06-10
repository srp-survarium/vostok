////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/material_pair.h>

#include <vostok/game_core/game_material_manager.h>

namespace survarium {

// STATE[95.07%|DONE]: vectora ctor inline-vs-call (LTCG), shape matches
material_pair::material_pair( ) :
	m_particles				( g_allocator ),
	m_current_particle_idx	( 0 )
{
	// STRUCTURE DIFF: target 0 stmts / base 0 stmts (0x8f vs 0x8d, member-init only)
	// VERDICT: STRUCTURE MATCH (shape ok) - residual is m_particles vectora ctor inline-vs-call, non-steerable.
}

// STATE[99.65%|DONE]: instruction-identical; frame pad (sub esp,4Ch vs 3Ch) + ICF fold-name relocs
material_pair::~material_pair( )
{
	// STRUCTURE DIFF: target 0 stmts / base 0 stmts (0x3e both, implicit member dtors)
	// VERDICT: STRUCTURE MATCH - identical stream; residual is the base frame 16B larger + intrusive_ptr::dec / eater relocs ICF-folded onto differently-named twins, non-steerable.
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
void material_pair::add_particle( resources::unmanaged_resource_ptr particle )
{
	m_particles.push_back( particle );

	// STRUCTURE DIFF: target 1 stmts / base 1 stmts
	// SIZE -0x5 | 48 | m_particles.push_back( particle );
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is the resource_ptr arg copy-ctor out-of-line in target vs inlined in base, non-steerable.
}

// STATE[97.14%|DONE]: vectora operator[] inline-vs-call (LTCG), shape matches
resources::unmanaged_resource_ptr const& material_pair::particle( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( !m_particles.empty( ) ) );
	if ( m_current_particle_idx == m_particles.size( ) )
		m_current_particle_idx = 0;

	return m_particles[m_current_particle_idx++]; // sushi@MATCH: Has some useless call at the end. Maybe 'at' artifact?

	// STRUCTURE DIFF: target 4 stmts / base 4 stmts
	// SIZE -0x5 | 63 | return m_particles[m_current_particle_idx++];
	// VERDICT: STRUCTURE MATCH (shape ok) - sole SIZE is m_particles operator[] out-of-line call in target vs inlined lea in base (sushi's "useless call"), non-steerable.
}

} // namespace survarium
