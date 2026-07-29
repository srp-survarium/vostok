////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\CORE\DX11\CUSTOM_CONFIG_VALUE_INLINE_H_INCLUDED
#define VOSTOK\RENDER\CORE\DX11\CUSTOM_CONFIG_VALUE_INLINE_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline float3 custom_config_value::operator<vostok::math::float3> vostok::math::float3( ) const
{
	return vostok::math::float3(1., 1., 1.);

	// FUNCTION BODY[0x682a0]: 6
	// <0>
	// <1>
	// <0x682a0>|0x000|+0x046:'91'
	// <0x682e6>|0x046|-0x012:'92'
	// <0>
	// <0x682d4>|0x034|+0x011:'94'
	// <0x682e5>|0x045|+0x010:'95'
	// ******
}

// STATE[STUB]
inline post_process_parameters& post_process_parameters::operator=( post_process_parameters const& __that )
{
	// FUNCTION BODY[0x67750]: 15
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x67670>|0x000|+0x0a1:'104'
	// <0>
	// <1>
	// <0x67711>|0x0a1|+0x035:'107'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x67746>|0x0d6|-0x004:'112'
	// <0>
	// <1>
	// <0x67742>|0x0d2|+0x008:'115'
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
	// 	vostok::render::material_effects_entry*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_pass_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_shader_technique_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
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

#endif // #ifndef VOSTOK\RENDER\CORE\DX11\CUSTOM_CONFIG_VALUE_INLINE_H_INCLUDED
