#include "pch.h"
#include "render_output_window.h"
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/render_target.h>
#include <vostok/render/core/backend.h>
#include <vostok/render/facade/common_types.h>
#include <vostok/scaleform/sources/flash_renderer.h>

namespace vostok {
namespace render {

// claude@NOTE: legacy body; the legacy .cpp generation carried an
// m_current_size member the shipped class does not have (both the legacy
// header and the canonical one are 0x2CF0 without it) - current-size reads
// are adapted to locals / m_targets.size().
render_output_window::render_output_window(
	output_window_configuration const& window_configuration
) :
	m_targets			( get_window_client_size( (HWND__*)window_configuration.hwnd, window_configuration.windowed ) ),
	m_output			( resource_manager::ref().create_render_output( (HWND__*)window_configuration.hwnd, window_configuration.windowed ) ),
	m_window			( (HWND__*)window_configuration.hwnd ),
	m_windowed			( window_configuration.windowed )
{
	// FUNCTION BODY[0x653c70]
	math::uint2 const current_size = get_window_client_size( m_window, m_windowed );

	if ( window_configuration.create_flash_renderer )
	{
		backend::ref().set_render_targets	( &*m_targets.m_family[rt_generic_0].target, 0, 0, 0 );
		backend::ref().flush				( );

		m_flash_renderer = NEW( survarium::flash_renderer )(
			window_configuration.scaleform_render_queue,
			device::ref().d3d_device(),
			device::ref().d3d_context()
		);
		m_flash_renderer->on_reset_device(
			current_size.x,
			current_size.y,
			device::ref().d3d_device(),
			device::ref().d3d_context()
		);
	}
	else
		m_flash_renderer = 0;

	if ( window_configuration.width )
		set_size(
			window_configuration.width,
			window_configuration.height,
			!window_configuration.windowed,
			true
		);
}

void render_output_window::goto_fullscreen( )
{
	// FUNCTION BODY[0x653a60]
	m_output->goto_fullscreen	( );
}

void render_output_window::set_size(
	const u32		width,
	const u32		height,
	const bool		fullscreen,
	bool			force_resize
)
{
	// FUNCTION BODY[0x653b70]
	if ( !width || !height )
		return;

	bool const windowed = !fullscreen;
	if (
		!force_resize &&
		width == m_targets.size().x &&
		height == m_targets.size().y &&
		windowed == m_windowed
	)
		return;

	m_windowed		= windowed;
	m_targets.resize( math::uint2( width, height ), force_resize );
	m_output->set_size( width, height, fullscreen, force_resize );

	if ( m_flash_renderer )
	{
		backend::ref().set_render_targets	( &*m_targets.m_family[rt_generic_0].target, 0, 0, 0 );
		backend::ref().flush				( );
		m_flash_renderer->on_reset_device(
			width,
			height,
			device::ref().d3d_device(),
			device::ref().d3d_context()
		);
	}
}

void render_output_window::resize( bool force_resize )
{
	// FUNCTION BODY[0x653a70]
	if ( !m_windowed )
		return;

	math::uint2 const new_size = get_window_client_size( m_window, m_windowed );
	if ( (new_size.x == 0) || (new_size.y == 0) )
		return;

	if ( !force_resize && (new_size.x == m_targets.size().x) && (new_size.y == m_targets.size().y) )
		return;

	m_targets.resize( new_size, force_resize );
	m_output->resize( force_resize );

	if ( m_flash_renderer )
	{
		backend::ref().set_render_targets	( &*m_targets.m_family[rt_generic_0].target, 0, 0, 0 );
		backend::ref().flush				( );
		m_flash_renderer->on_reset_device(
			new_size.x,
			new_size.y,
			device::ref().d3d_device(),
			device::ref().d3d_context()
		);
	}
}

math::uint2 render_output_window::get_window_client_size( HWND__* const window, bool windowed )
{
	// FUNCTION BODY[0x653a00]
	RECT			rect;
	BOOL const result = windowed ?
		GetClientRect( window, &rect ) :
		GetWindowRect( window, &rect );

	if ( result )
		return		math::uint2(rect.right-rect.left, rect.bottom-rect.top);

	return			math::uint2( 0, 0 );
}

} // namespace render
} // namespace vostok
