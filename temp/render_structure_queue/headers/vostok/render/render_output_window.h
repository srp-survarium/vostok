////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_OUTPUT_WINDOW_H_INCLUDED
#define RENDER_RENDER_OUTPUT_WINDOW_H_INCLUDED

/* INCLUDES */
struct HWND__;
class vostok::render::renderer_context_targets;
struct vostok::render::base_output_window;
struct survarium::flash_renderer;
class vostok::math::uint2;
typedef vostok::intrusive_ptr<vostok::render::res_render_output,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::res_render_output_ptr;
class vostok::render::res_render_output;

/* FORWARD REFS */
class vostok::render::output_window_configuration;

namespace vostok {
namespace render {

class render_output_window : public base_output_window {
public:
			explicit							render_output_window	( output_window_configuration const& window_configuration );

			void								resize					( bool force_resize );

			void								set_size				(
													const u32		width,
													const u32		height,
													const bool		fullscreen,
													bool			force_resize
												);

	inline	res_render_output*					render_output			( ) const { /* no source */ }

	inline	renderer_context_targets const&		target_context			( ) const { /* no source */ }

	inline	HWND__*								window					( ) const { /* no source */ }

	inline	math::uint2							get_window_client_size	( ) const { /* no source */ }

			void								goto_fullscreen			( );

	virtual										~render_output_window	( ) { /* no source */ }

private:
	static	math::uint2							get_window_client_size	( HWND__* window, bool windowed );

	/* 0x0000 */	/* base_output_window */
	/* 0x0110 */	renderer_context_targets		m_targets;
	/* 0x2ce0 */	res_render_output_ptr			m_output;
	/* 0x2ce4 */	HWND__*							m_window;
	/* 0x2ce8 */	bool							m_windowed;
public:
	/* 0x2cec */	survarium::flash_renderer*		m_flash_renderer;
}; // class render_output_window

STATIC_SIZE_ASSERT(render_output_window, 0x2CF0);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_OUTPUT_WINDOW_H_INCLUDED
