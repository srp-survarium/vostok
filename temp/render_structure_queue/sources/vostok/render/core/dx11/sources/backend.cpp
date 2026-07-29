////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\backend.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic initializer for 's_max_triagles_per_dip''( )
{
	// FUNCTION BODY[0x7df2a0]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_debug_enabled_ds_clearing_value_cc''( )
{
	// FUNCTION BODY[0x7df300]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_debug_enabled_rt_clearing_cc''( )
{
	// FUNCTION BODY[0x7df350]
	// ******
}

// STATE[STUB]
 backend::backend( )
{
	// CALL SITE INFO
	// <0x560523> -> unsigned long < unknown >()
	// ******

	// FUNCTION BODY[0x560260]: 35
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
	// <0x560478>|0x218|+0x044:'77'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5604bc>|0x25c|+0x049:'82'
	// <0>
	// <1>
	// <0x560505>|0x2a5|+0x012:'85'
	// <0>
	// <0x560517>|0x2b7|+0x00e:'87'
	// <0x560525>|0x2c5|+0x007:'88'
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
	// ******
}

// STATE[STUB]
 backend::~backend( )
{
	// FUNCTION BODY[0x55fa10]: 4
	// <0x55fa16>|0x006|+0x05e:'103'
	// <0x55fa74>|0x064|-0x044:'103'
	// <0x55fa30>|0x020|+0x04f:'104'
	// <0>
	// <0x55fa7f>|0x06f|+0x01d:'106'
	// ******
}

// STATE[STUB]
void backend::clear_depth_stencil( u32 flags, float z_value, u8 stencil_value )
{
	// CALL SITE INFO
	// <0x55f5e3> -> void < unknown >( ID3D11DepthStencilView*, u32, float, u8 )
	// ******

	// FUNCTION BODY[0x55f5b0]: 8
	// <0x55f5b0>|0x000|+0x009:'111'
	// <0>
	// <1>
	// <2>
	// <0x55f5b9>|0x009|+0x00a:'115'
	// <0>
	// <0x55f5c3>|0x013|+0x022:'117'
	// <0>
	// ******
}

// STATE[STUB]
void backend::clear_render_targets(
	float		r,
	float		g,
	float		b,
	float		a
)
{
	// LOCALS
	// float[4] 						color_elements
	// ******

	// CALL SITE INFO
	// <0x55f596> -> void < unknown >( ID3D11RenderTargetView*, float const* )
	// ******

	// FUNCTION BODY[0x55f530]: 8
	// <0x55f533>|0x003|+0x009:'123'
	// <0>
	// <1>
	// <0x55f53c>|0x00c|+0x039:'126'
	// <0>
	// <1>
	// <0x55f575>|0x045|+0x006:'129'
	// <0x55f57b>|0x04b|+0x028:'130'
	// ******
}

// STATE[STUB]
void backend::clear_render_targets( math::color color )
{
	// LOCALS
	// float[4] 						color_elements
	// ******

	// CALL SITE INFO
	// <0x55f911> -> void < unknown >( ID3D11RenderTargetView*, float const* )
	// ******

	// FUNCTION BODY[0x55f860]: 9
	// <0x55f863>|0x003|+0x00d:'135'
	// <0>
	// <1>
	// <2>
	// <0x55f870>|0x010|+0x080:'139'
	// <0>
	// <1>
	// <0x55f8f0>|0x090|+0x006:'142'
	// <0x55f8f6>|0x096|+0x028:'143'
	// ******
}

// STATE[STUB]
void backend::clear_render_targets(
	math::color		color0,
	math::color		color1,
	math::color		color2,
	math::color		color3
)
{
	// LOCALS
	// float[4] 						color_elements
	// ******

	// CALL SITE INFO
	// <0x55f694> -> void < unknown >( ID3D11RenderTargetView*, float const* )
	// <0x55f726> -> void < unknown >( ID3D11RenderTargetView*, float const* )
	// <0x55f7b8> -> void < unknown >( ID3D11RenderTargetView*, float const* )
	// <0x55f84b> -> void < unknown >( ID3D11RenderTargetView*, float const* )
	// ******

	// FUNCTION BODY[0x55f5f0]: 29
	// <0x55f5f3>|0x003|+0x010:'162'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x55f603>|0x013|+0x015:'168'
	// <0>
	// <0x55f618>|0x028|+0x05c:'170'
	// <0x55f674>|0x084|+0x028:'171'
	// <0>
	// <1>
	// <0x55f69c>|0x0ac|+0x00e:'174'
	// <0>
	// <0x55f6aa>|0x0ba|+0x05c:'176'
	// <0x55f706>|0x116|+0x028:'177'
	// <0>
	// <1>
	// <0x55f72e>|0x13e|+0x00e:'180'
	// <0>
	// <0x55f73c>|0x14c|+0x05c:'182'
	// <0x55f798>|0x1a8|+0x028:'183'
	// <0>
	// <1>
	// <0x55f7c0>|0x1d0|+0x094:'186'
	// <0x55f854>|0x264|-0x085:'186'
	// <0>
	// <0x55f7cf>|0x1df|+0x05c:'188'
	// <0x55f82b>|0x23b|+0x023:'189'
	// <0>
	// <0x55f84e>|0x25e|+0x009:'191'
	// ******
}

// STATE[STUB]
void backend::on_device_destroy( )
{
	// FUNCTION BODY[0x55f510]: 5
	// <0x55f510>|0x000|+0x016:'206'
	// <0x55f526>|0x016|+0x007:'207'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
bool sorted_vector_predicate( shader_constant_host const* first, shared_string const& second )
{
	return false;

	// FUNCTION BODY[0x55f4f0]: 1
	// <0x55f4f0>|0x000|+0x011:'220'
	// ******
}

// STATE[STUB]
shader_constant_host* backend::register_constant_host( shared_string const& name, enum_constant_type type )
{
	return NULL;

	// FUNCTION BODY[0x55f930]: 6
	// <0x55f938>|0x008|+0x01d:'230'
	// <0>
	// <0x55f955>|0x025|+0x00d:'232'
	// <0x55f962>|0x032|-0x002:'233'
	// <0>
	// <0x55f960>|0x030|+0x03c:'235'
	// ******
}

// STATE[STUB]
void backend::update_input_layout( )
{
	// CALL SITE INFO
	// <0x55f9ff> -> void < unknown >( ID3D11InputLayout* )
	// ******

	// FUNCTION BODY[0x55f9b0]: 12
	// <0>
	// <0x55f9b0>|0x000|+0x013:'268'
	// <0>
	// <0x55f9c3>|0x013|+0x009:'270'
	// <0>
	// <1>
	// <0x55f9cc>|0x01c|+0x018:'273'
	// <0>
	// <1>
	// <2>
	// <0x55f9e4>|0x034|+0x01d:'277'
	// <0>
	// ******
}

// STATE[STUB]
void backend::set_render_target( enum_render_target_enum target, render_target const* rt )
{
	// FUNCTION BODY[0x55f4c0]: 7
	// <0x55f4c4>|0x004|+0x007:'283'
	// <0>
	// <0x55f4cb>|0x00b|+0x009:'285'
	// <0>
	// <0x55f4d4>|0x014|+0x007:'287'
	// <0x55f4db>|0x01b|+0x008:'288'
	// <0>
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 's_max_triagles_per_dip''( )
{
	// FUNCTION BODY[0x7f1220]
	// ******
}

// STATE[STUB]
void start_profiling( )
{
	// FUNCTION BODY[0x55f4b0]: 13
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
	// ******
}

// STATE[STUB]
double end_profiling( pcstr scope_name, bool is_print )
{
	return 0.0f;

	// FUNCTION BODY[0x55f4a0]: 22
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
	// <0x55f4a0>|0x000|+0x003:'344'
	// ******
}

// STATE[STUB]
void backend::flush( )
{
	// LOCALS
	// float[4] 						blend_factor
	// ID3D11Buffer*[2] 				buffers
	// u32[2] 							offsets
	// u32[2] 							strides
	// ID3D11Buffer* 					buffer
	// ID3D11Buffer* 					buffer
	// ******

	// CALL SITE INFO
	// <0x55fd97> -> void < unknown >( ID3D11RasterizerState* )
	// <0x55fdc2> -> void < unknown >( ID3D11DepthStencilState*, u32 )
	// <0x55fe14> -> void < unknown >( ID3D11BlendState*, float const*, u32 )
	// <0x55fe4e> -> void < unknown >( ID3D11VertexShader*, ID3D11ClassInstance* const*, u32 )
	// <0x55fe81> -> void < unknown >( ID3D11GeometryShader*, ID3D11ClassInstance* const*, u32 )
	// <0x55feb4> -> void < unknown >( ID3D11PixelShader*, ID3D11ClassInstance* const*, u32 )
	// <0x55ff78> -> void < unknown >( u32, u32, ID3D11Buffer* const*, u32 const*, u32 const* )
	// <0x55ffbc> -> void < unknown >( u32, u32, ID3D11Buffer* const*, u32 const*, u32 const* )
	// <0x55fff4> -> void < unknown >( ID3D11Buffer*, DXGI_FORMAT, u32 )
	// <0x5600e6> -> void < unknown >( D3D_PRIMITIVE_TOPOLOGY )
	// ******

	// FUNCTION BODY[0x55fd70]: 139
	// <0>
	// <1>
	// <2>
	// <0x55fd73>|0x003|+0x00c:'352'
	// <0x55fd7f>|0x00f|+0x01a:'353'
	// <0>
	// <0x55fd99>|0x029|+0x009:'355'
	// <0x55fda2>|0x032|+0x022:'356'
	// <0>
	// <0x55fdc4>|0x054|+0x009:'358'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55fdcd>|0x05d|+0x049:'363'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x55fe16>|0x0a6|+0x007:'370'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x55fe1d>|0x0ad|+0x009:'378'
	// <0>
	// <0x55fe26>|0x0b6|+0x011:'380'
	// <0x55fe37>|0x0c7|+0x019:'381'
	// <0>
	// <1>
	// <0x55fe50>|0x0e0|+0x009:'384'
	// <0>
	// <0x55fe59>|0x0e9|+0x011:'386'
	// <0x55fe6a>|0x0fa|+0x019:'387'
	// <0>
	// <1>
	// <0x55fe83>|0x113|+0x009:'390'
	// <0>
	// <0x55fe8c>|0x11c|+0x011:'392'
	// <0x55fe9d>|0x12d|+0x019:'393'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x55feb6>|0x146|+0x016:'400'
	// <0>
	// <1>
	// <2>
	// <0x55fecc>|0x15c|+0x007:'404'
	// <0>
	// <0x55fed3>|0x163|+0x013:'406'
	// <0x55fee6>|0x176|+0x00d:'407'
	// <0>
	// <1>
	// <0x55fef3>|0x183|+0x00a:'410'
	// <0>
	// <1>
	// <0x55fefd>|0x18d|+0x045:'413'
	// <0>
	// <0x55ff42>|0x1d2|+0x002:'415'
	// <0>
	// <0x55ff44>|0x1d4|+0x007:'417'
	// <0x55ff4b>|0x1db|+0x02f:'418'
	// <0>
	// <0x55ff7a>|0x20a|+0x009:'420'
	// <0>
	// <0x55ff83>|0x213|+0x00d:'422'
	// <0x55ff90>|0x220|+0x02e:'423'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x55ffbe>|0x24e|+0x009:'428'
	// <0>
	// <0x55ffc7>|0x257|+0x011:'430'
	// <0x55ffd8>|0x268|+0x01e:'431'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x55fff6>|0x286|+0x00d:'438'
	// <0x560003>|0x293|+0x00d:'439'
	// <0x560010>|0x2a0|+0x00d:'440'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x56001d>|0x2ad|+0x009:'446'
	// <0x560026>|0x2b6|+0x005:'447'
	// <0>
	// <0x56002b>|0x2bb|+0x009:'449'
	// <0x560034>|0x2c4|+0x00b:'450'
	// <0>
	// <0x56003f>|0x2cf|+0x009:'452'
	// <0x560048>|0x2d8|+0x00b:'453'
	// <0>
	// <0x560053>|0x2e3|+0x009:'455'
	// <0x56005c>|0x2ec|+0x007:'456'
	// <0>
	// <0x560063>|0x2f3|+0x009:'458'
	// <0x56006c>|0x2fc|+0x00b:'459'
	// <0>
	// <0x560077>|0x307|+0x009:'461'
	// <0x560080>|0x310|+0x00b:'462'
	// <0>
	// <0x56008b>|0x31b|+0x009:'464'
	// <0x560094>|0x324|+0x007:'465'
	// <0>
	// <0x56009b>|0x32b|+0x009:'467'
	// <0x5600a4>|0x334|+0x00c:'468'
	// <0>
	// <0x5600b0>|0x340|+0x009:'470'
	// <0x5600b9>|0x349|+0x00b:'471'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5600c4>|0x354|+0x009:'477'
	// <0x5600cd>|0x35d|+0x01b:'478'
	// <0>
	// <0x5600e8>|0x378|+0x006:'480'
	// <0>
	// <0x5600ee>|0x37e|+0x007:'482'
	// <0>
	// <0x5600f5>|0x385|+0x016:'484'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void backend::render_indexed(
	D3D_PRIMITIVE_TOPOLOGY		type,
	u32							index_count,
	u32							start_index,
	u32							base_vertex
)
{
	// CALL SITE INFO
	// <0x560221> -> void < unknown >( u32, u32, int )
	// ******

	// FUNCTION BODY[0x5601b0]: 45
	// <0x5601ba>|0x00a|+0x011:'555'
	// <0x5601cb>|0x01b|+0x004:'556'
	// <0x5601cf>|0x01f|+0x006:'557'
	// <0>
	// <0x5601d5>|0x025|+0x005:'559'
	// <0>
	// <0x5601da>|0x02a|+0x007:'561'
	// <0x5601e1>|0x031|+0x003:'562'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5601e4>|0x034|+0x004:'570'
	// <0x5601e8>|0x038|+0x016:'571'
	// <0>
	// <1>
	// <2>
	// <0x5601fe>|0x04e|+0x006:'575'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x560204>|0x054|+0x01f:'580'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x560223>|0x073|+0x011:'586'
	// <0>
	// <1>
	// <0x560234>|0x084|+0x01f:'589'
	// <0>
	// <1>
	// <2>
	// <0x560253>|0x0a3|-0x00c:'593'
	// <0>
	// <1>
	// <2>
	// <0x560247>|0x097|-0x007:'597'
	// <0>
	// <1>
	// <0x560240>|0x090|+0x00c:'600'
	// <0x56024c>|0x09c|+0x00a:'600'
	// ******
}

// STATE[STUB]
void backend::render( D3D_PRIMITIVE_TOPOLOGY type, u32 vertex_count, u32 base_vertex )
{
	// CALL SITE INFO
	// <0x560177> -> void < unknown >( u32, u32 )
	// ******

	// FUNCTION BODY[0x560130]: 30
	// <0x560139>|0x009|+0x00f:'604'
	// <0x560148>|0x018|+0x004:'605'
	// <0x56014c>|0x01c|+0x006:'606'
	// <0>
	// <0x560152>|0x022|+0x005:'608'
	// <0>
	// <0x560157>|0x027|+0x006:'610'
	// <0x56015d>|0x02d|+0x003:'611'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x560160>|0x030|+0x019:'617'
	// <0>
	// <1>
	// <0x560179>|0x049|+0x011:'620'
	// <0>
	// <1>
	// <0x56018a>|0x05a|+0x01d:'623'
	// <0>
	// <1>
	// <2>
	// <0x5601a7>|0x077|-0x00b:'627'
	// <0>
	// <1>
	// <2>
	// <0x56019c>|0x06c|-0x006:'631'
	// <0>
	// <1>
	// <0x560196>|0x066|+0x00b:'634'
	// <0x5601a1>|0x071|+0x009:'634'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_buffer_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
