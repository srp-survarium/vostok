////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\stage_decals_accumulate.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `vostok::render::effect_manager::create_effect<vostok::render::effect_apply_decal>'::`2'::`dynamic atexit destructor for 'descriptor_object''( )
{
	// FUNCTION BODY[0x7ee4b0]: 4
	// <0x62cd4e>|0x00e|+0x024:'46'
	// <0x62cd72>|0x032|+0x00d:'47'
	// <0>
	// <0x62cd7f>|0x03f|+0x00f:'49'
	// ******
}

// STATE[STUB]
 stage_decals_accumulate::~stage_decals_accumulate( )
{
	// FUNCTION BODY[0x62c490]: 1
	// <0>
	// ******
}

// STATE[STUB]
bool stage_decals_accumulate::is_effects_ready( ) const
{
	return false;

	// FUNCTION BODY[0x62c470]: 2
	// <0>
	// <0x62c470>|0x000|+0x012:'60'
	// <0x62c482>|0x012|-0x001:'60'
	// <0x62c481>|0x011|+0x003:'61'
	// ******
}

// STATE[STUB]
void stage_decals_accumulate::debug_render( )
{
	// LOCALS
	// vector< decal_instance* >& 		visible_decals
	// decal_instance** 				it
	// ******

	// STATICS
	// static bool 						s_render_debug = <0x4c26598>;
	// ******

	// FUNCTION BODY[0x62ca90]: 12
	// <0>
	// <1>
	// <0x62ca99>|0x009|+0x011:'67'
	// <0>
	// <1>
	// <0x62caaa>|0x01a|+0x009:'70'
	// <0>
	// <0x62cab3>|0x023|+0x025:'72'
	// <0>
	// <1>
	// <0x62cad8>|0x048|+0x25b:'75'
	// <0>
	// ******
}

// STATE[STUB]
void stage_decals_accumulate::execute_disabled( )
{
	// FUNCTION BODY[0x62c4f0]: 13
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x62c4f0>|0x000|+0x0ca:'86'
	// <0x62c5ba>|0x0ca|-0x028:'86'
	// <0x62c592>|0x0a2|+0x01b:'87'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x62c5ad>|0x0bd|+0x00c:'93'
	// ******
}

// STATE[STUB]
void stage_decals_accumulate::execute( )
{
	// LOCALS
	// vector< decal_instance* > 		visible_decals
	// D3D11_VIEWPORT 					tmp_viewport
	// D3D11_VIEWPORT 					orig_viewport
	// ******

	// TYPEDEFS
	// typedef
	// 	stage_decals_accumulate::execute::__l4::sort_by_priority_predicate
	// 	stage_decals_accumulate::execute::__l4::sort_by_priority_predicate;

	// ******

	// CALL SITE INFO
	// <0x62c639> -> bool < unknown >() const
	// <0x62ca6b> -> void < unknown >()
	// ******

	// FUNCTION BODY[0x62c5d0]: 95
	// <0>
	// <1>
	// <0x62c5df>|0x00f|+0x016:'100'
	// <0>
	// <1>
	// <0x62c5f5>|0x025|+0x018:'103'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x62c60d>|0x03d|+0x013:'112'
	// <0x62c620>|0x050|+0x011:'113'
	// <0>
	// <0x62c631>|0x061|+0x432:'115'
	// <0>
	// <0x62ca63>|0x493|+0x00a:'117'
	// <0x62ca6d>|0x49d|-0x422:'118'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x62c64b>|0x07b|+0x0be:'128'
	// <0>
	// <0x62c709>|0x139|+0x006:'130'
	// <0x62c70f>|0x13f|+0x027:'131'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x62c736>|0x166|+0x030:'138'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x62c766>|0x196|+0x009:'144'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x62c76f>|0x19f|+0x045:'149'
	// <0x62c7b4>|0x1e4|+0x031:'150'
	// <0x62c7e5>|0x215|+0x003:'151'
	// <0>
	// <1>
	// <0x62c7e8>|0x218|+0x031:'154'
	// <0>
	// <1>
	// <2>
	// <0x62c819>|0x249|+0x017:'158'
	// <0>
	// <1>
	// <2>
	// <0x62c830>|0x260|+0x01a:'162'
	// <0>
	// <1>
	// <2>
	// <0x62c84a>|0x27a|+0x03a:'166'
	// <0>
	// <1>
	// <2>
	// <0x62c884>|0x2b4|+0x00b:'170'
	// <0>
	// <1>
	// <0x62c88f>|0x2bf|+0x01e:'173'
	// <0>
	// <0x62c8ad>|0x2dd|+0x008:'175'
	// <0>
	// <0x62c8b5>|0x2e5|+0x00a:'177'
	// <0x62c8bf>|0x2ef|+0x066:'178'
	// <0>
	// <0x62c925>|0x355|+0x00c:'180'
	// <0>
	// <0x62c931>|0x361|+0x00d:'182'
	// <0x62c93e>|0x36e|+0x067:'183'
	// <0>
	// <0x62c9a5>|0x3d5|+0x00d:'185'
	// <0x62c9b2>|0x3e2|+0x067:'186'
	// <0>
	// <1>
	// <0x62ca19>|0x449|+0x00d:'189'
	// <0x62ca26>|0x456|+0x012:'190'
	// <0>
	// <1>
	// <0x62ca38>|0x468|+0x04d:'193'
	// ******
}

// STATE[STUB]
bool `vostok::render::stage_decals_accumulate::execute'::`4'::sort_by_priority_predicate::operator()(
	decal_instance const* const		arg_0 /* vostok::render::decal_instance const* const a */,
	decal_instance const* const		arg_1 /* vostok::render::decal_instance const* const b */
)
{
	return false;

	// FUNCTION BODY[0x62c450]: 1
	// <0x62c450>|0x000|+0x011:'108'
	// <0x62c461>|0x011|-0x001:'108'
	// <0x62c460>|0x010|+0x003:'109'
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
