////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\stage_rain.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic atexit destructor for 's_rain_debug2_cc''( )
{
	// FUNCTION BODY[0x7eee20]
	// ******
}

// STATE[STUB]
 stage_rain::stage_rain( renderer* in_renderer, renderer_context* context )
{
	// FUNCTION BODY[0x60def0]: 23
	// <0x60df4c>|0x05c|+0x05a:'69'
	// <0x60dfa6>|0x0b6|+0x05c:'70'
	// <0x60e002>|0x112|+0x05c:'71'
	// <0x60e05e>|0x16e|+0x05c:'72'
	// <0x60e0ba>|0x1ca|+0x05c:'73'
	// <0>
	// <1>
	// <2>
	// <0x60e116>|0x226|+0x046:'77'
	// <0>
	// <0x60e15c>|0x26c|+0x00d:'79'
	// <0x60e169>|0x279|+0x00d:'80'
	// <0>
	// <0x60e176>|0x286|+0x01f:'82'
	// <0>
	// <0x60e195>|0x2a5|+0x00d:'84'
	// <0>
	// <1>
	// <2>
	// <0x60e1a2>|0x2b2|+0x071:'88'
	// <0x60e213>|0x323|+0x008:'89'
	// <0x60e21b>|0x32b|+0x013:'90'
	// <0>
	// ******
}

// STATE[STUB]
bool stage_rain::is_effects_ready( ) const
{
	return false;

	// FUNCTION BODY[0x60d430]: 1
	// <0x60d430>|0x000|+0x012:'97'
	// <0x60d442>|0x012|-0x001:'97'
	// <0x60d441>|0x011|+0x003:'98'
	// ******
}

// STATE[STUB]
 stage_rain::~stage_rain( )
{
	// FUNCTION BODY[0x60d480]: 1
	// <0x60d486>|0x006|+0x019:'102'
	// ******
}

// STATE[STUB]
void stage_rain::execute( )
{
	// LOCALS
	// float 							mult
	// math::random32 					r
	// float2 							right_dir_2d
	// float2 							offset_vector_ground
	// float2 							view_dir_2d
	// const float 						rain_angle_x
	// const float 						rain_speed
	// float2 							offset_direction_ground
	// vector< render_surface_instance* > m_caster_model
	// const float 						rain_angle_y
	// float2 							rain_uv_scales
	// const u32 						num_cones
	// float4x4 						view_to_shadow
	// float4x4 						view_matrix
	// const float 						abs_ov_dot_dir_ground2
	// const float 						rain_density
	// const float 						abs_ov_dot_dir_ground
	// float4x4 						cone_transform
	// const float 						cone_scale
	// ******

	// STATICS
	// static bool 						s_first_pass = <0xa79d47>;
	// ******

	// CALL SITE INFO
	// <0x60d53c> -> bool < unknown >() const
	// <0x60d5e2> -> void < unknown >( ID3D11Resource*, ID3D11Resource* )
	// <0x60dee2> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x60d510]: 149
	// <0>
	// <1>
	// <0x60d522>|0x012|+0x014:'109'
	// <0>
	// <1>
	// <2>
	// <0x60d536>|0x026|+0x9a4:'113'
	// <0>
	// <0x60deda>|0x9ca|-0x96a:'115'
	// <0>
	// <1>
	// <2>
	// <0x60d570>|0x060|+0x019:'119'
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
	// <0x60d589>|0x079|+0x06d:'130'
	// <0>
	// <1>
	// <2>
	// <0x60d5f6>|0x0e6|+0x008:'134'
	// <0>
	// <0x60d5fe>|0x0ee|+0x021:'136'
	// <0x60d61f>|0x10f|+0x006:'137'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x60d625>|0x115|+0x003:'142'
	// <0>
	// <0x60d628>|0x118|+0x092:'144'
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
	// <0x60d6ba>|0x1aa|+0x003:'168'
	// <0>
	// <0x60d6bd>|0x1ad|+0x059:'170'
	// <0x60d716>|0x206|+0x042:'171'
	// <0>
	// <1>
	// <0x60d758>|0x248|+0x024:'174'
	// <0x60d77c>|0x26c|+0x034:'175'
	// <0>
	// <0x60d7b0>|0x2a0|+0x024:'177'
	// <0x60d7d4>|0x2c4|+0x014:'178'
	// <0>
	// <0x60d7e8>|0x2d8|+0x01b:'180'
	// <0x60d803>|0x2f3|+0x05f:'181'
	// <0x60d862>|0x352|+0x005:'182'
	// <0x60d867>|0x357|+0x059:'183'
	// <0x60d8c0>|0x3b0|+0x012:'184'
	// <0x60d8d2>|0x3c2|+0x00c:'185'
	// <0x60d8de>|0x3ce|+0x023:'186'
	// <0x60d901>|0x3f1|+0x011:'187'
	// <0>
	// <0x60d912>|0x402|+0x005:'189'
	// <0x60d917>|0x407|+0x061:'190'
	// <0x60d978>|0x468|+0x005:'191'
	// <0x60d97d>|0x46d|+0x05b:'192'
	// <0x60d9d8>|0x4c8|+0x00a:'193'
	// <0x60d9e2>|0x4d2|+0x00c:'194'
	// <0x60d9ee>|0x4de|+0x023:'195'
	// <0x60da11>|0x501|+0x011:'196'
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
	// <0x60da22>|0x512|+0x07e:'213'
	// <0x60daa0>|0x590|+0x006:'214'
	// <0>
	// <0x60daa6>|0x596|+0x025:'216'
	// <0>
	// <0x60dacb>|0x5bb|-0x01d:'218'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x60daae>|0x59e|+0x027:'227'
	// <0x60dad5>|0x5c5|+0x1bd:'227'
	// <0>
	// <0x60dc92>|0x782|+0x00d:'229'
	// <0x60dc9f>|0x78f|+0x010:'230'
	// <0x60dcaf>|0x79f|+0x05d:'231'
	// <0x60dd0c>|0x7fc|+0x045:'232'
	// <0x60dd51>|0x841|+0x045:'233'
	// <0x60dd96>|0x886|+0x045:'234'
	// <0x60dddb>|0x8cb|+0x024:'235'
	// <0>
	// <0x60ddff>|0x8ef|+0x008:'237'
	// <0>
	// <0x60de07>|0x8f7|+0x023:'239'
	// <0x60de2a>|0x91a|+0x044:'240'
	// <0>
	// <1>
	// <2>
	// <0x60de6e>|0x95e|+0x019:'244'
	// <0x60de87>|0x977|+0x01a:'245'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x60dea1>|0x991|+0x00d:'254'
	// <0x60deae>|0x99e|+0x01b:'255'
	// <0x60dec9>|0x9b9|+0x01b:'256'
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
