#include "pch.h"
#include <vostok/render/core/device.h>
#include <vostok/render/core/dx11/device_caps.h>

vostok::command_line::key g_debug_render_device(
	"debug_dx",
	"",
	"render",
	"create d3d device with D3D_CREATE_DEVICE_DEBUG flag"
);

namespace vostok {
namespace render {

// STATE[STUB]
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
}

// STATE[STUB]
device::~device( )
{
	// FUNCTION BODY[0x565a90]
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

// STATE[STUB]
void device::create_d3d( )
{
	// FUNCTION BODY[0x565230]
}

// STATE[STUB]
void device::destroy_d3d( )
{
	// FUNCTION BODY[0x565200]
}

// STATE[STUB]
void device::create( )
{
	// FUNCTION BODY[0x565ab0]
}

// STATE[STUB]
void device::destroy( )
{
	// FUNCTION BODY[0x565a20]
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
