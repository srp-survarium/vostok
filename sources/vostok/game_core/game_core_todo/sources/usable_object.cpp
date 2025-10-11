////////////////////////////////////////////////////////////////////////////
//	Created 	: 12.10.2025
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "usable_object.h"

namespace survarium {

// STATE[STUB]
// survarium::usable_object::usable_object()
usable_object::usable_object( )
{
}

// STATE[STUB]
// void survarium::usable_object::~usable_object()
void usable_object::~usable_object( )
{
	// FUNCTION BODY
	// <0x5a0b1c>|0x000|0x000:'22'
	// ******
}

// STATE[STUB]
// void survarium::usable_object::load(vostok::configs::binary_config_value const&)
void usable_object::load( vostok::configs::binary_config_value const& cfg )
{
	// LOCALS
	// vostok::configs::binary_config_value collision_table
	// ******

	// FUNCTION BODY
	// <0x5a0b8a>|0x000|0x000:'27'
	// <0x5a0bba>|0x030|0x030:'28'
	// <0x5a0bc8>|0x03e|0x00e:'29'
	// 1
	// <0x5a0bd4>|0x04a|0x00c:'31'
	// ******
}

// STATE[STUB]
// void survarium::usable_object::resolve_links(survarium::base_project*, vostok::configs::binary_config_value)
void usable_object::resolve_links( base_project* p, vostok::configs::binary_config_value cfg )
{
	// LOCALS
	// vostok::configs::binary_config_value collision_table
	// u32 							i<1>
	// pcstr 						geom_name<2>
	// ******

	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58fac4 }, type_index: TypeIndex(0x94fb) })
	// ******

	// FUNCTION BODY
	// <0x5a0a49>|0x000|0x000:'36'
	// <0x5a0a79>|0x030|0x030|[1]:'37'
	// 1
	// <0x5a0a96>|0x04d|0x01d|[2]:'39'
	// <0x5a0ab5>|0x06c|0x01f:'40'
	// 1
	// ******
}

// STATE[STUB]
// void survarium::usable_object::insert(vostok::physics::world*)
void usable_object::insert( vostok::physics::world* world )
{
	// LOCALS
	// u32 							i<1>
	// ******

	// FUNCTION BODY
	// <0x5a09f9>|0x000|0x000|[1]:'46'
	// <0x5a0a16>|0x01d|0x01d:'47'
	// ******
}

// STATE[STUB]
// void survarium::usable_object::remove()
void usable_object::remove( )
{
	// LOCALS
	// u32 							i<1>
	// ******

	// FUNCTION BODY
	// <0x5a0919>|0x000|0x000|[1]:'52'
	// <0x5a0936>|0x01d|0x01d:'53'
	// ******
}

// STATE[STUB]
// vostok::math::float4x4 survarium::usable_object::get_transform()
vostok::math::float4x4 usable_object::get_transform( )
{
	// OTHER SYMBOLS
	// CallSiteInfo(CallSiteInfoSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x58f900 }, type_index: TypeIndex(0x209ee) })
	// ******

	return vostok::math::float4x4();
	// FUNCTION BODY
	// <0x5a08e7>|0x000|0x000:'58'
	// ******
}

	/* TYPEDEFS

	typedef
		base_project::resolve_link_object*
		iterator_type;

} // namespace survarium
