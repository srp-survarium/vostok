////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\effect_compiler.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic initializer for 's_no_effect_result''( )
{
	// FUNCTION BODY[0x7df500]
	// ******
}

// STATE[STUB]
void `dynamic initializer for 's_one_texture_result''( )
{
	// FUNCTION BODY[0x7df520]
	// ******
}

// STATE[STUB]
 effect_compiler::effect_compiler(
	res_effect&								effect,
	resources::query_result_for_cook*		parent,
	bool									shaders_cache_mode,
	associative_vector< binary_shader_key_type, binary_shader_source_ptr, vector, std::less< binary_shader_key_type > >*	in_shader_sources
)
{
	// FUNCTION BODY[0x7a6710]: 1
	// <0>
	// ******
}

// STATE[STUB]
 effect_compiler::~effect_compiler( )
{
	// FUNCTION BODY[0x7a6570]: 1
	// <0>
	// ******
}

// STATE[STUB]
effect_compiler& effect_compiler::set_depth( bool enable, bool write_enable, D3D11_COMPARISON_FUNC cmp_func )
{
	// FUNCTION BODY[0x7a6020]: 6
	// <0x7a6023>|0x003|+0x059:'49'
	// <0x7a607c>|0x05c|-0x022:'50'
	// <0>
	// <0x7a605a>|0x03a|+0x024:'52'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
effect_compiler& effect_compiler::set_stencil(
	s32							enable,
	u32							ref,
	u8							read_mask,
	u8							write_mask,
	D3D11_COMPARISON_FUNC		func,
	D3D11_STENCIL_OP			fail,
	D3D11_STENCIL_OP			pass,
	D3D11_STENCIL_OP			zfail
)
{
	// FUNCTION BODY[0x7a5f80]: 8
	// <0x7a5f81>|0x001|+0x041:'66'
	// <0>
	// <1>
	// <0x7a5fc2>|0x042|+0x016:'69'
	// <0x7a5fd8>|0x058|+0x01f:'70'
	// <0x7a5ff7>|0x077|+0x01e:'71'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
effect_compiler& effect_compiler::set_alpha_blend(
	s32					blend_enable,
	D3D11_BLEND			src_blend,
	D3D11_BLEND			dest_blend,
	D3D11_BLEND_OP		blend_op,
	D3D11_BLEND			src_alpha_blend,
	D3D11_BLEND			dest_alpha_blend,
	D3D11_BLEND_OP		blend_alpha_op
)
{
	// FUNCTION BODY[0x7a5f10]: 14
	// <0x7a5f16>|0x006|+0x038:'95'
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
	// <0x7a5f4e>|0x03e|+0x01f:'106'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
effect_compiler& effect_compiler::set_texture(
	pcstr				name,
	pcstr				physical_name,
	res_texture_ptr*	out_texture,
	bool				streamed,
	u32					num_last_mips_used
)
{
	// LOCALS
	// fs_new::virtual_path_string 		physical_name_lower_case
	// fixed_string< 512 > 				s
	// bool 							res
	// effect_compiler::texture_query_desc desc
	// texture_named_instance 			instance
	// ******

	// FUNCTION BODY[0x7a6c70]: 74
	// <0>
	// <0x7a6c83>|0x013|+0x076:'192'
	// <0x7a6cf9>|0x089|+0x015:'193'
	// <0>
	// <0x7a6d0e>|0x09e|+0x05b:'195'
	// <0>
	// <0x7a6d69>|0x0f9|+0x00e:'197'
	// <0x7a6d77>|0x107|+0x003:'198'
	// <0>
	// <0x7a6d7a>|0x10a|+0x014:'200'
	// <0x7a6d8e>|0x11e|+0x00c:'201'
	// <0>
	// <0x7a6d9a>|0x12a|+0x170:'203'
	// <0>
	// <1>
	// <2>
	// <0x7a6f0a>|0x29a|+0x00f:'207'
	// <0x7a6f19>|0x2a9|+0x048:'208'
	// <0>
	// <1>
	// <2>
	// <0x7a6f61>|0x2f1|+0x040:'212'
	// <0x7a6fa1>|0x331|-0x00f:'212'
	// <0>
	// <0x7a6f92>|0x322|-0x1b2:'214'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x7a6de0>|0x170|+0x014:'222'
	// <0x7a6df4>|0x184|+0x020:'223'
	// <0>
	// <0x7a6e14>|0x1a4|+0x00f:'225'
	// <0x7a6e23>|0x1b3|+0x010:'226'
	// <0>
	// <1>
	// <2>
	// <0x7a6e33>|0x1c3|+0x018:'230'
	// <0>
	// <0x7a6e4b>|0x1db|+0x00e:'232'
	// <0x7a6e59>|0x1e9|+0x004:'233'
	// <0>
	// <1>
	// <0x7a6e5d>|0x1ed|+0x007:'236'
	// <0>
	// <1>
	// <0x7a6e64>|0x1f4|+0x00f:'239'
	// <0>
	// <1>
	// <0x7a6e73>|0x203|+0x020:'242'
	// <0>
	// <0x7a6e93>|0x223|+0x01d:'244'
	// <0>
	// <0x7a6eb0>|0x240|+0x011:'246'
	// <0>
	// <0x7a6ec1>|0x251|+0x021:'248'
	// <0x7a6ee2>|0x272|+0x013:'249'
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
	// <0x7a6ef5>|0x285|-0x08f:'264'
	// <0x7a6e66>|0x1f6|+0x097:'265'
	// <0x7a6efd>|0x28d|+0x097:'265'
	// <0x7a6f94>|0x324|+0x019:'265'
	// ******
}

// STATE[STUB]
effect_compiler& effect_compiler::set_texture(
	pcstr			name,
	res_texture*	texture,
	bool			streamed,
	u32				arg_3
)
{
	// LOCALS
	// u32 								num_last_mips_used
	// ******

	// FUNCTION BODY[0x7a5e60]: 20
	// <0>
	// <1>
	// <0x7a5e63>|0x003|+0x0a1:'271'
	// <0x7a5f04>|0x0a4|-0x054:'272'
	// <0>
	// <1>
	// <2>
	// <0x7a5eb0>|0x050|+0x00f:'276'
	// <0x7a5ebf>|0x05f|+0x00d:'277'
	// <0>
	// <0x7a5ecc>|0x06c|+0x00f:'279'
	// <0x7a5edb>|0x07b|+0x00d:'280'
	// <0>
	// <0x7a5ee8>|0x088|+0x00f:'282'
	// <0x7a5ef7>|0x097|+0x00f:'283'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

// STATE[STUB]
effect_compiler& effect_compiler::set_cull_mode( D3D11_CULL_MODE mode )
{
	// FUNCTION BODY[0x7a5e10]: 5
	// <0x7a5e11>|0x001|+0x044:'315'
	// <0x7a5e55>|0x045|-0x00f:'316'
	// <0>
	// <0x7a5e46>|0x036|+0x011:'318'
	// <0>
	// ******
}

// STATE[STUB]
effect_compiler& effect_compiler::color_write_enable( D3D11_COLOR_WRITE_ENABLE mode )
{
	// FUNCTION BODY[0x7a5dc0]: 5
	// <0x7a5dc3>|0x003|+0x043:'323'
	// <0x7a5e06>|0x046|-0x00c:'324'
	// <0>
	// <0x7a5dfa>|0x03a|+0x00e:'326'
	// <0>
	// ******
}

// STATE[STUB]
effect_compiler& effect_compiler::set_fill_mode( D3D11_FILL_MODE fill_mode )
{
	// FUNCTION BODY[0x7a5d70]: 5
	// <0x7a5d71>|0x001|+0x044:'332'
	// <0x7a5db5>|0x045|-0x00f:'333'
	// <0>
	// <0x7a5da6>|0x036|+0x011:'335'
	// <0>
	// ******
}

// STATE[STUB]
effect_compiler& effect_compiler::bind_constant( shader_constant_binding const& binding )
{
	// FUNCTION BODY[0x7a6090]: 5
	// <0x7a6096>|0x006|+0x03b:'341'
	// <0>
	// <1>
	// <0x7a60d1>|0x041|+0x024:'344'
	// <0>
	// ******
}

// STATE[STUB]
effect_compiler& effect_compiler::begin_pass(
	pcstr							vs_name,
	pcstr							gs_name,
	pcstr							ps_name,
	shader_configuration const&		shader_config,
	shader_include_getter*			include_getter
)
{
	// LOCALS
	// effect_compiler::shader_cache_info info
	// ******

	// FUNCTION BODY[0x7a6100]: 38
	// <0x7a6116>|0x016|+0x2fb:'355'
	// <0>
	// <0x7a6411>|0x311|+0x009:'357'
	// <0x7a641a>|0x31a|+0x03a:'358'
	// <0x7a6454>|0x354|+0x051:'359'
	// <0x7a64a5>|0x3a5|+0x051:'360'
	// <0x7a64f6>|0x3f6|+0x00b:'361'
	// <0x7a6501>|0x401|+0x045:'362'
	// <0x7a6546>|0x446|-0x00f:'362'
	// <0>
	// <0x7a6537>|0x437|-0x3e0:'364'
	// <0>
	// <1>
	// <0x7a6157>|0x057|+0x008:'367'
	// <0x7a615f>|0x05f|+0x02c:'368'
	// <0>
	// <0x7a618b>|0x08b|+0x051:'370'
	// <0x7a61dc>|0x0dc|+0x051:'371'
	// <0x7a622d>|0x12d|+0x051:'372'
	// <0>
	// <0x7a627e>|0x17e|+0x010:'374'
	// <0x7a628e>|0x18e|+0x00b:'375'
	// <0x7a6299>|0x199|+0x002:'376'
	// <0x7a629b>|0x19b|+0x047:'377'
	// <0>
	// <0x7a62e2>|0x1e2|+0x010:'379'
	// <0x7a62f2>|0x1f2|+0x00b:'380'
	// <0x7a62fd>|0x1fd|+0x002:'381'
	// <0x7a62ff>|0x1ff|+0x047:'382'
	// <0>
	// <0x7a6346>|0x246|+0x010:'384'
	// <0x7a6356>|0x256|+0x00b:'385'
	// <0x7a6361>|0x261|+0x002:'386'
	// <0x7a6363>|0x263|+0x047:'387'
	// <0>
	// <0x7a63aa>|0x2aa|+0x04b:'389'
	// <0x7a63f5>|0x2f5|+0x00f:'390'
	// <0>
	// <1>
	// <0x7a6404>|0x304|+0x135:'393'
	// <0x7a6539>|0x439|+0x019:'393'
	// ******
}

// STATE[STUB]
effect_compiler& effect_compiler::end_pass( )
{
	// LOCALS
	// res_state_ptr 					state
	// res_xs< gs_data >_ptr 			gs
	// res_xs< vs_data >_ptr 			vs
	// res_xs< ps_data >_ptr 			ps
	// res_pass_ptr 					pass
	// ******

	// FUNCTION BODY[0x7a68f0]: 32
	// <0x7a68f9>|0x009|+0x042:'397'
	// <0>
	// <1>
	// <0x7a693b>|0x04b|+0x014:'400'
	// <0x7a694f>|0x05f|+0x00e:'401'
	// <0x7a695d>|0x06d|+0x00e:'402'
	// <0>
	// <0x7a696b>|0x07b|+0x01c:'404'
	// <0>
	// <0x7a6987>|0x097|+0x020:'406'
	// <0x7a69a7>|0x0b7|+0x020:'407'
	// <0x7a69c7>|0x0d7|+0x020:'408'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7a69e7>|0x0f7|+0x040:'413'
	// <0>
	// <0x7a6a27>|0x137|+0x03d:'415'
	// <0>
	// <0x7a6a64>|0x174|+0x008:'417'
	// <0>
	// <1>
	// <0x7a6a6c>|0x17c|+0x023:'420'
	// <0>
	// <0x7a6a8f>|0x19f|+0x01c:'422'
	// <0x7a6aab>|0x1bb|+0x01a:'423'
	// <0x7a6ac5>|0x1d5|+0x01a:'424'
	// <0>
	// <0x7a6adf>|0x1ef|+0x006:'426'
	// <0>
	// <0x7a6ae5>|0x1f5|+0x075:'428'
	// ******
}

// STATE[STUB]
effect_compiler& effect_compiler::begin_technique( )
{
	// FUNCTION BODY[0x7a66c0]: 7
	// <0x7a66c1>|0x001|+0x009:'433'
	// <0x7a66ca>|0x00a|+0x002:'434'
	// <0>
	// <0x7a66cc>|0x00c|+0x02c:'436'
	// <0x7a66f8>|0x038|+0x00a:'437'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void effect_compiler::end_technique( )
{
	// LOCALS
	// res_shader_technique_ptr 		se
	// ******

	// FUNCTION BODY[0x7a6b70]: 10
	// <0x7a6b73>|0x003|+0x042:'444'
	// <0>
	// <1>
	// <0x7a6bb5>|0x045|+0x024:'447'
	// <0>
	// <0x7a6bd9>|0x069|+0x03b:'449'
	// <0>
	// <0x7a6c14>|0x0a4|+0x02c:'451'
	// <0>
	// <0x7a6c40>|0x0d0|+0x010:'453'
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
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::data_indexer*
	// 	iterator_type;

	// typedef
	// 	vostok::render::effect_compiler::shader_cache_info*
	// 	iterator_type;

	// typedef
	// 	vostok::render::effect_compiler::texture_query_desc*
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
