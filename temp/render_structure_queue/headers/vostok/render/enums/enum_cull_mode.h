////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENUM_CULL_MODE_H_INCLUDED
#define RENDER_ENUM_CULL_MODE_H_INCLUDED

namespace vostok {
namespace render {

enum enum_cull_mode
{
	cull_mode_none		= 0x0,
	cull_mode_front		= 0x1,
	cull_mode_back		= 0x2,
};

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENUM_CULL_MODE_H_INCLUDED
