////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_OUTPUT_WINDOW_CONFIGURATION_H_INCLUDED
#define RENDER_OUTPUT_WINDOW_CONFIGURATION_H_INCLUDED

/* INCLUDES */
class survarium::scaleform_render_command_queue;

namespace vostok {
namespace render {

struct output_window_configuration {
	inline		output_window_configuration	( ) { /* no source */ }

	/* 0x0000 */	void*		hwnd;
	/* 0x0004 */	u32			width;
	/* 0x0008 */	u32			height;
	/* 0x000c */	bool		create_flash_renderer;
	/* 0x000d */	bool		windowed;
	/* 0x0010 */	survarium::scaleform_render_command_queue*	scaleform_render_queue;
}; // struct output_window_configuration

STATIC_SIZE_ASSERT(output_window_configuration, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_OUTPUT_WINDOW_CONFIGURATION_H_INCLUDED
