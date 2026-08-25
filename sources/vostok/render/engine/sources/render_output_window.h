#ifndef VOSTOK_RENDER_ENGINE_RENDER_OUTPUT_WINDOW_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_OUTPUT_WINDOW_H_INCLUDED

#include <vostok/render/core/res_render_output.h>
#include <vostok/render/engine/base_classes.h>
#include <vostok/render/facade/common_types.h>

#include "renderer_context_targets.h"

struct HWND__;

namespace survarium {
struct flash_renderer;
}

namespace vostok {
namespace render {

class renderer;

class render_output_window : public base_output_window {
public:
	explicit render_output_window(
		output_window_configuration const& window_configuration
	);

	void resize( bool force_resize );
	void set_size(
		u32 const width,
		u32 const height,
		bool const fullscreen,
		bool force_resize
	);

	res_render_output* render_output( ) const { return &*m_output; }
	renderer_context_targets const& target_context( ) const { return m_targets; }
	HWND__* window( ) const { return m_window; }
	math::uint2 get_window_client_size( ) const
	{
		return get_window_client_size( m_window, m_windowed );
	}

	void goto_fullscreen( );

private:
	static math::uint2 get_window_client_size( HWND__* window, bool windowed );

	friend class renderer;

	renderer_context_targets m_targets;
	res_render_output_ptr m_output;
	HWND__* m_window;
	bool m_windowed;

public:
	survarium::flash_renderer* m_flash_renderer;
};

STATIC_SIZE_ASSERT( render_output_window, 0x2CF0 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_OUTPUT_WINDOW_H_INCLUDED
