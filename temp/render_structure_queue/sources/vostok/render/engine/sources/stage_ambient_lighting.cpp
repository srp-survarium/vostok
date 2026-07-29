////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\stage_ambient_lighting.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic atexit destructor for 's_debug_sky_light_tech''( )
{
	// FUNCTION BODY[0x7ee620]
	// ******
}

// STATE[STUB]
 stage_ambient_lighting::stage_ambient_lighting( renderer* in_renderer, renderer_context* context )
{
	// LOCALS
	// D3D11_INPUT_ELEMENT_DESC[1] 		desc
	// ******

	// FUNCTION BODY[0x62a800]: 68
	// <0x62a843>|0x043|+0x04b:'58'
	// <0x62a88e>|0x08e|+0x00e:'59'
	// <0x62a89c>|0x09c|+0x00e:'60'
	// <0x62a8aa>|0x0aa|+0x00e:'61'
	// <0>
	// <0x62a8b8>|0x0b8|+0x00d:'63'
	// <0x62a8c5>|0x0c5|+0x00e:'64'
	// <0x62a8d3>|0x0d3|+0x00e:'65'
	// <0x62a8e1>|0x0e1|+0x00e:'66'
	// <0x62a8ef>|0x0ef|+0x00e:'67'
	// <0x62a8fd>|0x0fd|+0x00e:'68'
	// <0x62a90b>|0x10b|+0x00e:'69'
	// <0x62a919>|0x119|+0x00e:'70'
	// <0>
	// <0x62a927>|0x127|+0x00e:'72'
	// <0>
	// <0x62a935>|0x135|+0x00e:'74'
	// <0>
	// <0x62a943>|0x143|+0x058:'76'
	// <0x62a99b>|0x19b|+0x055:'77'
	// <0x62a9f0>|0x1f0|+0x056:'78'
	// <0x62aa46>|0x246|+0x055:'79'
	// <0x62aa9b>|0x29b|+0x055:'80'
	// <0x62aaf0>|0x2f0|+0x055:'81'
	// <0x62ab45>|0x345|+0x055:'82'
	// <0x62ab9a>|0x39a|+0x055:'83'
	// <0x62abef>|0x3ef|+0x055:'84'
	// <0x62ac44>|0x444|+0x055:'85'
	// <0x62ac99>|0x499|+0x055:'86'
	// <0>
	// <0x62acee>|0x4ee|+0x055:'88'
	// <0x62ad43>|0x543|+0x058:'89'
	// <0x62ad9b>|0x59b|+0x058:'90'
	// <0x62adf3>|0x5f3|+0x058:'91'
	// <0>
	// <0x62ae4b>|0x64b|+0x043:'93'
	// <0x62ae8e>|0x68e|+0x044:'94'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x62aed2>|0x6d2|+0x068:'100'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x62af3a>|0x73a|+0x04e:'109'
	// <0>
	// <1>
	// <0x62af88>|0x788|+0x00c:'112'
	// <0>
	// <1>
	// <0x62af94>|0x794|+0x00d:'115'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x62afa1>|0x7a1|+0x050:'123'
	// <0>
	// <0x62aff1>|0x7f1|+0x04a:'125'
	// ******
}

// STATE[STUB]
bool stage_ambient_lighting::is_effects_ready( ) const
{
	return false;

	// FUNCTION BODY[0x629a20]: 13
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
	// <0x629a20>|0x000|+0x054:'142'
	// <0x629a74>|0x054|-0x001:'142'
	// <0x629a73>|0x053|+0x003:'143'
	// ******
}

