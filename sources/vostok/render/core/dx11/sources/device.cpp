#include "pch.h"
// claude@NOTE: legacy-harvest disposition: the remaining stubs have no legacy ancestor (legacy device remainder holds only setup_states/get_device_state; the d3d_device/d3d_context inlines were consumed and are already reproduced; on_device_removed/get_query_data/get_output/video-memory are new-in-target) - matcher-phase work.
#include <vostok/render/core/device.h>
#include <vostok/render/core/dx11/device_caps.h>
#include <vostok/render/core/dx11/sources/com_utils.h>

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "dxgi.lib" )
// claude@NOTE: D3DReflect (res_xs_hw.h) lives in d3dcompiler.lib; the legacy
// tree pulled it in from the retired render_base platform_dx11.cpp, so the
// pragma moves next to the other DX import libs.
#pragma comment( lib, "d3dcompiler.lib" )

vostok::command_line::key g_debug_render_device(
	"debug_dx",
	"",
	"render",
	"create d3d device with D3D_CREATE_DEVICE_DEBUG flag"
);

namespace vostok {
namespace render {

device::device( bool is_editor ) :
	m_feature_level( D3D_FEATURE_LEVEL_9_1 ),
	m_adapter( 0 ),
	m_device( 0 ),
	m_context( 0 ),
	m_is_editor( is_editor ),
	m_use_perfhud( false ),
	m_device_removed( false ),
	m_avaliable_video_memory( 0 )
{
	for ( u32 i = 0; i < array_size( m_outputs ); ++i )
		m_outputs[i] = 0;

	// FUNCTION BODY[0x565eb0]
	//	Just check if initialization order is correct
	ASSERT( device::ptr());

	create();

	////	need device to be already created.
	//m_caps.update();
}

device::~device( )
{
	// FUNCTION BODY[0x565a90]
	destroy();
}

// STATE[STUB]
void device::on_device_removed( )
{
	// FUNCTION BODY[0x5655e0]
}

// STATE[STUB]
bool is_resolution_already_exists( u32 const& monitor_index, math::int2 const& res )
{
	VOSTOK_UNREFERENCED_PARAMETER( monitor_index );
	VOSTOK_UNREFERENCED_PARAMETER( res );

	// FUNCTION BODY[0x5651c0]
	return false;
}

void device::create_d3d( )
{
	// FUNCTION BODY[0x565230]
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
	// FUNCTION BODY[0x565200]
	log_ref_count	( "m_Adapter", m_adapter);
	safe_release	( m_adapter);
}

// claude@NOTE: legacy fill_vid_mode_list(this) shim dropped - the target
// replaced it with real monitor/resolution enumeration (m_outputs,
// is_resolution_already_exists) - matcher work from 0x565ab0.
void device::create( )
{
	// FUNCTION BODY[0x565ab0]
	create_d3d();

	// TODO: DX10: Create appropriate initialization
	// General - select adapter and device

	// Display the name of video board
	DXGI_ADAPTER_DESC Desc;
	CHECK_RESULT( m_adapter->GetDesc( &Desc));

	//	Warning: Desc.Description is wide string
	LOG_INFO ( "* GPU [vendor:%X]-[device:%X]: %S", Desc.VendorId, Desc.DeviceId, Desc.Description);

	UINT createDeviceFlags = 0;

#ifdef DEBUG
	if(g_debug_render_device.is_set())
		createDeviceFlags |= D3D_CREATE_DEVICE_DEBUG;
#endif

	HRESULT R;

	D3D_DRIVER_TYPE driver_type = D3D_DRIVER_TYPE_HARDWARE;
	// driver_type = m_caps.bForceGPU_REF ? D3D_DRIVER_TYPE_REFERENCE : D3D_DRIVER_TYPE_HARDWARE;

	if ( m_use_perfhud)
		driver_type =  D3D_DRIVER_TYPE_REFERENCE;


	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	R =  D3D11CreateDevice( 0,//m_adapter,//What wrong with adapter??? We should use another version of DXGI?????
		driver_type,
		NULL,
		createDeviceFlags,
		feature_levels,
		sizeof( feature_levels)/sizeof( feature_levels[0]),
		D3D11_SDK_VERSION,
		&m_device,
		&m_feature_level,
		&m_context);

	if ( FAILED( R))
	{
		// Fatal error! Cannot create rendering device AT STARTUP !!!
		LOG_INFO	( 	"Failed to initialize graphics hardware.\n"
						"Please try to restart the game.\n"
						"CreateDevice returned 0x%08x", R);

		//MessageBox			( NULL,"Failed to initialize graphics hardware.\nPlease try to restart the game.","Error!",MB_OK|MB_ICONERROR);
		TerminateProcess	( GetCurrentProcess(),0);
	};
	CHECK_RESULT( R);

	UINT out_sample_quality;

	for (u32 i = 0; i < 32; i++)
	{
		m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R16G16B16A16_FLOAT, i, &out_sample_quality);
	}
	for (u32 i = 0; i < 32; i++)
	{
		m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, i, &out_sample_quality);
	}
	log_ref_count	( "* CREATE: DeviceREF:",m_device);

	size_t	memory		= Desc.DedicatedVideoMemory;
	LOG_INFO			( "*     Texture memory: %d M",		memory/( 1024*1024));
}

void device::destroy( )
{
	// FUNCTION BODY[0x565a20]
	// moved to backend destructor
	//backend::ref().on_device_destroy();

	m_context->ClearState();

	log_ref_count( "* DESTROY: Device", m_device);
	safe_release( m_device);

	destroy_d3d();
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

// STATE[STUB]
ID3D11Device* device::d3d_device( ) const
{
	// FUNCTION BODY[0x565140]
	return m_device;
}

// STATE[STUB]
IDXGIOutput* device::get_output( u32 monitor_index ) const
{
	// FUNCTION BODY[0x565130]
	return m_outputs[monitor_index];
}

// STATE[STUB]
ID3D11DeviceContext* device::d3d_context( ) const
{
	// FUNCTION BODY[0x565120]
	return m_context;
}

// STATE[STUB]
u32 device::get_avaliable_video_memory( ) const
{
	// FUNCTION BODY[0x565110]
	return m_avaliable_video_memory;
}

} // namespace render
} // namespace vostok
