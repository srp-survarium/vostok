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

namespace vostok {
namespace render {

class render_output_window: public base_output_window {
public:
											render_output_window( HWND window );
			void							resize				( );
	inline	res_render_output*				render_output		( )	const	{ return &*m_output; }
	inline	renderer_context_targets const&	target_context		( ) const	{ return m_targets; }
	inline	HWND							window				( ) const	{ return m_window; }
	inline	math::uint2					get_window_client_size	( ) const	{ return get_window_client_size( m_window ); }

private:
	static	math::uint2					get_window_client_size	( HWND window );

private:
	renderer_context_targets		m_targets;
	math::uint2						m_current_size;
	ref_render_output				m_output;
	HWND							m_window;
}; // class render_output_window

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_OUTOUT_WINDOW_H_INCLUDED