////////////////////////////////////////////////////////////////////////////
//	Created		: 05.03.2009
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef COMMON_TYPES_H_INCLUDED
#define COMMON_TYPES_H_INCLUDED

namespace survarium {

class scaleform_render_command_queue;

} // namespace survarium

namespace vostok {
namespace render {

////////////////////////////////////////////////////////////////////////////
// Terrain defines
////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)
struct terrain_data
{
	float		height;

	float2		tex_shift;

	u8			tex_id0;
	u8			tex_id1;
	u8			tex_id2;

	u8			alpha0;
	u8			alpha1;
	u8			alpha2;

	u32			color;

}; // class terrain_data
#pragma pack(pop)

struct buffer_fragment 
{
	u32		start;
	u32		size;
	void*	buffer;

}; // class buffer_fragment

struct terrain_buffer_fragment // was : buffer_fragment_NEW
{
	terrain_buffer_fragment(vostok::memory::base_allocator* allocator):buffer(allocator){}
	vectora<terrain_data>	buffer;
	math::rectangle<math::uint2>	rect;
}; // class terrain_buffer_fragment

enum {
	terrain_texture_max_count	= 16,
	terrain_texture_size		= 1024,
};

} // namespace render 
} // namespace vostok 

#endif // #ifndef COMMON_TYPES_H_INCLUDED
