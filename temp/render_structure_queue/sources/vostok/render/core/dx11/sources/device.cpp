////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\device.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic initializer for 'g_debug_render_device''( )
{
	// FUNCTION BODY[0x7df090]
	// ******
}

// STATE[STUB]
explicit device::device( bool is_editor )
{
	// FUNCTION BODY[0x565eb0]: 1
	// <0x565ed9>|0x029|+0x005:'24'
	// ******
}

// STATE[STUB]
 device::~device( )
{
	// FUNCTION BODY[0x565a90]: 1
	// <0x565a91>|0x001|+0x005:'29'
	// ******
}

// STATE[STUB]
void device::on_device_removed( )
{
	// CALL SITE INFO
	// <0x565602> -> HRESULT < unknown >()
	// ******

	// FUNCTION BODY[0x5655e0]: 19
	// <0x5655e3>|0x003|+0x019:'34'
	// <0>
	// <1>
	// <2>
	// <0x5655fc>|0x01c|+0x0aa:'38'
	// <0x5656a6>|0x0c6|+0x0a6:'39'
	// <0x56574c>|0x16c|+0x0a7:'40'
	// <0x5657f3>|0x213|+0x0a7:'41'
	// <0x56589a>|0x2ba|+0x0a7:'42'
	// <0x565941>|0x361|+0x0ca:'43'
	// <0>
	// <0x565a0b>|0x42b|+0x004:'45'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// ******
}

// STATE[STUB]
bool is_resolution_already_exists( u32 const& monitor_index, math::int2 const& res )
{
	return false;

	// FUNCTION BODY[0x5651c0]: 7
	// <0x5651c0>|0x000|+0x01c:'57'
	// <0x5651dc>|0x01c|-0x00c:'57'
	// <0>
	// <1>
	// <0x5651d0>|0x010|+0x01e:'60'
	// <0x5651ee>|0x02e|-0x005:'61'
	// <0>
	// <0x5651e9>|0x029|+0x003:'63'
	// <0x5651ec>|0x02c|+0x005:'64'
	// ******
}

// STATE[STUB]
void device::create_d3d( )
{
	// LOCALS
	// IDXGIFactory* 					dxgi_factory
	// bool 							do_debug_break
	// u32 								monitor_index
	// u32 								num_display_modes
	// u32 								real_resolution_index
	// IDXGIOutput* 					output
	// DXGI_OUTPUT_DESC 				output_desc
	// u32 								resolution_index
	// ******

	// STATICS
	// static bool 						ignore_always = <0x4c266c0>;
	// ******

	// CALL SITE INFO
	// <0x5652b9> -> HRESULT < unknown >( u32, IDXGIAdapter** )
	// <0x565315> -> HRESULT < unknown >( u32, IDXGIOutput** )
	// <0x565346> -> HRESULT < unknown >( DXGI_FORMAT, u32, u32*, DXGI_MODE_DESC* )
	// <0x565370> -> HRESULT < unknown >( DXGI_FORMAT, u32, u32*, DXGI_MODE_DESC* )
	// <0x565582> -> HRESULT < unknown >( DXGI_OUTPUT_DESC* )
	// <0x5655b5> -> HRESULT < unknown >( u32, IDXGIOutput** )
	// <0x5655cb> -> unsigned long < unknown >()
	// ******

	// FUNCTION BODY[0x565230]: 91
	// <0>
	// <0x56523e>|0x00e|+0x01c:'69'
	// <0x56525a>|0x02a|+0x042:'70'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <0x56529c>|0x06c|-0x05a:'100'
	// <0>
	// <1>
	// <2>
	// <0x565242>|0x012|+0x0a0:'104'
	// <0x5652e2>|0x0b2|-0x012:'104'
	// <0x5652d0>|0x0a0|-0x015:'105'
	// <0x5652bb>|0x08b|+0x01a:'106'
	// <0x5652d5>|0x0a5|+0x012:'106'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5652e7>|0x0b7|+0x030:'113'
	// <0>
	// <0x565317>|0x0e7|+0x019:'115'
	// <0>
	// <1>
	// <0x565330>|0x100|+0x003:'118'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x565333>|0x103|+0x015:'124'
	// <0>
	// <0x565348>|0x118|+0x015:'126'
	// <0>
	// <0x56535d>|0x12d|+0x015:'128'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x565372>|0x142|+0x0be:'135'
	// <0x565430>|0x200|+0x124:'136'
	// <0x565554>|0x324|-0x113:'136'
	// <0>
	// <0x565441>|0x211|+0x008:'138'
	// <0x565449>|0x219|+0x020:'139'
	// <0>
	// <0x565469>|0x239|+0x016:'141'
	// <0x56547f>|0x24f|+0x0c7:'142'
	// <0>
	// <0x565546>|0x316|+0x007:'144'
	// <0>
	// <0x56554d>|0x31d|+0x01f:'146'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x56556c>|0x33c|+0x056:'154'
	// <0>
	// <1>
	// <2>
	// <0x5655c2>|0x392|+0x00b:'158'
	// ******
}

