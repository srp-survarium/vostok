////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\stage_accumulate_distortion.h"

namespace vostok {
namespace render {

// STATE[STUB]
 stage_accumulate_distortion::stage_accumulate_distortion( renderer* in_renderer, renderer_context* context )
{
	// FUNCTION BODY[0x7489c0]: 1
	// <0x7489cf>|0x00f|+0x018:'31'
	// ******
}

// STATE[STUB]
 stage_accumulate_distortion::~stage_accumulate_distortion( )
{
	// FUNCTION BODY[0x7489b0]: 1
	// <0>
	// ******
}

// STATE[STUB]
void stage_accumulate_distortion::execute( )
{
	// LOCALS
	// vector< render_surface_instance* > m_dynamic_visuals
	// D3D11_VIEWPORT 					prev_view_port
	// D3D11_VIEWPORT 					view_port
	// bool 							need_execute
	// vectora< particle::render_particle_emitter_instance* > emitters
	// vectora< particle::render_particle_emitter_instance* > emitters
	// particle::render_particle_emitter_instance* const* it
	// render_surface_instance** 		it_d
	// render_surface_instance* const* 	end_d
	// ******

	// CALL SITE INFO
	// <0x748a07> -> bool < unknown >() const
	// <0x748a14> -> void < unknown >()
	// <0x748aa3> -> void < unknown >( float4x4 const&, vectora< particle::render_particle_emitter_instance* >& )
	// <0x748ada> -> void < unknown >()
	// <0x748d0f> -> void < unknown >( float4x4 const&, vectora< particle::render_particle_emitter_instance* >& )
	// ******

	// FUNCTION BODY[0x7489f0]: 126
	// <0>
	// <1>
	// <0x748a00>|0x010|+0x00d:'43'
	// <0>
	// <0x748a0d>|0x01d|+0x011:'45'
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
	// <0x748a1e>|0x02e|+0x037:'56'
	// <0>
	// <0x748a55>|0x065|+0x010:'58'
	// <0x748a65>|0x075|+0x005:'59'
	// <0>
	// <0x748a6a>|0x07a|+0x00f:'61'
	// <0>
	// <0x748a79>|0x089|+0x004:'63'
	// <0>
	// <0x748a7d>|0x08d|+0x006:'65'
	// <0>
	// <0x748a83>|0x093|+0x022:'67'
	// <0>
	// <0x748aa5>|0x0b5|+0x012:'69'
	// <0>
	// <0x748ab7>|0x0c7|+0x005:'71'
	// <0>
	// <0x748abc>|0x0cc|+0x010:'73'
	// <0>
	// <1>
	// <0x748acc>|0x0dc|+0x007:'76'
	// <0>
	// <0x748ad3>|0x0e3|+0x009:'78'
	// <0x748adc>|0x0ec|+0x005:'79'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x748ae1>|0x0f1|+0x029:'84'
	// <0>
	// <0x748b0a>|0x11a|+0x03b:'86'
	// <0x748b45>|0x155|+0x03b:'87'
	// <0x748b80>|0x190|+0x003:'88'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x748b83>|0x193|+0x03e:'93'
	// <0>
	// <0x748bc1>|0x1d1|+0x0cf:'95'
	// <0x748c90>|0x2a0|+0x022:'96'
	// <0x748cb2>|0x2c2|+0x020:'97'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x748cd2>|0x2e2|+0x00f:'102'
	// <0>
	// <0x748ce1>|0x2f1|+0x008:'104'
	// <0>
	// <0x748ce9>|0x2f9|+0x005:'106'
	// <0>
	// <0x748cee>|0x2fe|+0x023:'108'
	// <0>
	// <0x748d11>|0x321|+0x246:'110'
	// <0x748f57>|0x567|-0x232:'110'
	// <0>
	// <0x748d25>|0x335|+0x002:'112'
	// <0x748d27>|0x337|+0x028:'113'
	// <0>
	// <0x748d4f>|0x35f|+0x02d:'115'
	// <0>
	// <1>
	// <0x748d7c>|0x38c|+0x012:'118'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x748d8e>|0x39e|+0x17c:'126'
	// <0x748f0a>|0x51a|+0x014:'127'
	// <0>
	// <0x748f1e>|0x52e|+0x00f:'129'
	// <0>
	// <0x748f2d>|0x53d|+0x02e:'131'
	// <0>
	// <1>
	// <0x748f5b>|0x56b|+0x015:'134'
	// <0>
	// <1>
	// <0x748f70>|0x580|+0x006:'137'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x748f76>|0x586|+0x004:'142'
	// <0x748f7a>|0x58a|+0x00c:'143'
	// <0>
	// <0x748f86>|0x596|+0x00a:'145'
	// <0>
	// <0x748f90>|0x5a0|+0x002:'147'
	// <0x748f92>|0x5a2|+0x027:'148'
	// <0>
	// <1>
	// <0x748fb9>|0x5c9|+0x00d:'151'
	// <0>
	// <1>
	// <0x748fc6>|0x5d6|+0x00c:'154'
	// <0x748fd2>|0x5e2|+0x024:'155'
	// <0x748ff6>|0x606|+0x008:'156'
	// <0x748ffe>|0x60e|+0x096:'157'
	// <0>
	// <1>
	// <2>
	// <0x749094>|0x6a4|+0x01d:'161'
	// <0>
	// <1>
	// <0x7490b1>|0x6c1|+0x00d:'164'
	// <0x7490be>|0x6ce|+0x01b:'165'
	// <0x7490d9>|0x6e9|-0x6c3:'166'
	// <0x748a16>|0x026|+0x6da:'167'
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
