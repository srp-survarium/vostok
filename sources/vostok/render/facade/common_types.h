// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_FACADE_COMMON_TYPES_H_INCLUDED
#define VOSTOK_RENDER_FACADE_COMMON_TYPES_H_INCLUDED
#include <vostok/math_rectangle.h>
#include <vostok/vectora.h>

namespace survarium {

class scaleform_render_command_queue;

} // namespace survarium

namespace vostok {
namespace render {

enum {
	terrain_texture_max_count	= 0x10,
	terrain_texture_size		= 0x400,
};

#pragma pack(push, 1)

struct terrain_data {
	float	height;
	float2	tex_shift;
	u8		tex_id0;
	u8		tex_id1;
	u8		tex_id2;
	u8		alpha0;
	u8		alpha1;
	u8		alpha2;
	u32		color;
};

STATIC_SIZE_ASSERT( terrain_data, 0x16 );

#pragma pack(pop)

struct buffer_fragment {
	u32 start;
	u32 size;
	void* buffer;
};

STATIC_SIZE_ASSERT( buffer_fragment, 0xc );

struct terrain_buffer_fragment {
	explicit terrain_buffer_fragment( memory::base_allocator* allocator ) :
		buffer( allocator )
	{
	}

	vectora<terrain_data>			buffer;
	math::rectangle<math::uint2>	rect;
};

STATIC_SIZE_ASSERT( terrain_buffer_fragment, 0x20 );

struct scene_configuration {
	inline scene_configuration( ) : m_create_terrain( false ), m_create_particle_world( false ), m_has_clouds( false ), m_create_speedtree_world( false ), m_create_grass_world( false ), m_sky_enabled( false ), m_use_occlusion_culling( false ) { }

	bool m_create_terrain			: 1;
	bool m_create_particle_world	: 1;
	bool m_has_clouds				: 1;
	bool m_create_speedtree_world	: 1;
	bool m_create_grass_world		: 1;
	bool m_sky_enabled				: 1;
	bool m_use_occlusion_culling	: 1;
};

STATIC_SIZE_ASSERT( scene_configuration, 0x1 );

struct output_window_configuration {
	inline output_window_configuration( ) :
		hwnd( 0 ),
		width( 0 ),
		height( 0 ),
		create_flash_renderer( false ),
		windowed( true ),
		scaleform_render_queue( 0 )
	{
	}

	void*										hwnd;
	u32											width;
	u32											height;
	bool										create_flash_renderer;
	bool										windowed;
	survarium::scaleform_render_command_queue*	scaleform_render_queue;
};

STATIC_SIZE_ASSERT( output_window_configuration, 0x14 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_FACADE_COMMON_TYPES_H_INCLUDED
