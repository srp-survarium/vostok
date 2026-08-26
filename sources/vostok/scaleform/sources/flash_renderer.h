////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef FLASH_RENDERER_H_INCLUDED
#define FLASH_RENDERER_H_INCLUDED

// survarium::flash_renderer (impl TU: renderer.cpp) - a name collision with
// the legacy vostok::render::flash_renderer, NOT the same type (different
// namespace, ctor and member set; see docs/binary_matching/game/README.md).

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace Scaleform {
namespace Render {
	class Renderer2D;
	namespace D3D1x {
		class HAL;
	} // namespace D3D1x
} // namespace Render
} // namespace Scaleform

namespace survarium {

struct flash_movie;
struct flash_text_manager;
class scaleform_render_command_queue;

struct flash_renderer {
					flash_renderer	(
						scaleform_render_command_queue*		arg_0,
						ID3D11Device*						arg_1,
						ID3D11DeviceContext*				arg_2
					);
	inline			~flash_renderer	( ) { }

			void	present			( flash_movie** arg_0, u32 arg_1, flash_text_manager* arg_2 );

			void	on_reset_device	(
						u32						arg_0,
						u32						arg_1,
						ID3D11Device*			arg_2,
						ID3D11DeviceContext*	arg_3
					);
	inline	void	on_create_device( ) { }
	inline	void	on_lost_device	( ) { }

private:
	/* 0x0000 */	u32									m_output_width;
	/* 0x0004 */	u32									m_output_height;
	/* 0x0008 */	Scaleform::Render::D3D1x::HAL*		m_HALRenderer;
	/* 0x000c */	Scaleform::Render::Renderer2D*		m_R2dRenderer;
}; // struct flash_renderer

STATIC_SIZE_ASSERT(flash_renderer, 0x10);

} // namespace survarium

#endif // #ifndef FLASH_RENDERER_H_INCLUDED
