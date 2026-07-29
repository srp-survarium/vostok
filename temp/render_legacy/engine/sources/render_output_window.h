////////////////////////////////////////////////////////////////////////////
//	Created		: 20.09.2010
//	Author		: Armen Abroyan
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RENDER_ENGINE_RENDER_OUTOUT_WINDOW_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_OUTOUT_WINDOW_H_INCLUDED

#include <vostok/render/core/res_render_output.h>
#include "renderer_context_targets.h"
#include <vostok/render/engine/base_classes.h>

namespace survarium {

struct flash_renderer;

} // namespace survarium

namespace vostok {
namespace render {

struct output_window_configuration;

class render_output_window: public base_output_window {
public:
	explicit								render_output_window	( output_window_configuration const& window_configuration );
			void							resize					( bool force_resize );
			void							set_size				(
												u32 const width,
												u32 const height,
												bool const fullscreen,
												bool force_resize
											);
	inline	res_render_output*				render_output		( )	const	{ return &*m_output; }
	inline	renderer_context_targets const&	target_context		( ) const	{ return m_targets; }
	inline	HWND							window				( ) const	{ return m_window; }
	inline	math::uint2						get_window_client_size	( ) const	{ return get_window_client_size( m_window, m_windowed ); }
			void							goto_fullscreen			( );

private:
	static	math::uint2						get_window_client_size	( HWND window, bool windowed );

private:
	renderer_context_targets			m_targets;
	ref_render_output					m_output;
	HWND								m_window;
	bool								m_windowed;
public:
	survarium::flash_renderer*			m_flash_renderer;

}; // class render_output_window

STATIC_SIZE_ASSERT( render_output_window, 0x2CF0 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_OUTOUT_WINDOW_H_INCLUDED
