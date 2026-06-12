////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FLASH_RENDERER_H_INCLUDED
#define FLASH_RENDERER_H_INCLUDED

/* INCLUDES */
class Scaleform::Render::D3D1x::HAL;
class Scaleform::Render::Renderer2D;

/* FORWARD REFS */
class survarium::flash_movie;
class survarium::flash_text_manager;
class survarium::scaleform_render_command_queue;

namespace survarium {

struct flash_renderer {
	inline			flash_renderer	(
						scaleform_render_command_queue*		arg_0,
						ID3D11Device*						arg_1,
						ID3D11DeviceContext*				arg_2
					) { /* no source */ }
	inline			~flash_renderer	( ) { /* no source */ }

	inline	void	present			( flash_movie** arg_0, u32 arg_1, flash_text_manager* arg_2 ) { /* no source */ }

	inline	void	on_reset_device	(
						u32						arg_0,
						u32						arg_1,
						ID3D11Device*			arg_2,
						ID3D11DeviceContext*	arg_3
					) { /* no source */ }
	inline	void	on_create_device( ) { /* no source */ }
	inline	void	on_lost_device	( ) { /* no source */ }

public:
	/* 0x0000 */	u32									m_output_width;
	/* 0x0004 */	u32									m_output_height;
	/* 0x0008 */	Scaleform::Render::D3D1x::HAL*		m_HALRenderer;
	/* 0x000c */	Scaleform::Render::Renderer2D*		m_R2dRenderer;
}; // struct flash_renderer

STATIC_SIZE_ASSERT(flash_renderer, 0x10);

} // namespace survarium

#endif // #ifndef FLASH_RENDERER_H_INCLUDED
