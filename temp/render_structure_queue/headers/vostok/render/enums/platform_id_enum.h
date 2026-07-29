////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_PLATFORM_ID_ENUM_H_INCLUDED
#define RENDER_PLATFORM_ID_ENUM_H_INCLUDED

namespace vostok {
namespace render {

enum platform_id_enum
{
	platform_uni				= 0x0,
	platform_windows_dx11		= 0x1,
	platform_xbox360			= 0x2,
	platform_ps3				= 0x3,
};

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_PLATFORM_ID_ENUM_H_INCLUDED
