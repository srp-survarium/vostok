////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENUM_SHADER_TYPE_H_INCLUDED
#define RENDER_ENUM_SHADER_TYPE_H_INCLUDED

namespace vostok {
namespace render {

enum enum_shader_type
{
	enum_shader_type_vertex			= 0x0,
	enum_shader_type_pixel			= 0x1,
	enum_shader_type_geometry		= 0x2,
	enum_shader_types_count			= 0x3,
};

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENUM_SHADER_TYPE_H_INCLUDED
