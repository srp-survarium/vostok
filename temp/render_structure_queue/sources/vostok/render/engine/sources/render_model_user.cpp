////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\render_model_user.h"

namespace vostok {
namespace render {

// STATE[STUB]
void user_render_surface::material_ready(
	resources::queries_result&		data,
	material_effects_instance_cook_data*	cook_data,
	char*							material_name
)
{
	// FUNCTION BODY[0x632c90]: 6
	// <0x632c94>|0x004|+0x012:'30'
	// <0>
	// <0x632ca6>|0x016|+0x013:'32'
	// <0x632cb9>|0x029|+0x05c:'33'
	// <0>
	// <0x632d15>|0x085|+0x019:'35'
	// ******
}

// STATE[STUB]
void user_render_model_instance::assign_surface( user_render_surface* surface )
{
	// FUNCTION BODY[0x632c10]: 5
	// <0x632c10>|0x000|+0x006:'40'
	// <0x632c16>|0x006|+0x006:'41'
	// <0x632c1c>|0x00c|+0x00c:'42'
	// <0x632c28>|0x018|+0x006:'43'
	// <0x632c2e>|0x01e|+0x00a:'44'
	// ******
}

// STATE[STUB]
void user_render_model_instance::get_surfaces(
	float4x4 const*							mat_vp,
	float3 const*							view_pos,
	vector< render_surface_instance* >&		dest,
	bool									visible_only,
	u8										__formal,
	u32										surface_flags
)
{
	// FUNCTION BODY[0x632c50]: 2
	// <0>
	// <0x632c52>|0x002|+0x021:'50'
	// <0x632c73>|0x023|-0x004:'50'
	// <0x632c6f>|0x01f|+0x014:'51'
	// ******
}

// STATE[STUB]
void user_render_model_instance::get_surface_stats( u32 surface_id, surface_stats& __formal ) const
{
	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x621c40 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN7") })
	// ******

	// FUNCTION BODY[0x632c40]
	// ******
}

// STATE[STUB]
void user_render_surface_editable::load_from_chunk_data( memory::chunk_reader& chunk )
{
	// LOCALS
	// untyped_buffer_ptr 				vb
	// material_effects_instance_cook_data* cook_data
	// variant< 32 > 					user_data
	// res_declaration_ptr 				decl
	// fixed_string< 256 > 				material_name
	// untyped_buffer_ptr 				ib
	// ******

	// FUNCTION BODY[0x634030]: 57
	// <0>
	// <0x63403c>|0x00c|+0x00a:'78'
	// <0>
	// <0x634046>|0x016|+0x06f:'80'
	// <0>
	// <0x6340b5>|0x085|+0x06e:'82'
	// <0>
	// <0x634123>|0x0f3|+0x012:'84'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x634135>|0x105|+0x002:'90'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x634137>|0x107|+0x032:'96'
	// <0>
	// <1>
	// <2>
	// <0x634169>|0x139|+0x012:'100'
	// <0x63417b>|0x14b|+0x002:'101'
	// <0x63417d>|0x14d|+0x007:'102'
	// <0>
	// <0x634184>|0x154|+0x031:'104'
	// <0>
	// <0x6341b5>|0x185|+0x024:'106'
	// <0x6341d9>|0x1a9|+0x03e:'107'
	// <0>
	// <0x634217>|0x1e7|+0x006:'109'
	// <0>
	// <1>
	// <0x63421d>|0x1ed|+0x038:'112'
	// <0>
	// <0x634255>|0x225|+0x00a:'114'
	// <0x63425f>|0x22f|+0x005:'115'
	// <0>
	// <0x634264>|0x234|+0x028:'117'
	// <0x63428c>|0x25c|+0x00f:'118'
	// <0x63429b>|0x26b|+0x016:'119'
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
	// <0x6342b1>|0x281|+0x0c2:'133'
	// ******
}

// STATE[STUB]
void user_render_surface_wire::load_from_chunk_data( memory::chunk_reader& chunk )
{
	// LOCALS
	// memory::reader 					reader
	// untyped_buffer_ptr 				vb
	// material_effects_instance_cook_data* cook_data
	// variant< 32 > 					user_data
	// u16* 							indices
	// res_declaration_ptr 				decl
	// const u16[18] 					faces_indices
	// fixed_string< 256 > 				material_name
	// untyped_buffer_ptr 				ib
	// const u32 						num_indices
	// float 							u_coord
	// float3 							up_vector
	// float3 							left_vector
	// float3 							source_pos
	// float3 							to_next
	// float3 							direction
	// u32 								vertex_index
	// ******

	// FUNCTION BODY[0x632d40]: 129
	// <0>
	// <1>
	// <0x632d5d>|0x01d|+0x021:'158'
	// <0>
	// <0x632d7e>|0x03e|+0x074:'160'
	// <0>
	// <0x632df2>|0x0b2|+0x003:'162'
	// <0>
	// <0x632df5>|0x0b5|+0x014:'164'
	// <0>
	// <0x632e09>|0x0c9|+0x010:'166'
	// <0>
	// <0x632e19>|0x0d9|+0x023:'168'
	// <0>
	// <0x632e3c>|0x0fc|+0x00e:'170'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x632e4a>|0x10a|+0x015:'175'
	// <0x632e5f>|0x11f|+0x020:'176'
	// <0x632e7f>|0x13f|+0x01e:'177'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x632e9d>|0x15d|+0x016:'182'
	// <0x632eb3>|0x173|+0x01e:'183'
	// <0>
	// <1>
	// <2>
	// <0x632ed1>|0x191|+0x0c1:'187'
	// <0x632f92>|0x252|-0x0be:'187'
	// <0>
	// <0x632ed4>|0x194|+0x0c4:'189'
	// <0>
	// <0x632f98>|0x258|+0x008:'191'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x632fa0>|0x260|+0x013:'196'
	// <0x632fb3>|0x273|+0x049:'197'
	// <0x632ffc>|0x2bc|+0x002:'198'
	// <0x632ffe>|0x2be|+0x04a:'199'
	// <0>
	// <0x633048>|0x308|+0x024:'201'
	// <0>
	// <1>
	// <2>
	// <0x63306c>|0x32c|+0x08a:'205'
	// <0x6330f6>|0x3b6|+0x0de:'206'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6331d4>|0x494|-0x2eb:'212'
	// <0x632ee9>|0x1a9|+0x382:'213'
	// <0x63326b>|0x52b|-0x326:'213'
	// <0x632f45>|0x205|+0x3d3:'214'
	// <0x633318>|0x5d8|+0x0d9:'214'
	// <0>
	// <0x6333f1>|0x6b1|+0x0f8:'216'
	// <0x6334e9>|0x7a9|+0x0f2:'217'
	// <0x6335db>|0x89b|+0x0f2:'218'
	// <0>
	// <0x6336cd>|0x98d|+0x0b7:'220'
	// <0x633784>|0xa44|+0x0bf:'221'
	// <0x633843>|0xb03|+0x075:'222'
	// <0>
	// <0x6338b8>|0xb78|+0x0ec:'224'
	// <0x6339a4>|0xc64|+0x0eb:'225'
	// <0x633a8f>|0xd4f|+0x102:'226'
	// <0>
	// <0x633b91>|0xe51|-0xc34:'228'
	// <0>
	// <1>
	// <2>
	// <0x632f5d>|0x21d|+0xc37:'232'
	// <0x633b94>|0xe54|+0x04f:'232'
	// <0x633be3>|0xea3|-0xc7e:'233'
	// <0x632f65>|0x225|+0xc8c:'234'
	// <0x633bf1>|0xeb1|+0x005:'234'
	// <0>
	// <0x633bf6>|0xeb6|+0x094:'236'
	// <0>
	// <1>
	// <0x633c8a>|0xf4a|+0x034:'239'
	// <0x633cbe>|0xf7e|+0x042:'240'
	// <0x633d00>|0xfc0|+0x041:'241'
	// <0>
	// <0x633d41>|0x1001|+0x01d:'243'
	// <0x633d5e>|0x101e|+0x012:'244'
	// <0x633d70>|0x1030|+0x023:'245'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x633d93>|0x1053|+0x02c:'252'
	// <0>
	// <0x633dbf>|0x107f|+0x003:'254'
	// <0x633dc2>|0x1082|+0x007:'255'
	// <0>
	// <0x633dc9>|0x1089|+0x033:'257'
	// <0>
	// <0x633dfc>|0x10bc|+0x026:'259'
	// <0x633e22>|0x10e2|+0x036:'260'
	// <0>
	// <1>
	// <0x633e58>|0x1118|+0x02d:'263'
	// <0>
	// <0x633e85>|0x1145|+0x00c:'265'
	// <0x633e91>|0x1151|+0x01a:'266'
	// <0>
	// <0x633eab>|0x116b|+0x020:'268'
	// <0x633ecb>|0x118b|+0x00f:'269'
	// <0x633eda>|0x119a|+0x015:'270'
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
	// <0x633eef>|0x11af|+0x0c5:'284'
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
