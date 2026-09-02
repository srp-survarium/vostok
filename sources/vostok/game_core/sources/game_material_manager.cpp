// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include <vostok/game_core/game_material_manager.h>

#include <vostok/game_core/game_material.h>
#include <vostok/game_core/material_pair.h>

namespace survarium {

game_material_manager::game_material_manager( )
{
}

void game_material_manager::clear_resources( )
{
	delete_pairs( );
	delete_materials( );
}

game_material_manager::~game_material_manager( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );
}

void game_material_manager::delete_pairs( )
{
	map< u16, map< u16, material_pair const* > >::iterator it	= m_pairs.begin( );
	map< u16, map< u16, material_pair const* > >::iterator end	= m_pairs.end( );

	for ( ; it != end ; ++it )
	{
		map< u16, material_pair const* >::iterator internail_it		= it->second.begin( );
		map< u16, material_pair const* >::iterator internail_end	= it->second.end( );

		for ( ; internail_it != internail_end ; ++internail_it )
			VOSTOK_DELETE_IMPL( g_allocator, internail_it->second );
	}
	m_pairs.clear( );
}

void game_material_manager::delete_materials( )
{
	map< u16, game_material const* >::iterator it = m_materials.begin( );
	map< u16, game_material const* >::iterator end = m_materials.end( );
	for ( ; it != end ; ++it )
		VOSTOK_DELETE_IMPL( g_allocator, it->second );
	m_materials.clear( );
}

game_material const* game_material_manager::get_material( u16 id ) const
{
	map< u16, game_material const* >::const_iterator it = m_materials.find( id );
	return it != m_materials.end( ) ? it->second : get_material( m_default_material_id );
}

material_pair const* game_material_manager::get_pair( u16 first_mtrl_id, u16 second_mtrl_id ) const
{
	u16 first_material	= material_exist( first_mtrl_id ) ? first_mtrl_id : m_default_material_id;
	u16 second_material = material_exist( second_mtrl_id ) ? second_mtrl_id : m_default_material_id;

	map< u16, map< u16, material_pair const * > >::const_iterator first_it = m_pairs.find( first_material );
	if ( first_it == m_pairs.end( ) ) {
		first_material = m_default_material_id;
		first_it = m_pairs.find( first_material );
	}

	ASSERT( UNKNOWN_EXPRESSION_T( first_it != m_pairs.end( ) ) );

	map< u16, material_pair const * >::const_iterator second_it = first_it->second.find( second_material );
	if ( second_it == first_it->second.end( ) ) {
		second_material = m_default_material_id;
		second_it = first_it->second.find( second_material );
	}

	ASSERT( UNKNOWN_EXPRESSION_T( second_it != first_it->second.end( ) ) );
	return second_it->second;
}

bool game_material_manager::material_exist( u16 id ) const
{
	return m_materials.find( id ) != m_materials.end( );
}

void game_material_manager::add_game_material( game_material const* const mtrl )
{
	m_materials[mtrl->id( )] = mtrl;
}

void game_material_manager::add_pair( material_pair const* const pair )
{
	u16 first_mtrl_id	= pair->first_material( )->id( );
	u16 second_mtrl_id	= pair->second_material( )->id( );
	m_pairs[first_mtrl_id][second_mtrl_id] = pair;
}

} // namespace survarium
