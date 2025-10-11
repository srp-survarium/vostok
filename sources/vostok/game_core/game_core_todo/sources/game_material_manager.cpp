////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "game_material_manager.h"

namespace survarium {

// STATE[STUB]
// survarium::game_material_manager::game_material_manager()
game_material_manager::game_material_manager( )
{
}

// STATE[STUB]
// void survarium::game_material_manager::clear_resources()
void game_material_manager::clear_resources( )
{
	// FUNCTION BODY
	// <0x73e8f7>|0x000|0x000:'20'
	// <0x73e8ff>|0x008|0x008:'21'
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager::~game_material_manager()
void game_material_manager::~game_material_manager( )
{
	// FUNCTION BODY
	// <0x73e9a2>|0x000|0x000:'26'
	// <0x73e9ae>|0x00c|0x00c:'27'
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager::delete_pairs()
void game_material_manager::delete_pairs( )
{
	// LOCALS
	// std::priv::_Rb_tree_iterator<std::pair<u16 const ,map<u16,material_pair const *,std::less<u16> > >,std::priv::_MapTraitsT<std::pair<u16 const ,map<u16,material_pair const *,std::less<u16> > > > > end
	// std::priv::_Rb_tree_iterator<std::pair<u16 const ,map<u16,material_pair const *,std::less<u16> > >,std::priv::_MapTraitsT<std::pair<u16 const ,map<u16,material_pair const *,std::less<u16> > > > > it
	// std::priv::_Rb_tree_iterator<std::pair<u16 const ,material_pair const *>,std::priv::_MapTraitsT<std::pair<u16 const ,material_pair const *> > > internail_it<1>
	// std::priv::_Rb_tree_iterator<std::pair<u16 const ,material_pair const *>,std::priv::_MapTraitsT<std::pair<u16 const ,material_pair const *> > > internail_end<1>
	// ******

	// FUNCTION BODY
	// <0x73e7d9>|0x000|0x000:'32'
	// <0x73e7f0>|0x017|0x017:'33'
	// 1
	// <0x73e801>|0x028|0x011:'35'
	// 1
	// <0x73e83d>|0x064|0x03c|[1]:'37'
	// <0x73e85a>|0x081|0x01d:'38'
	// 1
	// <0x73e86e>|0x095|0x014:'40'
	// <0x73e8a6>|0x0cd|0x038:'41'
	// <0x73e8d6>|0x0fd|0x030:'42'
	// <0x73e8db>|0x102|0x005:'43'
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager::delete_materials()
void game_material_manager::delete_materials( )
{
	// LOCALS
	// std::priv::_Rb_tree_iterator<std::pair<u16 const ,game_material const *>,std::priv::_MapTraitsT<std::pair<u16 const ,game_material const *> > > end
	// std::priv::_Rb_tree_iterator<std::pair<u16 const ,game_material const *>,std::priv::_MapTraitsT<std::pair<u16 const ,game_material const *> > > it
	// ******

	// FUNCTION BODY
	// <0x73e6e9>|0x000|0x000:'48'
	// <0x73e700>|0x017|0x017:'49'
	// <0x73e711>|0x028|0x011:'50'
	// <0x73e749>|0x060|0x038:'51'
	// <0x73e779>|0x090|0x030:'52'
	// ******
}

// STATE[STUB]
// survarium::game_material const* survarium::game_material_manager::get_material(unsigned short) const
game_material const* game_material_manager::get_material( u16 id ) const
{
	// LOCALS
	// std::priv::_Rb_tree_iterator<std::pair<u16 const ,game_material const *>,std::priv::_ConstMapTraitsT<std::pair<u16 const ,game_material const *> > > it
	// ******

	return NULL;
	// FUNCTION BODY
	// <0x73e669>|0x000|0x000:'58'
	// <0x73e689>|0x020|0x020:'59'
	// ******
}

// STATE[STUB]
// survarium::material_pair const* survarium::game_material_manager::get_pair(unsigned short, unsigned short) const
material_pair const* game_material_manager::get_pair( u16 first_mtrl_id, u16 second_mtrl_id ) const
{
	// LOCALS
	// u16 							second_material
	// std::priv::_Rb_tree_iterator<std::pair<u16 const ,material_pair const *>,std::priv::_ConstMapTraitsT<std::pair<u16 const ,material_pair const *> > > second_it
	// u16 							first_material
	// std::priv::_Rb_tree_iterator<std::pair<u16 const ,map<u16,material_pair const *,std::less<u16> > >,std::priv::_ConstMapTraitsT<std::pair<u16 const ,map<u16,material_pair const *,std::less<u16> > > > > first_it
	// ******

	return NULL;
	// FUNCTION BODY
	// <0x73e4c9>|0x000|0x000:'64'
	// <0x73e4fd>|0x034|0x034:'65'
	// 1
	// <0x73e531>|0x068|0x034:'67'
	// <0x73e551>|0x088|0x020:'68'
	// <0x73e574>|0x0ab|0x023:'69'
	// <0x73e582>|0x0b9|0x00e:'70'
	// 1
	// <0x73e5a8>|0x0df|0x026:'72'
	// 1
	// <0x73e5b4>|0x0eb|0x00c:'74'
	// <0x73e5d7>|0x10e|0x023:'75'
	// <0x73e600>|0x137|0x029:'76'
	// <0x73e60e>|0x145|0x00e:'77'
	// 1
	// <0x73e63a>|0x171|0x02c:'79'
	// <0x73e646>|0x17d|0x00c:'80'
	// ******
}

// STATE[STUB]
// bool survarium::game_material_manager::material_exist(unsigned short) const
bool game_material_manager::material_exist( u16 id ) const
{
	return false;
	// FUNCTION BODY
	// <0x73e479>|0x000|0x000:'102'
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager::add_game_material(survarium::game_material const* const)
void game_material_manager::add_game_material( game_material const* mtrl )
{
	// FUNCTION BODY
	// <0x73e7a0>|0x000|0x000:'131'
	// ******
}

// STATE[STUB]
// void survarium::game_material_manager::add_pair(survarium::material_pair const* const)
void game_material_manager::add_pair( material_pair const* pair )
{
	// LOCALS
	// u16 							first_mtrl_id
	// u16 							second_mtrl_id
	// ******

	// FUNCTION BODY
	// <0x73e926>|0x000|0x000:'136'
	// <0x73e942>|0x01c|0x01c:'137'
	// <0x73e95e>|0x038|0x01c:'138'
	// ******
}

} // namespace survarium
