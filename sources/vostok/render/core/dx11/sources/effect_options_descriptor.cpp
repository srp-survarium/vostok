#include "pch.h"

#include <vostok/render/core/effect_options_descriptor.h>

namespace vostok {
namespace render {

effect_options_descriptor::effect_options_descriptor( ) :
	id( 0 ),
	destroyer( 0 ),
	data( 0 ),
	bytes( 0 ),
	type( 0 ),
	count( 0 ),
	memory_size( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x559a10]
}

effect_options_descriptor::effect_options_descriptor( pcvoid in_data, u16 in_size ) :
	id( 0 ),
	destroyer( 0 ),
	data( reinterpret_cast<pbyte>( const_cast<pvoid>( in_data ) ) + sizeof( effect_options_descriptor ) ),
	bytes( 0 ),
	type( configs::t_table_named ),
	count( 0 ),
	memory_size( in_size )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5599e0]
}

effect_options_descriptor::~effect_options_descriptor( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x5599d0]
}

u32 effect_options_descriptor::get_crc( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x559bd0]
	return 0;
}

u32 effect_options_descriptor::get_data_memory_usage( u32&, u32& ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x559a30]
	return 0;
}

u32 effect_options_descriptor::get_num_total_fields( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x559980]
	return 0;
}

u32 effect_options_descriptor::get_num_used_bytes( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x559930]
	return bytes;
}

effect_options_descriptor& effect_options_descriptor::operator[]( pcstr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x559ac0]
	return *this;
}

} // namespace render
} // namespace vostok
