////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\effect_lighting_stage_default_materials.h"

namespace vostok {
namespace render {

// STATE[STUB]
bool effect_lighting_stage_default_materials::should_recompile_when_global_changes( vector< fs_new::virtual_path_string > const& in_changed_defines ) const
{
	return false;

	// FUNCTION BODY[0x5ed720]: 2
	// <0>
	// <0x5ed721>|0x001|+0x02f:'21'
	// <0x5ed750>|0x030|-0x003:'21'
	// <0x5ed74d>|0x02d|+0x009:'22'
	// ******
}

// STATE[STUB]
void effect_lighting_stage_default_materials::compile( effect_compiler& compiler, custom_config_value const& custom_config )
{
	// LOCALS
	// shader_configuration 			configuration
	// float4 							specular_intensity_ranges
	// float4 							specular_fresnel_roughness_parameters
	// float4 							solid_color_specular
	// float4 							solid_material_params
	// float 							solid_transparency
	// float4 							specular_intensity_ranges
	// float4 							specular_fresnel_roughness_parameters
	// shader_configuration 			probe_configuration
	// float4 							solid_color_specular
	// float 							solid_transparency
	// ******

	// FUNCTION BODY[0x5ed760]: 245
	// <0>
	// <1>
	// <0x5ed76a>|0x00a|+0x031:'28'
	// <0x5ed79b>|0x03b|+0x01f:'29'
	// <0x5ed7ba>|0x05a|+0x01f:'30'
	// <0x5ed7d9>|0x079|+0x024:'31'
	// <0>
	// <1>
	// <0x5ed7fd>|0x09d|+0x033:'34'
	// <0x5ed830>|0x0d0|+0x031:'35'
	// <0x5ed861>|0x101|+0x033:'36'
	// <0>
	// <1>
	// <0x5ed894>|0x134|+0x031:'39'
	// <0>
	// <0x5ed8c5>|0x165|+0x027:'41'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5ed8ec>|0x18c|+0x02e:'48'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5ed91a>|0x1ba|+0x00c:'55'
	// <0x5ed926>|0x1c6|+0x01f:'56'
	// <0>
	// <1>
	// <0x5ed945>|0x1e5|+0x01d:'59'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5ed962>|0x202|+0x026:'66'
	// <0x5ed988>|0x228|+0x04a:'67'
	// <0>
	// <0x5ed9d2>|0x272|+0x015:'69'
	// <0>
	// <0x5ed9e7>|0x287|+0x015:'71'
	// <0>
	// <0x5ed9fc>|0x29c|+0x00f:'73'
	// <0x5eda0b>|0x2ab|+0x020:'74'
	// <0>
	// <1>
	// <2>
	// <0x5eda2b>|0x2cb|+0x032:'78'
	// <0>
	// <0x5eda5d>|0x2fd|+0x020:'80'
	// <0>
	// <0x5eda7d>|0x31d|+0x010:'82'
	// <0>
	// <0x5eda8d>|0x32d|+0x017:'84'
	// <0x5edaa4>|0x344|+0x01b:'85'
	// <0>
	// <1>
	// <2>
	// <0x5edabf>|0x35f|+0x00f:'89'
	// <0x5edace>|0x36e|+0x017:'90'
	// <0>
	// <0x5edae5>|0x385|+0x015:'92'
	// <0>
	// <0x5edafa>|0x39a|+0x033:'94'
	// <0>
	// <0x5edb2d>|0x3cd|+0x028:'96'
	// <0x5edb55>|0x3f5|+0x097:'97'
	// <0>
	// <1>
	// <0x5edbec>|0x48c|+0x03d:'100'
	// <0>
	// <0x5edc29>|0x4c9|+0x003:'102'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5edc2c>|0x4cc|+0x03c:'109'
	// <0>
	// <0x5edc68>|0x508|+0x022:'111'
	// <0x5edc8a>|0x52a|+0x010:'112'
	// <0>
	// <0x5edc9a>|0x53a|+0x017:'114'
	// <0x5edcb1>|0x551|+0x01b:'115'
	// <0>
	// <1>
	// <0x5edccc>|0x56c|+0x002:'118'
	// <0>
	// <0x5edcce>|0x56e|+0x00e:'120'
	// <0x5edcdc>|0x57c|+0x017:'121'
	// <0>
	// <0x5edcf3>|0x593|+0x00a:'123'
	// <0>
	// <0x5edcfd>|0x59d|+0x022:'125'
	// <0x5edd1f>|0x5bf|+0x010:'126'
	// <0>
	// <0x5edd2f>|0x5cf|+0x017:'128'
	// <0x5edd46>|0x5e6|+0x01b:'129'
	// <0>
	// <1>
	// <0x5edd61>|0x601|+0x002:'132'
	// <0>
	// <0x5edd63>|0x603|+0x00e:'134'
	// <0x5edd71>|0x611|+0x017:'135'
	// <0>
	// <0x5edd88>|0x628|+0x033:'137'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5eddbb>|0x65b|+0x00f:'145'
	// <0x5eddca>|0x66a|+0x020:'146'
	// <0>
	// <0x5eddea>|0x68a|+0x037:'148'
	// <0>
	// <0x5ede21>|0x6c1|+0x033:'150'
	// <0>
	// <1>
	// <0x5ede54>|0x6f4|+0x03d:'153'
	// <0>
	// <0x5ede91>|0x731|+0x016:'155'
	// <0>
	// <1>
	// <0x5edea7>|0x747|+0x00b:'158'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5edeb2>|0x752|+0x004:'164'
	// <0x5edeb6>|0x756|+0x029:'165'
	// <0x5ededf>|0x77f|+0x01c:'166'
	// <0x5edefb>|0x79b|+0x023:'167'
	// <0x5edf1e>|0x7be|+0x023:'168'
	// <0x5edf41>|0x7e1|+0x031:'169'
	// <0x5edf72>|0x812|+0x033:'170'
	// <0x5edfa5>|0x845|+0x031:'171'
	// <0>
	// <0x5edfd6>|0x876|+0x01c:'173'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5edff2>|0x892|+0x015:'179'
	// <0>
	// <1>
	// <2>
	// <0x5ee007>|0x8a7|+0x013:'183'
	// <0x5ee01a>|0x8ba|+0x04d:'184'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5ee067>|0x907|+0x007:'189'
	// <0x5ee06e>|0x90e|+0x020:'190'
	// <0>
	// <1>
	// <0x5ee08e>|0x92e|+0x01d:'193'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5ee0ab>|0x94b|+0x042:'199'
	// <0>
	// <0x5ee0ed>|0x98d|+0x007:'201'
	// <0x5ee0f4>|0x994|+0x020:'202'
	// <0>
	// <1>
	// <2>
	// <0x5ee114>|0x9b4|+0x007:'206'
	// <0x5ee11b>|0x9bb|+0x020:'207'
	// <0>
	// <0x5ee13b>|0x9db|+0x037:'209'
	// <0>
	// <0x5ee172>|0xa12|+0x033:'211'
	// <0>
	// <0x5ee1a5>|0xa45|+0x007:'213'
	// <0x5ee1ac>|0xa4c|+0x015:'214'
	// <0>
	// <1>
	// <2>
	// <0x5ee1c1>|0xa61|+0x020:'218'
	// <0>
	// <0x5ee1e1>|0xa81|+0x022:'220'
	// <0x5ee203>|0xaa3|+0x010:'221'
	// <0>
	// <0x5ee213>|0xab3|+0x017:'223'
	// <0x5ee22a>|0xaca|+0x01b:'224'
	// <0>
	// <1>
	// <0x5ee245>|0xae5|+0x002:'227'
	// <0>
	// <0x5ee247>|0xae7|+0x00e:'229'
	// <0x5ee255>|0xaf5|+0x017:'230'
	// <0>
	// <0x5ee26c>|0xb0c|+0x005:'232'
	// <0>
	// <0x5ee271>|0xb11|+0x022:'234'
	// <0x5ee293>|0xb33|+0x010:'235'
	// <0>
	// <0x5ee2a3>|0xb43|+0x017:'237'
	// <0x5ee2ba>|0xb5a|+0x01b:'238'
	// <0>
	// <1>
	// <0x5ee2d5>|0xb75|+0x002:'241'
	// <0>
	// <0x5ee2d7>|0xb77|+0x00e:'243'
	// <0x5ee2e5>|0xb85|+0x017:'244'
	// <0>
	// <0x5ee2fc>|0xb9c|+0x033:'246'
	// <0>
	// <1>
	// <2>
	// <0x5ee32f>|0xbcf|+0x02a:'250'
	// <0>
	// <0x5ee359>|0xbf9|+0x020:'252'
	// <0>
	// <0x5ee379>|0xc19|+0x010:'254'
	// <0>
	// <0x5ee389>|0xc29|+0x017:'256'
	// <0x5ee3a0>|0xc40|+0x01b:'257'
	// <0>
	// <1>
	// <2>
	// <0x5ee3bb>|0xc5b|+0x033:'261'
	// <0>
	// <0x5ee3ee>|0xc8e|+0x028:'263'
	// <0x5ee416>|0xcb6|+0x099:'264'
	// <0>
	// <1>
	// <0x5ee4af>|0xd4f|+0x035:'267'
	// <0>
	// <0x5ee4e4>|0xd84|+0x00b:'269'
	// <0>
	// ******
}

	// TYPEDEFS
	// typedef
	// 	vostok::render::data_indexer*
	// 	iterator_type;

	// typedef
	// 	vostok::render::effect_compiler::shader_cache_info*
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
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