// STATE[STUB]
void stage_ambient_lighting::execute( )
{
	// LOCALS
	// float3 const* const 				eye_rays
	// environment_probe** 				it
	// float3 							plus_x
	// float3 							minus_z
	// float3 							plus_y
	// light* 							sun
	// float3 							minus_x
	// float3 							minus_y
	// float3 							plus_z
	// const float 						skylight_upper_limit
	// const float 						skylight_lower_limit
	// const float 						skylight_power
	// ambient_volume** 				it_av
	// const float 						radius
	// float4x4 						world_transform
	// environment_probe* 				probe
	// u32 								tech_index
	// float4x4 						world_to_probe
	// ******

	// TYPEDEFS
	// typedef
	// 	stage_ambient_lighting::execute::__l5::sort_by_size_predicate
	// 	stage_ambient_lighting::execute::__l5::sort_by_size_predicate;

	// ******

	// CALL SITE INFO
	// <0x629a98> -> bool < unknown >() const
	// <0x62a7ea> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x629a80]: 260
	// <0>
	// <1>
	// <0x629a92>|0x012|+0xd50:'160'
	// <0>
	// <0x62a7e2>|0xd62|-0xd31:'162'
	// <0>
	// <1>
	// <2>
	// <0x629ab1>|0x031|+0x009:'166'
	// <0>
	// <0x629aba>|0x03a|+0x00a:'168'
	// <0x629ac4>|0x044|+0x006:'169'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x629aca>|0x04a|+0x017:'177'
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
	// <0x629ae1>|0x061|+0x078:'189'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x629b59>|0x0d9|+0x044:'194'
	// <0x629b9d>|0x11d|+0x080:'195'
	// <0x629c1d>|0x19d|+0x079:'196'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x629c96>|0x216|+0x025:'206'
	// <0>
	// <0x629cbb>|0x23b|+0x00e:'208'
	// <0x629cc9>|0x249|+0x006:'209'
	// <0x629ccf>|0x24f|+0x006:'210'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x629cd5>|0x255|+0x101:'215'
	// <0x629dd6>|0x356|+0x085:'216'
	// <0x629e5b>|0x3db|+0x07c:'217'
	// <0x629ed7>|0x457|+0x07c:'218'
	// <0x629f53>|0x4d3|+0x09d:'219'
	// <0x629ff0>|0x570|+0x09a:'220'
	// <0x62a08a>|0x60a|+0x0da:'221'
	// <0>
	// <0x62a164>|0x6e4|+0x01c:'223'
	// <0x62a180>|0x700|+0x088:'224'
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
	// <30>
	// <31>
	// <32>
	// <33>
	// <34>
	// <35>
	// <36>
	// <37>
	// <38>
	// <39>
	// <40>
	// <41>
	// <42>
	// <43>
	// <44>
	// <45>
	// <46>
	// <47>
	// <48>
	// <49>
	// <50>
	// <51>
	// <52>
	// <53>
	// <54>
	// <55>
	// <56>
	// <57>
	// <58>
	// <59>
	// <60>
	// <61>
	// <62>
	// <63>
	// <64>
	// <65>
	// <66>
	// <67>
	// <68>
	// <0x62a208>|0x788|+0x009:'294'
	// <0x62a211>|0x791|+0x00b:'295'
	// <0x62a21c>|0x79c|+0x007:'296'
	// <0>
	// <0x62a223>|0x7a3|+0x011:'298'
	// <0>
	// <0x62a234>|0x7b4|+0x043:'300'
	// <0x62a277>|0x7f7|+0x012:'301'
	// <0>
	// <0x62a289>|0x809|+0x0e7:'303'
	// <0x62a370>|0x8f0|-0x0d0:'303'
	// <0>
	// <1>
	// <2>
	// <0x62a2a0>|0x820|+0x029:'307'
	// <0>
	// <1>
	// <0x62a2c9>|0x849|+0x00c:'310'
	// <0>
	// <1>
	// <0x62a2d5>|0x855|+0x00a:'313'
	// <0x62a2df>|0x85f|+0x00b:'314'
	// <0>
	// <1>
	// <0x62a2ea>|0x86a|+0x016:'317'
	// <0>
	// <1>
	// <0x62a300>|0x880|+0x010:'320'
	// <0>
	// <0x62a310>|0x890|+0x021:'322'
	// <0x62a331>|0x8b1|+0x00b:'323'
	// <0>
	// <0x62a33c>|0x8bc|+0x01c:'325'
	// <0>
	// <1>
	// <0x62a358>|0x8d8|+0x02d:'328'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x62a385>|0x905|+0x026:'334'
	// <0>
	// <0x62a3ab>|0x92b|+0x081:'336'
	// <0x62a42c>|0x9ac|+0x012:'337'
	// <0>
	// <0x62a43e>|0x9be|+0x35f:'339'
	// <0x62a79d>|0xd1d|-0x348:'339'
	// <0>
	// <0x62a455>|0x9d5|+0x004:'341'
	// <0>
	// <0x62a459>|0x9d9|+0x040:'343'
	// <0>
	// <1>
	// <0x62a499>|0xa19|+0x002:'346'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x62a49b>|0xa1b|+0x099:'351'
	// <0>
	// <0x62a534>|0xab4|+0x01e:'353'
	// <0>
	// <1>
	// <0x62a552>|0xad2|+0x00a:'356'
	// <0x62a55c>|0xadc|+0x009:'357'
	// <0>
	// <0x62a565>|0xae5|+0x00b:'359'
	// <0x62a570>|0xaf0|+0x005:'360'
	// <0>
	// <0x62a575>|0xaf5|+0x002:'362'
	// <0>
	// <0x62a577>|0xaf7|+0x00b:'364'
	// <0>
	// <1>
	// <0x62a582>|0xb02|+0x016:'367'
	// <0>
	// <1>
	// <0x62a598>|0xb18|+0x007:'370'
	// <0x62a59f>|0xb1f|+0x007:'371'
	// <0x62a5a6>|0xb26|+0x01a:'372'
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
	// <0x62a5c0>|0xb40|+0x140:'392'
	// <0x62a700>|0xc80|+0x009:'393'
	// <0>
	// <0x62a709>|0xc89|+0x00b:'395'
	// <0x62a714>|0xc94|+0x005:'396'
	// <0>
	// <0x62a719>|0xc99|+0x002:'398'
	// <0>
	// <0x62a71b>|0xc9b|+0x004:'400'
	// <0x62a71f>|0xc9f|+0x021:'401'
	// <0>
	// <0x62a740>|0xcc0|+0x02e:'403'
	// <0>
	// <0x62a76e>|0xcee|+0x00b:'405'
	// <0>
	// <1>
	// <0x62a779>|0xcf9|+0x039:'408'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x62a7b2>|0xd32|+0x00d:'416'
	// <0x62a7bf>|0xd3f|+0x01b:'417'
	// <0x62a7da>|0xd5a|+0x012:'418'
	// ******
}

// STATE[STUB]
bool `vostok::render::stage_ambient_lighting::execute'::`5'::sort_by_size_predicate::operator()(
	environment_probe const*	arg_0 /* vostok::render::environment_probe const* left */,
	environment_probe const*	arg_1 /* vostok::render::environment_probe const* right */
) const
{
	return false;

	// FUNCTION BODY[0x629a00]: 1
	// <0x629a00>|0x000|+0x017:'173'
	// <0x629a17>|0x017|-0x001:'173'
	// <0x629a16>|0x016|+0x003:'174'
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
	// 	vostok::render::effect_compiler::shader_cache_info*
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
