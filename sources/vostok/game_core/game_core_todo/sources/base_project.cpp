////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "base_project.h"

namespace survarium {

// STATE[STUB]
// survarium::base_project::base_project()
base_project::base_project( )
{
}

// STATE[STUB]
// void survarium::base_project::~base_project()
void base_project::~base_project( )
{
	// FUNCTION BODY
	// <0x6ff432>|0x000|0x000:'24'
	// <0x6ff43b>|0x009|0x009:'25'
	// ******
}

// STATE[STUB]
// survarium::base_game_object* survarium::base_project::get_object_by_name(char const*)
base_game_object* base_project::get_object_by_name( pcstr name )
{
	return NULL;
	// FUNCTION BODY
	// <0x6ff3f9>|0x000|0x000:'30'
	// <0x6ff405>|0x00c|0x00c:'31'
	// ******
}

// STATE[STUB]
// void survarium::base_project::resolve_links()
void base_project::resolve_links( )
{
	// LOCALS
	// base_project::resolve_link_object* end
	// base_project::resolve_link_object* it
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ee344 }, type_index: TypeIndex(0x951c) })
	// ******

	// FUNCTION BODY
	// <0x6ff2c9>|0x000|0x000:'36'
	// <0x6ff2dd>|0x014|0x014:'37'
	// <0x6ff2f1>|0x028|0x014:'38'
	// <0x6ff304>|0x03b|0x013:'39'
	// 1
	// <0x6ff348>|0x07f|0x044:'41'
	// ******
}

// STATE[STUB]
// void survarium::read_transform(vostok::configs::binary_config_value const&, vostok::math::float4x4&)
void read_transform( vostok::configs::binary_config_value const& cfg, vostok::math::float4x4& result )
{
	// LOCALS
	// vostok::math::float3 const& 	rotation
	// vostok::math::float3 const& 	scale
	// vostok::math::float3 const& 	position
	// ******

	// FUNCTION BODY
	// <0x6ff21b>|0x000|0x000:'46'
	// <0x6ff230>|0x015|0x015:'47'
	// <0x6ff245>|0x02a|0x015:'48'
	// <0x6ff25a>|0x03f|0x015:'49'
	// ******
}

// STATE[STUB]
// void survarium::static_collision::insert(vostok::physics::world*)
void static_collision::insert( vostok::physics::world* w )
{
	// LOCALS
	// vostok::physics::bt_rigid_body_construction_info info
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ee4e2 }, type_index: TypeIndex(0x94e2) })
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ee506 }, type_index: TypeIndex(0x9464) })
	// ******

	// FUNCTION BODY
	// <0x6ff499>|0x000|0x000:'109'
	// <0x6ff4a1>|0x008|0x008:'110'
	// <0x6ff4b0>|0x017|0x00f:'111'
	// 1
	// <0x6ff4bd>|0x024|0x00d:'113'
	// 1
	// <0x6ff4cb>|0x032|0x00e:'115'
	// 1
	// 2
	// 3
	// <0x6ff4e4>|0x04b|0x019:'119'
	// ******
}

// STATE[STUB]
// void survarium::static_collision::remove(vostok::physics::world*)
void static_collision::remove( vostok::physics::world* w )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x6ee1ea }, type_index: TypeIndex(0x9467) })
	// ******

	// FUNCTION BODY
	// <0x6ff1d8>|0x000|0x000:'124'
	// <0x6ff1ec>|0x014|0x014:'125'
	// <0x6ff1f7>|0x01f|0x00b:'126'
	// ******
}

	/* TYPEDEFS

	typedef
		base_project::resolve_link_object*
		iterator_type;

} // namespace survarium
