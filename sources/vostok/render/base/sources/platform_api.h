////////////////////////////////////////////////////////////////////////////
//	Created		: 02.06.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef PLATFORM_API_H_INCLUDED
#define PLATFORM_API_H_INCLUDED

namespace vostok {
namespace render {

struct platform;

	render::platform * create_render_platform( vostok::render::engine::wrapper& wrapper, HWND window_handle);

} // namespace render
} // namespace vostok

#endif // #ifndef PLATFORM_API_H_INCLUDED