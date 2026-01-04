////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/game_core/game_material_manager.h>

#include <vostok/game_core/game_material.h>
#include <vostok/game_core/material_pair.h>

namespace survarium {

// STATE[UNCHECKED]
game_material_manager::game_material_manager( )
{
}

// STATE[UNCHECKED]
void game_material_manager::clear_resources( )
{
	delete_pairs( );
	delete_materials( );

	// FUNCTION BODY
	// <0x73e8f7>|0x007|+0x008:'20'
	// <0x73e8ff>|0x00f|+0x008:'21'
	// ******
}

// STATE[UNCHECKED]
game_material_manager::~game_material_manager( )
{
	ASSERT( UNKNOWN_EXPRESSION );
	ASSERT( UNKNOWN_EXPRESSION );

	// FUNCTION BODY
	// <0x73e9a2>|0x012|+0x00c:'26'
	// <0x73e9ae>|0x01e|+0x00c:'27'
	// ******
}

// STATE[UNCHECKED]
void game_material_manager::delete_pairs( )
{
	map< u16, map< u16, material_pair const* > >::iterator it	= m_pairs.begin( );
	map< u16, map< u16, material_pair const* > >::iterator end	= m_pairs.end( );

	for ( ; it != end ; it++ )
	{
		map< u16, material_pair const* >::iterator internail_it		= it->second.begin( );
		map< u16, material_pair const* >::iterator internail_end	= it->second.end( );

		for ( ; internail_it != internail_end ; internail_it++ )
			VOSTOK_DELETE_IMPL( g_allocator, internail_it->second );
	}
	m_pairs.clear( );

	// FUNCTION BODY
	// <0x73e7d9>|0x009|+0x017:'32'
	// <0x73e7f0>|0x020|+0x011:'33'
	// <0>
	// <0x73e801>|0x031|+0x03c:'35'
	// <0>
	// <0x73e83d>|0x06d|+0x01d|[1]:'37'
	// <0x73e85a>|0x08a|+0x014:'38'
	// <0>
	// <0x73e86e>|0x09e|+0x038:'40'
	// <0x73e8a6>|0x0d6|+0x030:'41'
	// <0x73e8d6>|0x106|+0x005:'42'
	// <0x73e8db>|0x10b|+0x00e:'43'
	// ******
}

// STATE[UNCHECKED]
void game_material_manager::delete_materials( )
{
	map< u16, game_material const* >::iterator it = m_materials.begin( );
	map< u16, game_material const* >::iterator end = m_materials.end( );
	for ( ; it != end ; it++ )
		VOSTOK_DELETE_IMPL( g_allocator, it->second );
	m_materials.clear( );

	// FUNCTION BODY
	// <0x73e6e9>|0x009|+0x017:'48'
	// <0x73e700>|0x020|+0x011:'49'
	// <0x73e711>|0x031|+0x038:'50'
	// <0x73e749>|0x069|+0x030:'51'
	// <0x73e779>|0x099|+0x00e:'52'
	// ******
}

// STATE[100%|DONE]
game_material const* game_material_manager::get_material( u16 id ) const
{
	map< u16, game_material const* >::const_iterator it = m_materials.find( id );
	return it != m_materials.end( ) ? it->second : get_material( m_default_material_id );

	// std::priv::_Rb_tree_iterator<std::pair<u16 const ,game_material const *>,std::priv::_MapTraitsT<std::pair<u16 const ,game_material const *> > > it

	// FUNCTION BODY
	// <0x73e669>|0x009|+0x020:'58'
	// <0x73e689>|0x029|+0x047:'59'
	// ******
}

// STATE[UNCHECKED]
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

	// FUNCTION BODY
	// <0x73e4c9>|0x009|+0x034:'64'
	// <0x73e4fd>|0x03d|+0x034:'65'
	// <0>
	// <0x73e531>|0x071|+0x020:'67'
	// <0x73e551>|0x091|+0x023:'68'
	// <0x73e574>|0x0b4|+0x00e:'69'
	// <0x73e582>|0x0c2|+0x026:'70'
	// <0>
	// <0x73e5a8>|0x0e8|+0x00c:'72'
	// <0>
	// <0x73e5b4>|0x0f4|+0x023:'74'
	// <0x73e5d7>|0x117|+0x029:'75'
	// <0x73e600>|0x140|+0x00e:'76'
	// <0x73e60e>|0x14e|+0x02c:'77'
	// <0>
	// <0x73e63a>|0x17a|+0x00c:'79'
	// <0x73e646>|0x186|+0x006:'80'
	// ******
}

// STATE[UNCHECKED]
bool game_material_manager::material_exist( u16 id ) const
{
	return m_materials.find( id ) != m_materials.end( );

	// FUNCTION BODY
	// <0x73e479>|0x009|+0x03c:'102'
	// ******
}

// STATE[UNCHECKED]
void game_material_manager::add_game_material( game_material const* const mtrl )
{
	m_materials[mtrl->id( )] = mtrl;

	// FUNCTION BODY
	// <0x73e7a0>|0x010|+0x02a:'131'
	// ******
}

// STATE[UNCHECKED]
void game_material_manager::add_pair( material_pair const* const pair )
{
	u16 first_mtrl_id	= pair->first_material( )->id( );
	u16 second_mtrl_id	= pair->second_material( )->id( );
	m_pairs[first_mtrl_id][second_mtrl_id] = pair;

	// FUNCTION BODY
	// <0x73e926>|0x016|+0x01c:'136'
	// <0x73e942>|0x032|+0x01c:'137'
	// <0x73e95e>|0x04e|+0x025:'138'
	// ******
}

} // namespace survarium
