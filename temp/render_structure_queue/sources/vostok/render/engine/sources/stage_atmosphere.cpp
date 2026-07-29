////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\stage_atmosphere.h"

namespace vostok {
namespace render {

// STATE[STUB]
void stage_atmosphere::fill_surfaces( render_target_ptr surf0, render_target_ptr surf1, bool use_base_depth_stencil )
{
	// LOCALS
	// D3D11_VIEWPORT 					view_port
	// D3D11_VIEWPORT 					view_port_saved
	// u32 								offset
	// ******

	// FUNCTION BODY[0x60fb40]: 37
	// <0>
	// <0x60fb40>|0x000|+0x04a:'51'
	// <0x60fb8a>|0x04a|+0x002:'52'
	// <0x60fb8c>|0x04c|+0x064:'53'
	// <0>
	// <0x60fbf0>|0x0b0|+0x007:'55'
	// <0x60fbf7>|0x0b7|+0x012:'56'
	// <0x60fc09>|0x0c9|+0x002:'57'
	// <0x60fc0b>|0x0cb|+0x015:'58'
	// <0>
	// <1>
	// <2>
	// <0x60fc20>|0x0e0|+0x014:'62'
	// <0x60fc34>|0x0f4|+0x015:'63'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x60fc49>|0x109|+0x050:'69'
	// <0>
	// <0x60fc99>|0x159|+0x01e:'71'
	// <0>
	// <1>
	// <2>
	// <0x60fcb7>|0x177|+0x01a:'75'
	// <0x60fcd1>|0x191|+0x05c:'76'
	// <0x60fd2d>|0x1ed|+0x049:'77'
	// <0x60fd76>|0x236|+0x049:'78'
	// <0x60fdbf>|0x27f|+0x036:'79'
	// <0x60fdf5>|0x2b5|+0x03c:'80'
	// <0>
	// <0x60fe31>|0x2f1|+0x00c:'82'
	// <0>
	// <0x60fe3d>|0x2fd|+0x076:'84'
	// <0>
	// <0x60feb3>|0x373|+0x02a:'86'
	// ******
}

// STATE[STUB]
void `vostok::render::effect_manager::create_effect<vostok::render::effect_atmospheric_scattering>'::`2'::`dynamic atexit destructor for 'descriptor_object''( )
{
	// FUNCTION BODY[0x7eede0]: 29
	// <0x611153>|0x043|+0x00b:'95'
	// <0>
	// <0x61115e>|0x04e|+0x059:'97'
	// <0x6111b7>|0x0a7|+0x058:'98'
	// <0x61120f>|0x0ff|+0x058:'99'
	// <0x611267>|0x157|+0x058:'100'
	// <0x6112bf>|0x1af|+0x058:'101'
	// <0x611317>|0x207|+0x058:'102'
	// <0x61136f>|0x25f|+0x058:'103'
	// <0x6113c7>|0x2b7|+0x058:'104'
	// <0x61141f>|0x30f|+0x058:'105'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x611477>|0x367|+0x00d:'111'
	// <0>
	// <1>
	// <0x611484>|0x374|+0x002:'114'
	// <0x611486>|0x376|+0x0ac:'115'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x611532>|0x422|+0x046:'121'
	// <0>
	// <0x611578>|0x468|+0x00b:'123'
	// ******
}

// STATE[STUB]
 stage_atmosphere::~stage_atmosphere( )
{
	// FUNCTION BODY[0x60ff30]: 1
	// <0>
	// ******
}

// STATE[STUB]
bool stage_atmosphere::is_effects_ready( ) const
{
	return false;

	// FUNCTION BODY[0x60fb30]: 1
	// <0x60fb30>|0x000|+0x008:'133'
	// ******
}

// STATE[STUB]
void stage_atmosphere::execute( )
{
	// LOCALS
	// float3 							to_sun_direction
	// float4x4 						proj_matrix
	// float 							sun_int
	// bool 							recalc_rayleigh_scattering
	// const float 						angle_in_rad
	// float3 							L_up
	// float4x4 						rotation_X_translation
	// float4x4 						world_transform
	// u32 								offset
	// float3 							L_right
	// float 							scale
	// float3 							L_dir
	// float3 const* const 				eye_rays
	// ******

	// CALL SITE INFO
	// <0x60ffe3> -> bool < unknown >() const
	// <0x60fff1> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x60ffc0]: 200
	// <0>
	// <1>
	// <0x60ffd2>|0x012|+0x00b:'140'
	// <0>
	// <1>
	// <0x60ffdd>|0x01d|+0x00c:'143'
	// <0>
	// <0x60ffe9>|0x029|+0x012:'145'
	// <0>
	// <1>
	// <2>
	// <0x60fffb>|0x03b|+0x009:'149'
	// <0>
	// <1>
	// <2>
	// <0x610004>|0x044|+0x068:'153'
	// <0>
	// <0x61006c>|0x0ac|+0x00a:'155'
	// <0>
	// <1>
	// <2>
	// <0x610076>|0x0b6|+0x01c:'159'
	// <0>
	// <0x610092>|0x0d2|+0x008:'161'
	// <0>
	// <0x61009a>|0x0da|+0x022:'163'
	// <0>
	// <0x6100bc>|0x0fc|+0x00e:'165'
	// <0x6100ca>|0x10a|+0x005:'166'
	// <0>
	// <0x6100cf>|0x10f|+0x016:'168'
	// <0>
	// <0x6100e5>|0x125|+0x03c:'170'
	// <0>
	// <1>
	// <0x610121>|0x161|+0x007:'173'
	// <0>
	// <1>
	// <2>
	// <0x610128>|0x168|+0x00d:'177'
	// <0>
	// <0x610135>|0x175|+0x010:'179'
	// <0>
	// <0x610145>|0x185|+0x00a:'181'
	// <0x61014f>|0x18f|+0x08b:'182'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x6101da>|0x21a|+0x081:'191'
	// <0x61025b>|0x29b|+0x027:'192'
	// <0>
	// <0x610282>|0x2c2|+0x01b:'194'
	// <0>
	// <1>
	// <0x61029d>|0x2dd|+0x0ba:'197'
	// <0x610357>|0x397|-0x029:'197'
	// <0x61032e>|0x36e|+0x01b:'198'
	// <0>
	// <0x610349>|0x389|+0x015:'200'
	// <0x61035e>|0x39e|+0x008:'200'
	// <0x610366>|0x3a6|+0x01c:'201'
	// <0>
	// <0x610382>|0x3c2|+0x00d:'203'
	// <0x61038f>|0x3cf|+0x078:'204'
	// <0x610407>|0x447|+0x078:'205'
	// <0>
	// <0x61047f>|0x4bf|+0x003:'207'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x610482>|0x4c2|+0x019:'213'
	// <0>
	// <1>
	// <0x61049b>|0x4db|+0x024:'216'
	// <0x6104bf>|0x4ff|+0x014:'217'
	// <0>
	// <0x6104d3>|0x513|+0x008:'219'
	// <0>
	// <1>
	// <0x6104db>|0x51b|+0x01e:'222'
	// <0>
	// <1>
	// <0x6104f9>|0x539|+0x015:'225'
	// <0x61050e>|0x54e|+0x026:'226'
	// <0>
	// <0x610534>|0x574|+0x08b:'228'
	// <0x6105bf>|0x5ff|+0x078:'229'
	// <0>
	// <0x610637>|0x677|+0x00c:'231'
	// <0x610643>|0x683|+0x08e:'232'
	// <0>
	// <0x6106d1>|0x711|+0x008:'234'
	// <0>
	// <1>
	// <2>
	// <0x6106d9>|0x719|+0x022:'238'
	// <0>
	// <1>
	// <0x6106fb>|0x73b|+0x022:'241'
	// <0>
	// <0x61071d>|0x75d|+0x05c:'243'
	// <0x610779>|0x7b9|+0x049:'244'
	// <0x6107c2>|0x802|+0x049:'245'
	// <0x61080b>|0x84b|+0x047:'246'
	// <0>
	// <0x610852>|0x892|+0x00d:'248'
	// <0>
	// <0x61085f>|0x89f|+0x008:'250'
	// <0>
	// <1>
	// <2>
	// <0x610867>|0x8a7|+0x00b:'254'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x610872>|0x8b2|+0x014:'260'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x610886>|0x8c6|+0x046:'265'
	// <0>
	// <1>
	// <0x6108cc>|0x90c|+0x021:'268'
	// <0x6108ed>|0x92d|+0x083:'269'
	// <0x610970>|0x9b0|+0x078:'270'
	// <0x6109e8>|0xa28|+0x005:'271'
	// <0>
	// <0x6109ed>|0xa2d|+0x052:'273'
	// <0x610a3f>|0xa7f|+0x057:'274'
	// <0x610a96>|0xad6|+0x07d:'275'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x610b13>|0xb53|+0x05b:'281'
	// <0>
	// <1>
	// <0x610b6e>|0xbae|+0x006:'284'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x610b74>|0xbb4|+0x013:'289'
	// <0x610b87>|0xbc7|+0x004:'290'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x610b8b>|0xbcb|+0x169:'297'
	// <0>
	// <0x610cf4>|0xd34|+0x010:'299'
	// <0>
	// <0x610d04>|0xd44|+0x02a:'301'
	// <0x610d2e>|0xd6e|+0x07b:'302'
	// <0>
	// <0x610da9>|0xde9|+0x018:'304'
	// <0>
	// <0x610dc1>|0xe01|+0x017:'306'
	// <0>
	// <1>
	// <2>
	// <0x610dd8>|0xe18|+0x013:'310'
	// <0>
	// <0x610deb>|0xe2b|+0x011:'312'
	// <0>
	// <1>
	// <0x610dfc>|0xe3c|+0x00e:'315'
	// <0>
	// <0x610e0a>|0xe4a|+0x00c:'317'
	// <0>
	// <0x610e16>|0xe56|+0x011:'319'
	// <0>
	// <0x610e27>|0xe67|+0x00b:'321'
	// <0x610e32>|0xe72|+0x07d:'322'
	// <0x610eaf>|0xeef|+0x035:'323'
	// <0x610ee4>|0xf24|+0x014:'324'
	// <0x610ef8>|0xf38|+0x074:'325'
	// <0x610f6c>|0xfac|+0x021:'326'
	// <0>
	// <0x610f8d>|0xfcd|+0x00d:'328'
	// <0x610f9a>|0xfda|+0x07d:'329'
	// <0x611017>|0x1057|+0x035:'330'
	// <0x61104c>|0x108c|+0x014:'331'
	// <0x611060>|0x10a0|+0x074:'332'
	// <0x6110d4>|0x1114|+0x021:'333'
	// <0>
	// <1>
	// <2>
	// <0x6110f5>|0x1135|-0x1102:'337'
	// <0x60fff3>|0x033|+0x110c:'338'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	pbyte
	// 	iterator_type;

	// typedef
	// 	pcvoid*
	// 	iterator_type;

	// typedef
	// 	u16*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::geometry_batch*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_pass_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_shader_technique_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_buffer_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
