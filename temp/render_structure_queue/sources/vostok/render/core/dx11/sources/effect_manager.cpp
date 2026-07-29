////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\effect_manager.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `vostok::render::effect_manager::effect_manager'::`2'::`dynamic atexit destructor for 'effect_cooker''( )
{
	// FUNCTION BODY[0x7f12e0]
	// ******
}

// STATE[STUB]
void effect_loader::on_effect_ready( resources::queries_result& data )
{
	// FUNCTION BODY[0x77f6a0]: 16
	// <0x77f6a4>|0x004|+0x00d:'28'
	// <0>
	// <0x77f6b1>|0x011|+0x033:'30'
	// <0>
	// <0x77f6e4>|0x044|+0x014:'32'
	// <0x77f6f8>|0x058|+0x070:'33'
	// <0x77f768>|0x0c8|+0x002:'34'
	// <0x77f76a>|0x0ca|+0x008:'35'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x77f772>|0x0d2|+0x018:'43'
	// ******
}

// STATE[STUB]
 effect_manager::effect_manager( )
{
	// STATICS
	// static effect_cook 				effect_cooker = <0x4c271f0>;
	// ******

	// FUNCTION BODY[0x77f110]: 4
	// <0x77f1e0>|0x0d0|+0x03d:'69'
	// <0x77f21d>|0x10d|+0x00a:'70'
	// <0>
	// <0x77f227>|0x117|+0x002:'72'
	// ******
}