// STATE[STUB]
void device::destroy_d3d( )
{
	// CALL SITE INFO
	// <0x56521c> -> unsigned long < unknown >()
	// ******

	// FUNCTION BODY[0x565200]: 2
	// <0x565201>|0x001|+0x00b:'163'
	// <0x56520c>|0x00c|+0x01d:'164'
	// ******
}

// STATE[STUB]
void device::create( )
{
	// LOCALS
	// DXGI_ADAPTER_DESC 				Desc
	// D3D_FEATURE_LEVEL[3] 			feature_levels
	// bool 							do_debug_break
	// bool 							do_debug_break
	// ******

	// STATICS
	// static bool 						ignore_always = <0x4c266c1>;
	// static bool 						ignore_always = <0x4c266c2>;
	// ******

	// CALL SITE INFO
	// <0x565ae1> -> HRESULT < unknown >( DXGI_ADAPTER_DESC* )
	// <0x565b0c> -> HRESULT < unknown >( DXGI_ADAPTER_DESC* )
	// ******

	// FUNCTION BODY[0x565ab0]: 61
	// <0x565abb>|0x00b|+0x07c:'169'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x565b37>|0x087|+0x007:'174'
	// <0>
	// <0x565b3e>|0x08e|+0x0d6:'176'
	// <0>
	// <1>
	// <2>
	// <0x565c14>|0x164|+0x030:'180'
	// <0x565c44>|0x194|+0x005:'181'
	// <0>
	// <1>
	// <2>
	// <0x565c49>|0x199|+0x00d:'185'
	// <0>
	// <0x565c56>|0x1a6|+0x0c4:'187'
	// <0x565d1a>|0x26a|+0x005:'188'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <9>
	// <10>
	// <11>
	// <12>
	// <13>
	// <14>
	// <15>
	// <16>
	// <17>
	// <18>
	// <19>
	// <20>
	// <21>
	// <22>
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <0x565d1f>|0x26f|+0x058:'219'
	// <0>
	// <0x565d77>|0x2c7|+0x04d:'221'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x565dc4>|0x314|+0x00c:'226'
	// <0>
	// <0x565dd0>|0x320|-0x313:'228'
	// <0x565abd>|0x00d|+0x323:'229'
	// <0x565de0>|0x330|+0x0c3:'229'
	// ******
}

// STATE[STUB]
void device::destroy( )
{
	// CALL SITE INFO
	// <0x565a30> -> void < unknown >()
	// <0x565a55> -> unsigned long < unknown >()
	// <0x565a7c> -> unsigned long < unknown >()
	// ******

	// FUNCTION BODY[0x565a20]: 5
	// <0x565a21>|0x001|+0x011:'234'
	// <0>
	// <0x565a32>|0x012|+0x010:'236'
	// <0x565a42>|0x022|+0x01f:'237'
	// <0x565a61>|0x041|+0x028:'238'
	// ******
}

// STATE[STUB]
bool device::get_query_data(
	ID3D11Query*	in_query,
	void*			in_out_data,
	const u32		in_data_size,
	bool			in_wait
)
{
	// CALL SITE INFO
	// <0x565167> -> HRESULT < unknown >( ID3D11Asynchronous*, void*, u32, u32 )
	// <0x565182> -> HRESULT < unknown >( ID3D11Asynchronous*, void*, u32, u32 )
	// ******

	return false;

	// FUNCTION BODY[0x565150]: 37
	// <0>
	// <0x565155>|0x005|+0x014:'244'
	// <0>
	// <0x565169>|0x019|+0x007:'246'
	// <0>
	// <1>
	// <2>
	// <0x565170>|0x020|+0x014:'250'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x565184>|0x034|+0x005:'257'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x565189>|0x039|+0x004:'262'
	// <0>
	// <0x56518d>|0x03d|+0x006:'264'
	// <0>
	// <0x565193>|0x043|+0x005:'266'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x565198>|0x048|+0x015:'271'
	// <0>
	// <0x5651ad>|0x05d|+0x007:'273'
	// <0x5651b4>|0x064|-0x009:'274'
	// <0>
	// <1>
	// <2>
	// <0x5651ab>|0x05b|-0x01b:'278'
	// <0>
	// <0x565190>|0x040|+0x027:'280'
	// ******
}

// STATE[STUB]
ID3D11Device* device::d3d_device( ) const
{
	return NULL;

	// FUNCTION BODY[0x565140]: 1
	// <0x565140>|0x000|+0x006:'284'
	// ******
}

// STATE[STUB]
IDXGIOutput* device::get_output( const u32 monitor_index ) const
{
	return NULL;

	// FUNCTION BODY[0x565130]: 4
	// <0x565130>|0x000|+0x007:'289'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
ID3D11DeviceContext* device::d3d_context( ) const
{
	return NULL;

	// FUNCTION BODY[0x565120]: 1
	// <0x565120>|0x000|+0x006:'297'
	// ******
}

// STATE[STUB]
u32 device::get_avaliable_video_memory( ) const
{
	return 0;

	// FUNCTION BODY[0x565110]: 1
	// <0x565110>|0x000|+0x006:'313'
	// ******
}

} // namespace render
} // namespace vostok
