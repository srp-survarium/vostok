#ifndef VOSTOK_RENDER_CORE_TEXTURE_OPTIONS_H_INCLUDED
#define VOSTOK_RENDER_CORE_TEXTURE_OPTIONS_H_INCLUDED

namespace vostok {
namespace render {
namespace texture_options {

enum ETType {
	sky				= 0x0,
	bump			= 0x1,
	normal_map		= 0x2,
	terrain			= 0x3,
	terrain_bump	= 0x4,
	terrain_color	= 0x5,
	type_2d			= 0x6,
};

} // namespace texture_options
} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_TEXTURE_OPTIONS_H_INCLUDED
