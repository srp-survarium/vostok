////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENUM_CONSTANT_TYPE_H_INCLUDED
#define RENDER_ENUM_CONSTANT_TYPE_H_INCLUDED

namespace vostok {
namespace render {

enum enum_constant_type
{
	rc_float		= 0x0000,
	rc_int			= 0x0001,
	rc_bool			= 0x0002,
	rc_INVALID		= 0xffff,
};

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENUM_CONSTANT_TYPE_H_INCLUDED
