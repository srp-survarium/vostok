////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_ENUM_OPTIONS_CHANGES_RESULT_H_INCLUDED
#define RENDER_ENUM_OPTIONS_CHANGES_RESULT_H_INCLUDED

namespace vostok {
namespace render {

enum enum_options_changes_result
{
	ocr_need_nothing					= 0x0001,
	ocr_need_resatart_application		= 0x0002,
	ocr_need_reload_textures			= 0x0004,
	ocr_need_reload_geometry			= 0x0008,
	ocr_need_reload_shaders				= 0x0010,
	ocr_need_recreate_samplers			= 0x0020,
	ocr_need_update_vegetation			= 0x0040,
	ocr_need_resize_window				= 0x0080,
	ocr_need_recreate_device			= 0x0100,
	ocr_need_reset_renderer				= 0x0200,
	ocr_need_reset_postprocess			= 0x0400,
	ocr_need_reset_lighting				= 0x0800,
	ocr_need_reset_rain					= 0x1000,
};

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_ENUM_OPTIONS_CHANGES_RESULT_H_INCLUDED
