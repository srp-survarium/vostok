#include "pch.h"
#include <vostok/render/core/res_render_output.h>
#include <vostok/render/core/res_texture.h>
#include "com_utils.h"
#include <vostok/render/core/resource_manager.h>
#include <vostok/render/core/device.h>
#include <vostok/render/core/options.h>

static pcstr const depth_render_target_texture_name = "$user$depth";

namespace vostok {
namespace render {

res_render_output::res_render_output( HWND window, bool windowed ) :
	m_swap_chain( NULL ),
	m_base_rt( NULL ),
	m_base_zb( NULL ),
	m_window( window ),
	m_present_sync_mode( D3DPRESENT_INTERVAL_DEFAULT ),
	m_valid_previous_present( true ),
	m_windowed( windowed ),
	m_is_registered( false )
{
	ZeroMemory( &m_swap_chain_desc, sizeof( m_swap_chain_desc ) );

	m_window = window; m_windowed = windowed;

	m_swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_swap_chain_desc.BufferCount = 1;
	m_swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	if ( m_window ) select_resolution( m_swap_chain_desc.BufferDesc.Width, m_swap_chain_desc.BufferDesc.Height, m_windowed, m_window ); else GetLastError();

	if ( m_windowed )
	{
		m_swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
		m_swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	}
	else
	{
		m_swap_chain_desc.BufferDesc.RefreshRate = select_refresh(
			m_swap_chain_desc.BufferDesc.Width,
			m_swap_chain_desc.BufferDesc.Height,
			m_swap_chain_desc.BufferDesc.Format
		);
	}

	m_swap_chain_desc.SampleDesc.Count = 1;
	m_swap_chain_desc.SampleDesc.Quality = 0;
	m_swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	m_swap_chain_desc.OutputWindow = m_window;
	m_swap_chain_desc.Windowed = m_windowed;

	if ( !m_windowed )
	{
		// flags stay zero from ZeroMemory; the explicit store is only fullscreen
		m_swap_chain_desc.Flags = 0;
	}

	initialize_swap_chain( NULL );
}

res_render_output::~res_render_output( )
{
	log_ref_count( "m_base_zb", m_base_zb );
	safe_release( m_base_zb );

	log_ref_count( "m_base_rt", m_base_rt );
	safe_release( m_base_rt );
}

void res_render_output::destroy_impl( ) const
{
	resource_manager::ref().release( this );
}

void res_render_output::initialize_swap_chain( IDXGISwapChain* swap_chain )
{
	ASSERT( !m_swap_chain );

	if ( !swap_chain )
	{
		IDXGIDevice* pDXGIDevice;
		HRESULT hr = device::ref().d3d_device()->QueryInterface(
			__uuidof( IDXGIDevice ),
			(void**)&pDXGIDevice
		);
		R_ASSERT( hr == S_OK );

		IDXGIAdapter* pDXGIAdapter;
		hr = pDXGIDevice->GetParent( __uuidof( IDXGIAdapter ), (void**)&pDXGIAdapter );
		R_ASSERT( hr == S_OK );

		IDXGIFactory* dxgi_factory;
		hr = pDXGIAdapter->GetParent( __uuidof( IDXGIFactory ), (void**)&dxgi_factory );
		R_ASSERT( hr == S_OK );

		hr = dxgi_factory->CreateSwapChain( device::ref().d3d_device(), &m_swap_chain_desc, &m_swap_chain );
		R_ASSERT( hr == S_OK );
	}
	else
		m_swap_chain = swap_chain;

	update_targets();
}

void res_render_output::present( )
{
	m_present_sync_mode = options::ref().current.m_vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_DEFAULT;

	HRESULT res = m_swap_chain->Present( m_present_sync_mode, 0 );

	if ( res == DXGI_ERROR_DEVICE_REMOVED || res == DXGI_ERROR_DEVICE_RESET ||
		res == DXGI_ERROR_DRIVER_INTERNAL_ERROR )
	{
		device::ref().on_device_removed();
	}
	else
	{
		CHECK_RESULT( res );
	}
}

DXGI_RATIONAL res_render_output::select_refresh( u32 width, u32 height, DXGI_FORMAT fmt ) const
{
	VOSTOK_UNREFERENCED_PARAMETERS( width, height, fmt );

	DXGI_RATIONAL res;

	res.Numerator = 0;
	res.Denominator = 1;

	return res;
}

void res_render_output::select_resolution( u32& width, u32& height, bool windowed, HWND window ) const
{
	RECT rect;
	if ( windowed ? GetClientRect( window, &rect ) : GetWindowRect( window, &rect ) )
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}
	else
	{
		DWORD error_code = GetLastError();
		VOSTOK_UNREFERENCED_PARAMETER( error_code );
	}
}

bool set_client_rect( HWND h_wnd, s32 pos_x, s32 pos_y, s32 size_x, s32 size_y )
{
	RECT rect = { pos_x, pos_x, pos_x + size_x, pos_y + size_y };


	AdjustWindowRect( &rect, GetWindowLong( h_wnd, GWL_STYLE ), FALSE );
	RECT rect2; GetWindowRect( h_wnd, &rect2 );

	SetWindowLong( h_wnd, GWL_STYLE, GetWindowLong( h_wnd, GWL_STYLE ) | WS_CAPTION | WS_SYSMENU | WS_VISIBLE );
	SetWindowPos( h_wnd, NULL, pos_x, pos_y, size_x, size_y, SWP_SHOWWINDOW );

	return true;
}

void res_render_output::set_size( const u32 in_width, const u32 in_height, const bool in_fullscreen, bool force_resize )
{
	m_windowed = !in_fullscreen;

	RECT desktop_rect;
	GetClientRect( GetDesktopWindow(), &desktop_rect );


	u32 const screen_width = GetSystemMetrics( SM_CXSCREEN );
	u32 const screen_height = GetSystemMetrics( SM_CYSCREEN );

	u32 const pos_x = ( math::max( screen_width, in_width ) - in_width ) / 2;
	u32 const pos_y = ( math::max( screen_height, in_height ) - in_height ) / 2;

	if ( in_fullscreen )
		SetWindowPos( m_window, NULL, 0, 0, in_width, in_height, SWP_SHOWWINDOW );

	resize( !in_fullscreen, in_width, in_height, force_resize );

	if ( !in_fullscreen )
		set_client_rect( m_window, pos_x, pos_y, in_width, in_height );

}

void res_render_output::resize( bool force_resize )
{
	resize( m_windowed, 0, 0, force_resize );
}

void res_render_output::resize( bool windowed, const u32 size_x, const u32 size_y, bool force_resize )
{
	DXGI_MODE_DESC& buffer_desc = m_swap_chain_desc.BufferDesc;

	math::uint2 new_size = math::uint2( size_x, size_y );

	if ( !new_size.x || !new_size.y )
		if ( m_window ) select_resolution( new_size.x, new_size.y, windowed, m_window ); else GetLastError();

	if ( !force_resize && buffer_desc.Width == new_size.x && buffer_desc.Height == new_size.y && m_windowed == windowed )
		return;

	if ( new_size.x < 16 || new_size.y < 16 )
		return;

	m_swap_chain_desc.Windowed = m_windowed = windowed;

	buffer_desc.Width = new_size.x;
	buffer_desc.Height = new_size.y;

	log_ref_count( "refCount:pBaseZB", m_base_zb );
	log_ref_count( "refCount:pBaseRT", m_base_rt );

	safe_release( m_base_rt );
	m_texture_zb->set_hw_texture( NULL );

	CHECK_RESULT(
		m_swap_chain->ResizeBuffers(
			m_swap_chain_desc.BufferCount,
			buffer_desc.Width,
			buffer_desc.Height,
			buffer_desc.Format,
			0 )
	);

	IDXGIOutput* output = m_windowed ?
		NULL : device::ref().get_output( options::ref().current.m_monitor_index );

	if ( m_swap_chain->SetFullscreenState( !m_windowed, output ) != S_OK )
	{
		SetFocus( m_window );

		MSG msg;
		BOOL message_result;
		while ( (message_result = GetMessage( &msg, NULL, 0, 0 )) )
		{
			if ( message_result != -1 )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}

		CHECK_RESULT( m_swap_chain->SetFullscreenState( !m_windowed, output ) );
	}

	update_targets();

	update_window_properties();
	m_present_sync_mode = select_presentation_interval();

}

void res_render_output::goto_fullscreen( )
{
	if ( m_swap_chain && m_window && !m_windowed )
	{
		CHECK_RESULT(
			m_swap_chain->SetFullscreenState( !m_windowed,
				device::ref().get_output( options::ref().current.m_monitor_index ) )
		);
	}
}

void res_render_output::update_targets( )
{
	HRESULT res;


	ID3DTexture2D* buffer;
	res = m_swap_chain->GetBuffer( 0, __uuidof( ID3DTexture2D ), (void**)&buffer );
	CHECK_RESULT( res );

	res = device::ref().d3d_device()->CreateRenderTargetView( buffer, NULL, &m_base_rt );
	buffer->Release();
	CHECK_RESULT( res );

	update_depth_stencil_buffer();
}

void res_render_output::update_depth_stencil_buffer( )
{
	ID3DTexture2D* depth_texture;


	D3D_TEXTURE2D_DESC desc_depth;
	desc_depth.Width = m_swap_chain_desc.BufferDesc.Width;
	desc_depth.Height = m_swap_chain_desc.BufferDesc.Height;
	desc_depth.MipLevels = 1;
	desc_depth.ArraySize = 1;
	desc_depth.Format = DXGI_FORMAT_R24G8_TYPELESS;
	desc_depth.SampleDesc.Count = 1;
	desc_depth.SampleDesc.Quality = 0;
	desc_depth.Usage = D3D_USAGE_DEFAULT;
	desc_depth.BindFlags = D3D_BIND_DEPTH_STENCIL | D3D_BIND_SHADER_RESOURCE;
	desc_depth.CPUAccessFlags = 0;
	desc_depth.MiscFlags = 0;

	HRESULT res = device::ref().d3d_device()->CreateTexture2D( &desc_depth, NULL, &depth_texture );
	CHECK_RESULT( res );

	D3D_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof( descDSV ) );

	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	res = device::ref().d3d_device()->CreateDepthStencilView( depth_texture, &descDSV, &m_base_zb );
	CHECK_RESULT( res );

	static u32 depth_texture_id = 0;
	m_depth_rexture_name.assignf( "%s%d", depth_render_target_texture_name, depth_texture_id++ );

	m_texture_zb = resource_manager::ref().create_texture( m_depth_rexture_name.get_buffer(), 0, 0, false, true, true, u32( -1 ) );
	m_texture_zb->set_hw_texture( depth_texture );

	depth_texture->Release();
}

u32 res_render_output::select_presentation_interval( )
{
	return options::ref().current.m_vsync ?
		D3DPRESENT_INTERVAL_ONE :
		D3DPRESENT_INTERVAL_DEFAULT;

}

void res_render_output::update_window_properties( )
{
}

} // namespace render
} // namespace vostok
