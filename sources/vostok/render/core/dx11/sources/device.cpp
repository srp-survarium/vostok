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

static bool is_resolution_already_exists( u32 const& monitor_index, math::int2 const& res )
{
	for ( u32 i = 0; i < 512; ++i )
	{

		if ( g_monitor_resolutions[monitor_index][i] == res )
			return true;
	}
	return false;
}

void device::create_d3d( )
{
	IDXGIFactory* dxgi_factory;
	HRESULT res = CreateDXGIFactory( __uuidof( IDXGIFactory), ( void**)( &dxgi_factory));
	CHECK_RESULT(res);

	m_adapter		= 0;
	m_use_perfhud	= false;

#ifndef	MASTER_GOLD
//	claude@NOTE: MASTER_GOLD compiles this span out, so the shipped image carries no
//	evidence of what it contained - only of how much of the file it took: the PDB line
//	map holds no record on 71..99 while pinning CHECK_RESULT on 70 and the EnumAdapters
//	fallback on 100. The NVPerfHUD search below is the legacy idiom laid out to fit.
	//	Look for the 'NVIDIA PerfHUD' adapter.
	//	If it is present, override the default settings.
	UINT adapter_index = 0;
	while ( dxgi_factory->EnumAdapters( adapter_index, &m_adapter) != DXGI_ERROR_NOT_FOUND)
	{
		m_adapter->GetDesc( &m_adapter_desc);

		if ( !wcscmp( m_adapter_desc.Description, L"NVIDIA PerfHUD"))
		{
			m_use_perfhud	= true;
			break;
		}

		m_adapter->Release();
		m_adapter		= 0;

		++adapter_index;
	}
#endif	//	#ifndef MASTER_GOLD

	if ( !m_adapter)
		dxgi_factory->EnumAdapters( 0, &m_adapter);

	g_num_monitors = 0;
	for ( u32 monitor_index = 0; monitor_index < array_size( g_monitor_resolutions ); ++monitor_index )
		for ( u32 resolution_index = 0; resolution_index < array_size( g_monitor_resolutions[0] ); ++resolution_index )
			g_monitor_resolutions[monitor_index][resolution_index] = math::int2( 0, 0 );

	u32				monitor_index	= 0;
	IDXGIOutput*	output			= 0;

//	claude@NOTE: neither initialiser above keeps a line record of its own - the
//	scheduler sinks both stores into the m_outputs clear below.
	memset( m_outputs, 0, sizeof( m_outputs ) );

	while ( m_adapter->EnumOutputs( monitor_index, &output ) != DXGI_ERROR_NOT_FOUND )
	{
//	claude@NOTE: this store files under the GetDisplayModeList record below.
		m_outputs[monitor_index] = output;

		u32 num_display_modes = 0;

		//	ask for the number of supported modes first

		output->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, 0, &num_display_modes, 0 );

		DXGI_MODE_DESC* const display_modes = ( DXGI_MODE_DESC* )ALLOCA( num_display_modes * sizeof( DXGI_MODE_DESC ) );

		output->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, 0, &num_display_modes, display_modes );

//	claude@NOTE: real_resolution_index is the write cursor into the monitor's row of
//	g_monitor_resolutions - it only advances for a resolution that is not there yet, so
//	it runs behind resolution_index as soon as the mode list repeats a resolution at
//	several refresh rates.
		u32 real_resolution_index = 0;
		LOG_INFO( "monitor %d", monitor_index );
		for ( u32 resolution_index = 0; resolution_index < num_display_modes; ++resolution_index )
		{
			math::int2 const res( display_modes[resolution_index].Width, display_modes[resolution_index].Height );
			if ( is_resolution_already_exists( monitor_index, res ) )
				continue;
			g_monitor_resolutions[monitor_index][real_resolution_index] = res;
			LOG_INFO( "  %dx%d", res.x, res.y );

			++real_resolution_index;

			if ( real_resolution_index >= array_size( g_monitor_resolutions[0] ) )
				break;
		}

		++g_num_monitors;

		DXGI_OUTPUT_DESC output_desc;

		output->GetDesc( &output_desc );
		++monitor_index;
	}

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

	LOG_INFO( "* gpu [vendor:%X]-[device:%X]: %S", Desc.VendorId, Desc.DeviceId, Desc.Description );

	//	Warning: Desc.Description is a wide string
	UINT create_device_flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
	if ( g_debug_render_device.is_set( ) )
		create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;

	D3D_DRIVER_TYPE driver_type = D3D_DRIVER_TYPE_HARDWARE;

	if ( m_use_perfhud )
	{
		LOG_WARNING( "using reference d3d device" );
		driver_type = D3D_DRIVER_TYPE_REFERENCE;
	}

//	claude@NOTE: the shipped line map records nothing on 190..198, 205..207 and
//	222..225, and their content cannot be recovered - the statements that bracket them
//	are pinned by their own records (the perfhud arm on 188, the D3D11CreateDevice
//	closing paren on 219, CHECK_RESULT on 221 through its __LINE__ 0DDh, log_ref_count
//	on 226), so the spans are held open to keep the five baked __LINE__ constants of
//	this function - 0AEh, 0B0h, 0BBh, 0DDh, 0E5h - on 174, 176, 187, 221 and 229.
//	Only their extent is evidence; nothing about their content is.

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

bool device::get_query_data( ID3D11Query* in_query, void* in_out_data, u32 const in_data_size, bool in_wait )
{

	HRESULT result = m_context->GetData( in_query, in_out_data, in_data_size, 0 );

	while ( ( result == S_FALSE ) && in_wait )
	{


		result = m_context->GetData( in_query, in_out_data, in_data_size, 0 );






	}

//	claude@NOTE: this body records statements on 244, 246, 250, 257, 262, 264, 266,
//	271, 273, 274 and 278 only; the spans between them emit nothing in the shipped
//	image and are held open so the recorded statements keep their own lines.
	if ( result == S_OK )

		return true;

	if ( result != S_FALSE )
	{



		if ( result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET ||
			result == DXGI_ERROR_DRIVER_INTERNAL_ERROR )
			m_device_removed = true;

	}


	return false;

}

ID3D11Device* device::d3d_device( ) const
{
	return m_device;
}

IDXGIOutput* device::get_output( u32 const monitor_index ) const
{
	ASSERT( UNKNOWN_EXPRESSION_T( monitor_index < array_size( m_outputs ) ) );
	ASSERT( UNKNOWN_EXPRESSION_T( m_outputs[monitor_index] ) );

	return m_outputs[monitor_index];
}

ID3D11DeviceContext* device::d3d_context( ) const
{
	return m_context;
}

// claude@NOTE: the shipped device.cpp leaves 299..310 free of line records between
// d3d_context (296..298) and get_avaliable_video_memory (312..314). Both globals are
// read by is_resolution_already_exists above and written by create_d3d, and a POD
// array leaves no dynamic initializer to place it by, so this gap is the only
// file-scope room the line map offers them.



int g_num_monitors;
math::int2 g_monitor_resolutions[6][512];

u32 device::get_avaliable_video_memory( ) const
{
	return m_avaliable_video_memory;
}

} // namespace render
} // namespace vostok
