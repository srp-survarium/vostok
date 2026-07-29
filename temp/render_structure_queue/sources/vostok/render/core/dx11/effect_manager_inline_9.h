////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\CORE\DX11\EFFECT_MANAGER_INLINE_H_INCLUDED
#define VOSTOK\RENDER\CORE\DX11\EFFECT_MANAGER_INLINE_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline void effect_manager::create_effect<vostok::render::effect_motion_vectors_accumulation>( res_effect_ptr* out_effect, effect_options_descriptor const& desc )
{
	// LOCALS
	// u32 								crc
	// ******

	// STATICS
	// static effect_motion_vectors_accumulation descriptor_object = <0x4c26c64>;
	// ******

	// FUNCTION BODY[0x81ef0]: 16
	// <0x81efc>|0x00c|+0x02c:'37'
	// <0x81f28>|0x038|+0x007:'38'
	// <0x81f2f>|0x03f|+0x008:'39'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x81f37>|0x047|+0x05d:'45'
	// <0x81f94>|0x0a4|+0x002:'46'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x81f96>|0x0a6|+0x038:'51'
	// <0>
	// ******
}

// STATE[STUB]
inline void effect_manager::create_effect<vostok::render::effect_gather_bloom>( res_effect_ptr* out_effect )
{
	// LOCALS
	// effect_options_descriptor 		empty_desc
	// u32 								crc
	// ******

	// STATICS
	// static effect_gather_bloom 		descriptor_object = <0x4c267b0>;
	// ******

	// FUNCTION BODY[0x7fe00]: 15
	// <0x7fe0a>|0x00a|+0x02d:'69'
	// <0>
	// <0x7fe37>|0x037|+0x00c:'71'
	// <0x7fe43>|0x043|+0x013:'72'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x7fe56>|0x056|+0x073:'81'
	// <0x7fec9>|0x0c9|+0x002:'82'
	// <0x7fecb>|0x0cb|+0x035:'83'
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
	// 	vostok::render::material_effects*
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

#endif // #ifndef VOSTOK\RENDER\CORE\DX11\EFFECT_MANAGER_INLINE_H_INCLUDED