// STATE[STUB]
 effect_manager::~effect_manager( )
{
	// CALL SITE INFO
	// <0x77ef56> -> void* < unknown >( u32 )
	// <0x77efc9> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x77ef10]: 13
	// <0x77ef14>|0x004|+0x008:'77'
	// <0x77ef1c>|0x00c|+0x003:'78'
	// <0>
	// <0x77ef1f>|0x00f|+0x011:'80'
	// <0x77ef30>|0x020|+0x05a:'81'
	// <0>
	// <0x77ef8a>|0x07a|+0x006:'83'
	// <0x77ef90>|0x080|+0x006:'84'
	// <0>
	// <0x77ef96>|0x086|+0x00a:'86'
	// <0x77efa0>|0x090|+0x05e:'87'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
res_pass* effect_manager::create_pass( res_pass const& pass )
{
	// LOCALS
	// res_pass* 						new_pass
	// ******

	return NULL;

	// FUNCTION BODY[0x77f790]: 11
	// <0>
	// <1>
	// <2>
	// <0x77f795>|0x005|+0x015:'97'
	// <0x77f7aa>|0x01a|+0x004:'98'
	// <0x77f7ae>|0x01e|+0x009:'99'
	// <0>
	// <0x77f7b7>|0x027|+0x02b:'101'
	// <0>
	// <0x77f7e2>|0x052|-0x031:'103'
	// <0>
	// <0x77f7b1>|0x021|+0x049:'105'
	// ******
}

// STATE[STUB]
void effect_manager::delete_pass( res_pass const* pass )
{
	// FUNCTION BODY[0x77f5a0]: 10
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x77f5ab>|0x00b|+0x043:'118'
	// <0x77f5ee>|0x04e|-0x007:'118'
	// <0x77f5e7>|0x047|+0x0a4:'119'
	// ******
}

// STATE[STUB]
effect_descriptor* effect_manager::get_effect_descriptor_by_name( pcstr name )
{
	return NULL;

	// FUNCTION BODY[0x77ed30]: 9
	// <0x77ed31>|0x001|+0x011:'123'
	// <0>
	// <0x77ed42>|0x012|+0x005:'125'
	// <0x77ed47>|0x017|+0x009:'126'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x77ed50>|0x020|-0x003:'131'
	// <0x77ed4d>|0x01d|+0x005:'132'
	// ******
}

// STATE[STUB]
void effect_manager::add_effect(
	effect_descriptor*			in_descriptor,
	custom_config_ptr const&	in_config,
	res_effect*					in_effect
)
{
	// LOCALS
	// effect_manager::effect_holder_struct holder
	// ******

	// FUNCTION BODY[0x77ee70]: 6
	// <0>
	// <1>
	// <0x77ee73>|0x003|+0x021:'156'
	// <0>
	// <1>
	// <0x77ee94>|0x024|+0x052:'159'
	// ******
}

// STATE[STUB]
void effect_manager::remove_effect( res_effect* in_effect )
{
	// LOCALS
	// effect_manager::effect_holder_struct* end_it
	// ******

	// FUNCTION BODY[0x77ede0]: 11
	// <0>
	// <0x77ede1>|0x001|+0x004:'165'
	// <0x77ede5>|0x005|+0x007:'166'
	// <0>
	// <0x77edec>|0x00c|+0x009:'168'
	// <0>
	// <0x77edf5>|0x015|+0x009:'170'
	// <0>
	// <0x77edfe>|0x01e|+0x068:'172'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void effect_manager::on_effect_created( res_effect_ptr* out_effect_ptr, resources::queries_result& data )
{
	// FUNCTION BODY[0x77f490]: 9
	// <0x77f493>|0x003|+0x035:'223'
	// <0>
	// <1>
	// <0x77f4c8>|0x038|+0x01d:'226'
	// <0>
	// <0x77f4e5>|0x055|+0x073:'228'
	// <0>
	// <1>
	// <0x77f558>|0x0c8|+0x00e:'231'
	// <0x77f566>|0x0d6|-0x009:'231'
	// <0x77f55d>|0x0cd|+0x030:'232'
	// ******
}

// STATE[STUB]
void effect_manager::on_async_effect_created(
	resources::queries_result&		data,
	res_effect_ptr*					out_effect_ptr,
	effect_descriptor*				descriptor
)
{
	// FUNCTION BODY[0x77f390]: 12
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x77f391>|0x001|+0x035:'243'
	// <0>
	// <1>
	// <0x77f3c6>|0x036|+0x019:'246'
	// <0x77f3df>|0x04f|+0x073:'247'
	// <0>
	// <0x77f452>|0x0c2|+0x00c:'249'
	// <0x77f45e>|0x0ce|-0x007:'249'
	// <0x77f457>|0x0c7|+0x02e:'250'
	// ******
}

// STATE[STUB]
void effect_manager::on_async_effect_created_callback(
	resources::queries_result&		data,
	res_effect_ptr*					out_effect_ptr,
	effect_descriptor*				descriptor,
	boost::function< void() >		callback
)
{
	// FUNCTION BODY[0x77f230]: 11
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x77f236>|0x006|+0x026:'262'
	// <0x77f25c>|0x02c|+0x09e:'263'
	// <0x77f2fa>|0x0ca|+0x002:'264'
	// <0x77f2fc>|0x0cc|+0x026:'265'
	// <0>
	// <0x77f322>|0x0f2|+0x031:'267'
	// ******
}

// STATE[STUB]
void effect_manager::create_new_effect(
	res_effect_ptr*				out_effect,
	effect_descriptor*			descriptor,
	custom_config_ptr const&	config,
	u32							crc
)
{
	// LOCALS
	// variant< 32 > 					user_data_variant
	// ******

	// FUNCTION BODY[0x77f9e0]: 14
	// <0>
	// <1>
	// <2>
	// <0x77f9e6>|0x006|+0x054:'299'
	// <0x77fa3a>|0x05a|+0x01e:'300'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x77fa58>|0x078|+0x0c5:'309'
	// ******
}

// STATE[STUB]
res_effect_ptr effect_manager::create_new_effect( effect_descriptor& descriptor, custom_config_ptr const& ptr, u32 crc )
{
	// LOCALS
	// variant< 32 > const*[1] 			user_data_variants
	// variant< 32 > 					user_data_variant
	// resources::creation_request[1] 	requests
	// ******

	// FUNCTION BODY[0x77ff50]: 21
	// <0>
	// <1>
	// <2>
	// <0x77ff56>|0x006|+0x04d:'342'
	// <0x77ffa3>|0x053|+0x020:'343'
	// <0>
	// <0x77ffc3>|0x073|+0x002:'345'
	// <0>
	// <0x77ffc5>|0x075|+0x024:'347'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x77ffe9>|0x099|+0x097:'357'
	// <0>
	// <0x780080>|0x130|+0x012:'359'
	// ******
}

// STATE[STUB]
void effect_manager::on_effects_recompiled(
	vectora< effect_manager::effect_to_recompile_struct >*	effects_to_recompile,
	resources::queries_result&		data
)
{
	// FUNCTION BODY[0x77f900]: 17
	// <0>
	// <0x77f901>|0x001|+0x021:'365'
	// <0>
	// <0x77f922>|0x022|+0x010:'367'
	// <0>
	// <0x77f932>|0x032|+0x042:'369'
	// <0>
	// <0x77f974>|0x074|+0x014:'371'
	// <0x77f988>|0x088|+0x00e:'372'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x77f996>|0x096|+0x02d:'377'
	// <0>
	// <1>
	// <0x77f9c3>|0x0c3|+0x004:'380'
	// ******
}

// STATE[STUB]
res_shader_technique* effect_manager::create_effect_technique( res_shader_technique const& element )
{
	// LOCALS
	// res_shader_technique* 			new_technique
	// ******

	return NULL;

	// FUNCTION BODY[0x77f850]: 20
	// <0x77f858>|0x008|+0x00c:'435'
	// <0x77f864>|0x014|+0x00c:'436'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x77f870>|0x020|+0x015:'442'
	// <0x77f885>|0x035|+0x004:'443'
	// <0x77f889>|0x039|+0x00d:'444'
	// <0>
	// <1>
	// <0x77f896>|0x046|+0x027:'447'
	// <0>
	// <0x77f8bd>|0x06d|+0x003:'449'
	// <0x77f8c0>|0x070|+0x010:'450'
	// <0>
	// <1>
	// <0x77f8d0>|0x080|+0x015:'453'
	// <0x77f8e5>|0x095|-0x07f:'454'
	// <0x77f866>|0x016|+0x026:'455'
	// <0x77f88c>|0x03c|+0x05b:'455'
	// ******
}

// STATE[STUB]
void effect_manager::delete_effect_technique( res_shader_technique const* technique )
{
	// FUNCTION BODY[0x77f810]: 10
	// <0x77f815>|0x005|+0x007:'459'
	// <0>
	// <1>
	// <0x77f81c>|0x00c|+0x012:'462'
	// <0>
	// <0x77f82e>|0x01e|+0x014:'464'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
void effect_manager::recompile_shaders_async( vector< fs_new::virtual_path_string > const& in_changed_defines )
{
	// LOCALS
	// vectora< effect_manager::effect_to_recompile_struct > effects_to_recompile
	// ******

	// CALL SITE INFO
	// <0x77fb7b> -> bool < unknown >( vector< fs_new::virtual_path_string > const& ) const
	// ******

	// FUNCTION BODY[0x77fb40]: 54
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x77fb46>|0x006|+0x02a:'479'
	// <0>
	// <1>
	// <2>
	// <0x77fb70>|0x030|+0x015:'483'
	// <0>
	// <0x77fb85>|0x045|+0x0e8:'485'
	// <0>
	// <1>
	// <2>
	// <0x77fc6d>|0x12d|+0x00e:'489'
	// <0x77fc7b>|0x13b|+0x05c:'490'
	// <0>
	// <0x77fcd7>|0x197|-0x04d:'492'
	// <0x77fc8a>|0x14a|+0x00e:'493'
	// <0x77fc98>|0x158|+0x00e:'494'
	// <0x77fca6>|0x166|+0x012:'495'
	// <0>
	// <1>
	// <0x77fcb8>|0x178|+0x022:'498'
	// <0>
	// <0x77fcda>|0x19a|+0x010:'500'
	// <0x77fcea>|0x1aa|+0x0ba:'501'
	// <0x77fda4>|0x264|+0x00e:'502'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x77fdb2>|0x272|+0x02e:'508'
	// <0x77fde0>|0x2a0|+0x026:'509'
	// <0>
	// <1>
	// <0x77fe06>|0x2c6|+0x003:'512'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x77fe09>|0x2c9|+0x090:'520'
	// <0>
	// <0x77fe99>|0x359|+0x017:'522'
	// <0>
	// <0x77feb0>|0x370|+0x005:'524'
	// <0x77feb5>|0x375|+0x059:'525'
	// <0>
	// ******
}

// STATE[STUB]
void effect_manager::register_effect_desctiptor( pcstr name, effect_descriptor* dectriptor )
{
	// FUNCTION BODY[0x77ed60]: 1
	// <0x77ed66>|0x006|+0x068:'583'
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
	// 	vostok::render::effect_compiler::shader_cache_info*
	// 	iterator_type;

	// typedef
	// 	vostok::render::effect_manager::effect_holder_struct*
	// 	iterator_type;

	// typedef
	// 	vostok::render::effect_manager::effect_to_recompile_struct*
	// 	iterator_type;

	// typedef
	// 	vostok::render::effect_manager::shader_cache_info*
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
