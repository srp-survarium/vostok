#include "pch.h"
#include <vostok/render/core/device.h>
#include <vostok/render/core/dx11/device_caps.h>
#include <vostok/render/core/dx11/sources/com_utils.h>

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )
// claude@NOTE: D3DReflect (res_xs_hw.h) lives in d3dcompiler.lib; the legacy tree
// pulled it in from the retired render_base platform_dx11.cpp.
#pragma comment( lib, "d3dcompiler.lib" )

vostok::command_line::key g_debug_render_device( "debug_dx", "", "render", "create d3d device with D3D_CREATE_DEVICE_DEBUG flag" );

namespace vostok {
namespace render {

device::device( bool is_editor ) :
	m_device					( 0 ),
	m_context					( 0 ),
	m_is_editor					( is_editor ),
	m_device_removed			( false ),
	m_avaliable_video_memory	( 0 )
{
	create( );
}

device::~device( )
{
	destroy( );
}

void device::on_device_removed( )
{
	HRESULT const reason = device::ref( ).d3d_device( )->GetDeviceRemovedReason( );

//	claude@NOTE: every line of this function is load-bearing - the six LOG_ERROR
//	sites below bake __LINE__ (38..43 in the shipped image), do not reflow them.
	if ( reason == DXGI_ERROR_DEVICE_HUNG )					LOG_ERROR( "Device remove reason: %s", "DXGI_ERROR_DEVICE_HUNG" );
	else if ( reason == DXGI_ERROR_DEVICE_REMOVED )			LOG_ERROR( "Device remove reason: %s", "DXGI_ERROR_DEVICE_REMOVED" );
	else if ( reason == DXGI_ERROR_DEVICE_RESET )			LOG_ERROR( "Device remove reason: %s", "DXGI_ERROR_DEVICE_RESET" );
	else if ( reason == DXGI_ERROR_DRIVER_INTERNAL_ERROR )	LOG_ERROR( "Device remove reason: %s", "DXGI_ERROR_DRIVER_INTERNAL_ERROR" );
	else if ( reason == DXGI_ERROR_INVALID_CALL )			LOG_ERROR( "Device remove reason: %s", "DXGI_ERROR_INVALID_CALL" );
	else if ( reason == S_OK )								LOG_ERROR( "Device remove reason: %s", "S_OK" );

	m_device_removed = true;

//	claude@NOTE: lines 47..52 emit no code in the shipped build (its PDB line map puts
//	this function's closing brace on 53) and their content is unrecoverable; the span
//	is kept so the __LINE__ constants above stay correct. Residual vs the target: MSVC
//	spends its inline budget on the FIRST arm's log-temp destructor and out-lines the
//	merged one, where the target inlines the single merged copy at the join.

}

bool is_resolution_already_exists( u32 const& monitor_index, math::int2 const& res )
{
	for ( u32 i = 0; i < 512; ++i )
	{

		if ( g_monitor_resolutions[monitor_index][i] == res )
			return true;
	}
	return false;
}

// claude@NOTE: the shipped create_d3d spans lines 67..159 (0x3aa bytes) and also
// enumerates the outputs of the selected adapter into m_outputs / g_num_monitors /
// g_monitor_resolutions (alloca'd DXGI_MODE_DESC list, LOG_INFO "monitor %d" at
// line 135 and "  %dx%d" at line 142), with the NVPerfHUD search below sitting in
// the compiled-out 71..99 span. This body is the pre-enumeration remnant. Until it
// is reconstructed, is_resolution_already_exists above has no caller and /OPT:REF
// strips it, so it cannot pair - and device::create, destroy_d3d, destroy and
// get_query_data cannot reach their own recorded lines (168..230, 162..165,
// 233..239, 242..280) either.
void device::create_d3d( )
{
	IDXGIFactory * dxgi_factory;
	HRESULT res = CreateDXGIFactory( __uuidof( IDXGIFactory), ( void**)( &dxgi_factory));
	CHECK_RESULT(res);

	m_adapter = 0;
	m_use_perfhud = false;

#ifndef	MASTER_GOLD
	// Look for 'NVIDIA NVPerfHUD' adapter
	// If it is present, override default settings
	UINT i = 0;
	while( dxgi_factory->EnumAdapters( i, &m_adapter) != DXGI_ERROR_NOT_FOUND)
	{
		m_adapter->GetDesc( &m_adapter_desc);
		if( !wcscmp( m_adapter_desc.Description,L"NVIDIA PerfHUD"))
		{
			m_use_perfhud = true;
			break;
		}
		else
		{
			m_adapter->Release();
			m_adapter = 0;
		}
		++i;
	}
#endif	//	MASTER_GOLD

	if ( !m_adapter)
		dxgi_factory->EnumAdapters( 0, &m_adapter);

	dxgi_factory->Release();
}

void device::destroy_d3d( )
{
	log_ref_count	( "m_Adapter", m_adapter);
	safe_release	( m_adapter);
}

void device::create( )
{
	create_d3d( );

	//	Display the name of video board
	DXGI_ADAPTER_DESC Desc;
	CHECK_RESULT( m_adapter->GetDesc( &Desc ) );

	//	Warning: Desc.Description is a wide string
	LOG_INFO( "* gpu [vendor:%X]-[device:%X]: %S", Desc.VendorId, Desc.DeviceId, Desc.Description );

	UINT create_device_flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
	if ( g_debug_render_device.is_set( ) )
		create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;

	D3D_DRIVER_TYPE driver_type = D3D_DRIVER_TYPE_HARDWARE;
	if ( m_use_perfhud )
	{
		LOG_WARNING( "using reference d3d device" );
		driver_type = D3D_DRIVER_TYPE_REFERENCE;
	}

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	HRESULT const result = D3D11CreateDevice(
		m_use_perfhud ? m_adapter : 0,
		driver_type,
		NULL,
		create_device_flags,
		feature_levels,
		array_size( feature_levels ),
		D3D11_SDK_VERSION,
		&m_device,
		&m_feature_level,
		&m_context
	);
	CHECK_RESULT( result );

	log_ref_count( "* create: deviceref:", m_device );

	m_avaliable_video_memory = Desc.DedicatedVideoMemory / ( 1024 * 1024 );
	LOG_INFO( "* texture memory: %d Mb", m_avaliable_video_memory );
}

void device::destroy( )
{
	m_context->ClearState( );

	log_ref_count( "* destroy: device", m_device );
	safe_release( m_device );
	destroy_d3d( );
}

// STATE[STUB]
bool device::get_query_data(
	ID3D11Query* in_query,
	void* in_out_data,
	u32 in_data_size,
	bool in_wait
)
{
	VOSTOK_UNREFERENCED_PARAMETER( in_query );
	VOSTOK_UNREFERENCED_PARAMETER( in_out_data );
	VOSTOK_UNREFERENCED_PARAMETER( in_data_size );
	VOSTOK_UNREFERENCED_PARAMETER( in_wait );

	// FUNCTION BODY[0x565150]
	return false;
}

ID3D11Device* device::d3d_device( ) const
{
	return m_device;
}

IDXGIOutput* device::get_output( u32 monitor_index ) const
{
	return m_outputs[monitor_index];
}

ID3D11DeviceContext* device::d3d_context( ) const
{
	return m_context;
}

// claude@NOTE: the shipped device.cpp has a 13-line hole between d3d_context (296..298)
// and get_avaliable_video_memory (312..314), and both globals are read by
// is_resolution_already_exists above, so they are declared in device.h and defined
// here - the only file-scope room the PDB line map leaves for them.
int g_num_monitors;
math::int2 g_monitor_resolutions[6][512];

u32 device::get_avaliable_video_memory( ) const
{
	return m_avaliable_video_memory;
}

} // namespace render
} // namespace vostok
