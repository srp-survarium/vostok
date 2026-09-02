// SPDX-License-Identifier: GPL-3.0-or-later

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

// claude@NOTE: STRUCTURE MATCH (1 stmt, 0 locals). Residual is an inline-vs-call fold on a
// resource_ptr passthrough: target emits an extra void-returning out-of-line call on
// &particle (lea eax,[ebp+8]; call <folded-empty>; push eax) before push_back - an ICF-folded
// helper (identity/eater) emitted out-of-line in the target but inlined to nothing in our
// build (which just does lea ecx; push). NOT a plain ASSERT (those materialize a false bool
// temp + lea; this takes &particle directly and preserves eax). Not steerable from this caller.
void material_pair::add_particle( resources::unmanaged_resource_ptr particle )
{
	m_particles.push_back( particle );
}

// claude@NOTE: STRUCTURE MATCH (4 stmts, 0 locals). The leading ASSERT(!empty) matches
// byte-for-byte. Residual is the trailing "useless call": target wraps the returned element
// in an out-of-line void call (lea eax,[edx+ecx*4] = &element; call <folded-empty>; ret, eax
// preserved). It is NOT a plain/_U ASSERT (no false-bool temp, no guard test/je/pushes) - it
// takes &element directly, i.e. an ICF-folded identity/passthrough helper that the target
// emits out-of-line and our build inlines away. Same inline-vs-call fold as add_particle.
resources::unmanaged_resource_ptr const& material_pair::particle( ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( !m_particles.empty( ) ) );
	if ( m_current_particle_idx == m_particles.size( ) )
		m_current_particle_idx = 0;

	return m_particles[m_current_particle_idx++];
}

} // namespace survarium
