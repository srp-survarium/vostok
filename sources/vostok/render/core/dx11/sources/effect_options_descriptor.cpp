#include "pch.h"

#include <vostok/render/core/effect_options_descriptor.h>
#include <vostok/render/core/custom_config.h>

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
	// FUNCTION BODY[0x559a10]
}

effect_options_descriptor::effect_options_descriptor( pcvoid in_data, u16 in_size )
{
	// FUNCTION BODY[0x5599e0]
	data	= (pbyte)in_data + sizeof(effect_options_descriptor);
	type	= configs::t_table_named;
	bytes	= 0;
	count	= 0;
	id		= 0;
	destroyer = 0;
	memory_size = in_size;
}

effect_options_descriptor::~effect_options_descriptor( )
{
	// FUNCTION BODY[0x5599d0]
	// Created effect configs call destructors.
	//call_destructors();
}

u32 effect_options_descriptor::get_crc( ) const
{
	// FUNCTION BODY[0x559bd0]
	u32 num_bytes_for_data = calc_config_memory_usage(*this);

	pcvoid mem_buffer = ALLOCA(num_bytes_for_data);

	vostok::mutable_buffer mbuffer( pbyte(mem_buffer), num_bytes_for_data );

	u32 crc;
	custom_config_ptr config = create_custom_config(*this, mbuffer, crc, true);
	config->own_buffer		 = false;
	config->call_destructors = false;

	return crc;
}

u32 effect_options_descriptor::get_data_memory_usage( u32& need_bytes_to_align4, u32& last_align_value ) const
{
	// FUNCTION BODY[0x559a30]
	u32 result = 0;

	if (id)
		result = strings::length(id) + 1;

	if (type==configs::t_table_named)
	{
		effect_options_descriptor* children = (effect_options_descriptor*)data;
		for (u32 i=0; i<count; i++)
		{
			result += children->get_data_memory_usage(need_bytes_to_align4, last_align_value);
			children = (effect_options_descriptor*)((pbyte)children + children->get_num_used_bytes());
		}
	}
	else
	{
		if (count > sizeof(u32))
			result += count;

		u32 const rem = result % 4;

		if (rem)
		{
			last_align_value = 4 - rem;
			need_bytes_to_align4 += last_align_value;
		}
	}
	return result;
}

u32 effect_options_descriptor::get_num_total_fields( ) const
{
	// FUNCTION BODY[0x559980]
	if (type!=configs::t_table_named)
		return 0;

	u32 result = count;

	effect_options_descriptor* children = (effect_options_descriptor*)data;
	for (u32 i=0; i<count; i++)
	{
		result += children->get_num_total_fields();
		children = (effect_options_descriptor*)((pbyte)children + children->get_num_used_bytes());
	}
	return result;
}

u32 effect_options_descriptor::get_num_used_bytes( ) const
{
	// FUNCTION BODY[0x559930]
	u32 result = bytes;

	if (type==configs::t_table_named)
	{
		effect_options_descriptor* children = (effect_options_descriptor*)data;
		for (u32 i=0; i<count; i++)
		{
			result += children->get_num_used_bytes();
			children = (effect_options_descriptor*)((pbyte)children + children->get_num_used_bytes());
		}
	}
	return result;
}

// claude@NOTE: 271 bytes and 23 blocks on both sides, 16/16 statements with identical line
// deltas. `sema blocks --diff` reports flow DIFFERS and names a "missing early-out" at B2 -
// that is an artifact: our 1-byte `nop` alignment pad is its own block and displaces every
// later index (see sema_tools.md "degenerate fall-through blocks"). The real residual is
// register allocation: the target keeps a CSE'd zero in edx (an extra `xor edx,edx` at
// entry and one more at the loop tail) and spells the two null tests `cmp reg,edx` where we
// emit `test reg,reg`, then reuses that zero for the placement-new ctor's four stores.
effect_options_descriptor& effect_options_descriptor::operator[]( pcstr key )
{
	u32 offset = 0;

	if (type==configs::t_table_named)
	{
		effect_options_descriptor* children = (effect_options_descriptor*)data;

		for (u32 i=0; i<count; i++)
		{
			if (children->id && key && vostok::strings::compare(children->id, key) == 0)
			{
				return *children;
			}
			offset += children->get_num_used_bytes();
			children = (effect_options_descriptor*)((pbyte)children + children->get_num_used_bytes());
		}
	}

	effect_options_descriptor& v = *new(data+offset)effect_options_descriptor;

	vostok::memory::copy(data + offset + sizeof(v), strings::length(key)+1, key, strings::length(key)+1);

	count++;

	v.id = pcstr(data + offset + sizeof(v));
	v.type = configs::t_table_named;
	v.bytes = sizeof(v) + strings::length(key) + 1;
	v.data  = data + offset + v.bytes;
	v.count = 0;
	v.destroyer = 0;

	return v;
}

} // namespace render
} // namespace vostok
