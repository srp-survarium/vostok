////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\stage_pre_rain.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic initializer for 's_rain_debug0_cc''( )
{
	// FUNCTION BODY[0x7d6640]
	// ******
}

// STATE[STUB]
void `dynamic atexit destructor for 's_rain_debug0_cc''( )
{
	// FUNCTION BODY[0x7ee4d0]
	// ******
}

// STATE[STUB]
 stage_pre_rain::stage_pre_rain( renderer* in_renderer, renderer_context* context )
{
	// FUNCTION BODY[0x62c190]: 19
	// <0x62c1c2>|0x032|+0x059:'58'
	// <0x62c21b>|0x08b|+0x058:'59'
	// <0x62c273>|0x0e3|+0x058:'60'
	// <0x62c2cb>|0x13b|+0x058:'61'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x62c323>|0x193|+0x04b:'71'
	// <0>
	// <0x62c36e>|0x1de|+0x039:'73'
	// <0>
	// <0x62c3a7>|0x217|+0x00e:'75'
	// <0x62c3b5>|0x225|+0x00e:'76'
	// ******
}

// STATE[STUB]
bool stage_pre_rain::is_effects_ready( ) const
{
	return false;

	// FUNCTION BODY[0x62b0d0]: 1
	// <0x62b0d0>|0x000|+0x012:'81'
	// <0x62b0e2>|0x012|-0x001:'81'
	// <0x62b0e1>|0x011|+0x003:'82'
	// ******
}

// STATE[STUB]
 stage_pre_rain::~stage_pre_rain( )
{
	// FUNCTION BODY[0x62b660]: 1
	// <0>
	// ******
}

// STATE[STUB]
float3 stage_pre_rain::compute_aligment( float3 const& lightXZshift, float4x4 const& light_space_transform, float smap_res )
{
	// LOCALS
	// float4x4 						light_space_transform_invert
	// float3 							align_origin
	// float4x4 						viewport_invert
	// float4x4 						viewport
	// float4 							origin_pixel
	// ******

	// STATICS
	// static float 					sign_test = <0x10000>;
	// ******

	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY[0x62b0f0]: 31
	// <0>
	// <0x62b0fc>|0x00c|+0x00c:'92'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x62b108>|0x018|+0x115:'97'
	// <0>
	// <0x62b21d>|0x12d|+0x030:'99'
	// <0x62b24d>|0x15d|+0x098:'100'
	// <0>
	// <0x62b2e5>|0x1f5|+0x080:'102'
	// <0>
	// <0x62b365>|0x275|+0x0c1:'104'
	// <0>
	// <1>
	// <2>
	// <0x62b426>|0x336|+0x0a9:'108'
	// <0x62b4cf>|0x3df|+0x032:'109'
	// <0x62b501>|0x411|+0x008:'110'
	// <0x62b509>|0x419|+0x008:'111'
	// <0>
	// <1>
	// <0x62b511>|0x421|+0x08b:'114'
	// <0x62b59c>|0x4ac|+0x098:'115'
	// <0>
	// <0x62b634>|0x544|+0x017:'117'
	// <0>
	// <1>
	// <2>
	// <0x62b64b>|0x55b|+0x002:'121'
	// ******
}

// STATE[STUB]
float4x4 stage_pre_rain::render_rain_shadow_map( )
{
	// LOCALS
	// D3D11_VIEWPORT 					tmp_viewport
	// float4x4 						shadow_projection_transform
	// float4x4 						rotation_matrix
	// D3D11_VIEWPORT 					orig_viewport
	// render_surface_instance** 		it_d
	// float4x4 						texture_space
	// float3 							adjastment
	// vector< render_surface_instance* > m_caster_model
	// render_surface_instance* const* 	end_d
	// float3 							direction
	// float3 							position
	// float4x4 						shadow_view_transform
	// float3 							view_dir
	// float4x4 						shadow_full_transform
	// ******

	return vostok::math::float4x4();

	// FUNCTION BODY[0x62b6f0]: 102
	// <0>
	// <0x62b6fe>|0x00e|+0x00c:'129'
	// <0>
	// <1>
	// <0x62b70a>|0x01a|+0x03f:'132'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x62b749>|0x059|+0x0ea:'138'
	// <0x62b833>|0x143|+0x056:'139'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x62b889>|0x199|+0x04b:'146'
	// <0>
	// <0x62b8d4>|0x1e4|+0x048:'148'
	// <0>
	// <0x62b91c>|0x22c|+0x07c:'150'
	// <0>
	// <1>
	// <2>
	// <0x62b998>|0x2a8|+0x02f:'154'
	// <0x62b9c7>|0x2d7|+0x031:'155'
	// <0>
	// <0x62b9f8>|0x308|+0x059:'157'
	// <0x62ba51>|0x361|+0x023:'158'
	// <0x62ba74>|0x384|+0x02e:'159'
	// <0>
	// <1>
	// <0x62baa2>|0x3b2|+0x02b:'162'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x62bacd>|0x3dd|+0x01f:'167'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x62baec>|0x3fc|+0x040:'172'
	// <0>
	// <1>
	// <2>
	// <0x62bb2c>|0x43c|+0x030:'176'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x62bb5c>|0x46c|+0x01d:'183'
	// <0x62bb79>|0x489|+0x008:'184'
	// <0>
	// <0x62bb81>|0x491|+0x002:'186'
	// <0x62bb83>|0x493|+0x002:'187'
	// <0>
	// <1>
	// <2>
	// <0x62bb85>|0x495|+0x01a:'191'
	// <0>
	// <1>
	// <0x62bb9f>|0x4af|+0x02b:'194'
	// <0>
	// <1>
	// <0x62bbca>|0x4da|+0x021:'197'
	// <0>
	// <0x62bbeb>|0x4fb|+0x00a:'199'
	// <0x62bbf5>|0x505|+0x008:'200'
	// <0x62bbfd>|0x50d|+0x00c:'201'
	// <0x62bc09>|0x519|+0x097:'202'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x62bca0>|0x5b0|+0x003:'207'
	// <0>
	// <0x62bca3>|0x5b3|+0x02f:'209'
	// <0x62bcd2>|0x5e2|+0x013:'210'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x62bce5>|0x5f5|+0x01e:'220'
	// <0>
	// <0x62bd03>|0x613|+0x121:'222'
	// <0>
	// <0x62be24>|0x734|+0x024:'224'
	// <0x62be48>|0x758|+0x017:'225'
	// <0x62be5f>|0x76f|+0x00d:'226'
	// <0x62be6c>|0x77c|+0x01b:'227'
	// <0>
	// <0x62be87>|0x797|+0x01c:'229'
	// ******
}

// STATE[STUB]
void stage_pre_rain::execute( )
{
	// LOCALS
	// vector< render_surface_instance* > m_caster_model
	// float4x4 						view_to_shadow
	// ******

	// CALL SITE INFO
	// <0x62beee> -> bool < unknown >() const
	// <0x62c17d> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x62beb0]: 89
	// <0>
	// <1>
	// <0x62bec1>|0x011|+0x015:'236'
	// <0>
	// <1>
	// <0x62bed6>|0x026|+0x29f:'239'
	// <0>
	// <0x62c175>|0x2c5|-0x267:'241'
	// <0>
	// <1>
	// <2>
	// <0x62bf0e>|0x05e|+0x01a:'245'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x62bf28>|0x078|+0x008:'253'
	// <0>
	// <0x62bf30>|0x080|+0x04d:'255'
	// <0>
	// <0x62bf7d>|0x0cd|+0x017:'257'
	// <0x62bf94>|0x0e4|+0x00b:'258'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x62bf9f>|0x0ef|+0x00c:'263'
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
	// <0x62bfab>|0x0fb|+0x00b:'280'
	// <0>
	// <0x62bfb6>|0x106|+0x00d:'282'
	// <0>
	// <0x62bfc3>|0x113|+0x00d:'284'
	// <0x62bfd0>|0x120|+0x03d:'285'
	// <0x62c00d>|0x15d|+0x05b:'286'
	// <0x62c068>|0x1b8|+0x00b:'287'
	// <0>
	// <0x62c073>|0x1c3|+0x00a:'289'
	// <0x62c07d>|0x1cd|+0x003:'290'
	// <0x62c080>|0x1d0|+0x014:'291'
	// <0x62c094>|0x1e4|+0x01b:'292'
	// <0x62c0af>|0x1ff|+0x00c:'293'
	// <0x62c0bb>|0x20b|+0x017:'294'
	// <0>
	// <0x62c0d2>|0x222|+0x05a:'296'
	// <0>
	// <1>
	// <0x62c12c>|0x27c|+0x011:'299'
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
	// <0x62c13d>|0x28d|+0x014:'320'
	// <0x62c151>|0x2a1|+0x00d:'321'
	// <0x62c15e>|0x2ae|+0x007:'322'
	// <0x62c165>|0x2b5|+0x01a:'323'
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
