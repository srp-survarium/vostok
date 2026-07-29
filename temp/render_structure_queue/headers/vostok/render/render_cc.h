////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_CC_H_INCLUDED
#define RENDER_RENDER_CC_H_INCLUDED

/* INCLUDES */
class vostok::render::render_cc;
class vostok::render::enum_options_changes_result;

/* FORWARD REFS */
class vostok::render::shader_macro;

namespace vostok {
namespace render {

enum vostok::render::enum_options_changes_result
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

class render_cc {
public:
											render_cc			( pcstr define_name, enum_options_changes_result changed_result );

	inline	pcstr							define_name			( ) const { /* no source */ }

private:
	virtual	bool							fill_macro			( shader_macro& arg_0 ) const = 0;

	inline	enum_options_changes_result		get_changes_result	( ) const { /* no source */ }

	virtual	bool							is_changed			( ) const = 0;

	/* 0x0004 */	render_cc*						render_next;
	/* 0x0008 */	pcstr							m_define_name;
	/* 0x000c */	enum_options_changes_result		m_changes_result;
}; // class render_cc

STATIC_SIZE_ASSERT(render_cc, 0x10);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_CC_H_INCLUDED
