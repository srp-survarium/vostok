#ifndef VOSTOK_RENDER_ENGINE_RENDER_OUTPUT_WINDOW_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_OUTPUT_WINDOW_H_INCLUDED

#include <vostok/render/engine/base_classes.h>
#include <vostok/render/facade/common_types.h>

struct HWND__;

namespace vostok {
namespace render {

class render_output_window : public base_output_window {
public:
	explicit render_output_window( output_window_configuration const& window_configuration );
	virtual ~render_output_window( ) { }

	void resize( bool force_resize );
	void set_size( u32 width, u32 height, bool fullscreen, bool force_resize );
	void goto_fullscreen( );

private:
	static math::uint2 get_window_client_size( HWND__* window, bool windowed );
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_OUTPUT_WINDOW_H_INCLUDED
