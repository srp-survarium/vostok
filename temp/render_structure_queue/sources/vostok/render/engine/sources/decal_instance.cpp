////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\decal_instance.h"

namespace vostok {
namespace render {

// STATE[STUB]
 decal_shader_constants_and_geometry::decal_shader_constants_and_geometry( )
{
	// FUNCTION BODY[0x641550]: 5
	// <0x64156a>|0x01a|+0x05e:'32'
	// <0x6415c8>|0x078|+0x059:'33'
	// <0x641621>|0x0d1|+0x059:'34'
	// <0x64167a>|0x12a|+0x059:'35'
	// <0x6416d3>|0x183|+0x005:'36'
	// ******
}

// STATE[STUB]
void decal_shader_constants_and_geometry::set(
	renderer_context*		context,
	float4x4 const&			world_to_decal_matrix,
	float4x4 const&			decal_tangent_to_view_space_matrix,
	float					alpha_angle,
	float					clip_angle,
	float3 const&			decal_width_height_far_distance,
	float4x4 const&			decal_transform
)
{
	// FUNCTION BODY[0x640b30]: 18
	// <0x640b38>|0x008|+0x008:'47'
	// <0x640b40>|0x010|+0x029:'48'
	// <0x640b69>|0x039|+0x025:'49'
	// <0x640b8e>|0x05e|+0x077:'50'
	// <0x640c05>|0x0d5|+0x00d:'51'
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
	// ******
}

// STATE[STUB]
void decal_shader_constants_and_geometry::set_geometry( )
{
	// FUNCTION BODY[0x640b20]: 0
	// ******
}

// STATE[STUB]
void decal_shader_constants_and_geometry::create_decal_geometry( )
{
	// LOCALS
	// D3D11_INPUT_ELEMENT_DESC[1] 		layout
	// ******

	// FUNCTION BODY[0x641450]: 31
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x641453>|0x003|+0x042:'79'
	// <0>
	// <1>
	// <0x641495>|0x045|+0x00c:'82'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x6414a1>|0x051|+0x00d:'87'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6414ae>|0x05e|+0x03d:'93'
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
	// <0x6414eb>|0x09b|+0x05c:'104'
	// ******
}

// STATE[STUB]
 decal_instance::decal_instance(
	collision::space_partitioning_tree*		tree,
	decal_properties const&					properties,
	const u32								id
)
{
	// FUNCTION BODY[0x6412b0]: 9
	// <0x64132d>|0x07d|+0x043:'122'
	// <0>
	// <1>
	// <2>
	// <0x641370>|0x0c0|+0x00a:'126'
	// <0>
	// <1>
	// <2>
	// <0x64137a>|0x0ca|+0x027:'130'
	// ******
}

// STATE[STUB]
 decal_instance::~decal_instance( )
{
	// FUNCTION BODY[0x640ef0]: 4
	// <0x640ef3>|0x003|+0x005:'135'
	// <0>
	// <1>
	// <0x640ef8>|0x008|+0x00f:'138'
	// ******
}

// STATE[STUB]
void decal_instance::destroy_impl( ) const
{
	// FUNCTION BODY[0x640f90]: 2
	// <0>
	// <0x640f94>|0x004|+0x01f:'144'
	// ******
}

// STATE[STUB]
void decal_instance::remove_collision( )
{
	// CALL SITE INFO
	// <0x640a84> -> void < unknown >( collision::object* )
	// ******

	// FUNCTION BODY[0x640a60]: 5
	// <0x640a65>|0x005|+0x013:'149'
	// <0x640a78>|0x018|+0x00e:'150'
	// <0>
	// <0x640a86>|0x026|+0x038:'152'
	// <0x640abe>|0x05e|+0x03b:'153'
	// ******
}

// STATE[STUB]
bool decal_instance::is_occluded( ) const
{
	return false;

	// FUNCTION BODY[0x640b00]: 1
	// <0x640b00>|0x000|+0x01d:'158'
	// <0x640b1d>|0x01d|-0x001:'158'
	// <0x640b1c>|0x01c|+0x003:'159'
	// ******
}

// STATE[STUB]
void decal_instance::set_materail_effects( resources::unmanaged_resource_ptr const& in_ptr )
{
	// FUNCTION BODY[0x640f30]: 12
	// <0x640f37>|0x007|+0x002:'163'
	// <0>
	// <0x640f39>|0x009|+0x00f:'165'
	// <0>
	// <0x640f48>|0x018|+0x00f:'167'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x640f57>|0x027|+0x029:'174'
	// ******
}

// STATE[STUB]
void decal_instance::set_properties( decal_properties const& in_properties )
{
	// LOCALS
	// float4x4 						m
	// float3 							sc
	// float4x4 						new_transform
	// ******

	// CALL SITE INFO
	// <0x64121e> -> void < unknown >( collision::object*, float4x4 const& )
	// ******

	// FUNCTION BODY[0x641000]: 49
	// <0x641016>|0x016|+0x01e:'179'
	// <0>
	// <0x641034>|0x034|+0x00f:'181'
	// <0x641043>|0x043|+0x018:'182'
	// <0>
	// <0x64105b>|0x05b|+0x016:'184'
	// <0>
	// <0x641071>|0x071|+0x008:'186'
	// <0>
	// <0x641079>|0x079|+0x03a:'188'
	// <0x6410b3>|0x0b3|+0x002:'189'
	// <0>
	// <0x6410b5>|0x0b5|+0x00b:'191'
	// <0x6410c0>|0x0c0|+0x00e:'192'
	// <0x6410ce>|0x0ce|+0x008:'193'
	// <0>
	// <1>
	// <0x6410d6>|0x0d6|+0x00c:'196'
	// <0x6410e2>|0x0e2|+0x010:'197'
	// <0x6410f2>|0x0f2|+0x006:'198'
	// <0x6410f8>|0x0f8|+0x006:'199'
	// <0x6410fe>|0x0fe|+0x007:'200'
	// <0x641105>|0x105|+0x007:'201'
	// <0x64110c>|0x10c|+0x006:'202'
	// <0x641112>|0x112|+0x007:'203'
	// <0x641119>|0x119|+0x007:'204'
	// <0>
	// <1>
	// <0x641120>|0x120|+0x00d:'207'
	// <0>
	// <0x64112d>|0x12d|+0x035:'209'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x641162>|0x162|+0x01b:'214'
	// <0x64117d>|0x17d|+0x03b:'215'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x6411b8>|0x1b8|+0x037:'221'
	// <0x6411ef>|0x1ef|+0x00e:'222'
	// <0x6411fd>|0x1fd|+0x00d:'223'
	// <0x64120a>|0x20a|+0x016:'224'
	// <0>
	// <0x641220>|0x220|+0x06e:'226'
	// <0x64128e>|0x28e|+0x00c:'227'
	// ******
}

// STATE[STUB]
decal_properties const& decal_instance::get_properties( ) const
{
	// FUNCTION BODY[0x640a50]: 1
	// <0x640a50>|0x000|+0x003:'232'
	// ******
}

// STATE[STUB]
material_effects const& decal_instance::get_effects( ) const
{
	// FUNCTION BODY[0x640fc0]: 9
	// <0x640fc1>|0x001|+0x035:'237'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x640ff6>|0x036|-0x01b:'242'
	// <0>
	// <1>
	// <0x640fdb>|0x01b|+0x017:'245'
	// <0x640ff2>|0x032|+0x009:'246'
	// ******
}

// STATE[STUB]
float4x4 get_decal_view_matrix( decal_instance* decal )
{
	// LOCALS
	// float3 							decal_direction
	// float4x4 						decal_world_matrix
	// float3 							scale
	// ******

	return vostok::math::float4x4();

	// FUNCTION BODY[0x640dc0]: 11
	// <0>
	// <0x640dc6>|0x006|+0x047:'252'
	// <0>
	// <0x640e0d>|0x04d|+0x00e:'254'
	// <0x640e1b>|0x05b|+0x033:'255'
	// <0x640e4e>|0x08e|+0x06f:'256'
	// <0>
	// <0x640ebd>|0x0fd|+0x018:'258'
	// <0x640ed5>|0x115|+0x00c:'259'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
float4x4 get_world_to_decal_matrix( decal_instance* decal )
{
	// LOCALS
	// float3 							decal_direction
	// float4x4 						decal_world_matrix
	// float4x4 						decal_projection_matrix
	// float3 							scale
	// float4x4 						decal_view_matrix
	// ******

	return vostok::math::float4x4();

	// FUNCTION BODY[0x640c20]: 18
	// <0>
	// <0x640c26>|0x006|+0x047:'267'
	// <0>
	// <0x640c6d>|0x04d|+0x00e:'269'
	// <0x640c7b>|0x05b|+0x033:'270'
	// <0x640cae>|0x08e|+0x04e:'271'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x640cfc>|0x0dc|+0x06c:'277'
	// <0>
	// <0x640d68>|0x148|+0x00e:'279'
	// <0x640d76>|0x156|+0x019:'280'
	// <0x640d8f>|0x16f|+0x016:'281'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void decal_instance::render_geometry( )
{
	// FUNCTION BODY[0x6413b0]: 8
	// <0x6413b0>|0x000|+0x010:'288'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6413c0>|0x010|+0x07d:'295'
	// ******
}

// STATE[STUB]
void decal_instance::render( renderer_context* context, enum_render_stage_type stage_type )
{
	// FUNCTION BODY[0x6416e0]: 21
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6416e0>|0x000|+0x0c1:'307'
	// <0x6417a1>|0x0c1|-0x051:'307'
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
	// <0x641750>|0x070|+0x041:'318'
	// <0>
	// <0x641791>|0x0b1|+0x005:'320'
	// <0>
	// ******
}

// STATE[STUB]
u32 decal_instance::draw(
	renderer_context*			context,
	res_effect_ptr				opaque_geometry_mask_effect,
	enum_render_stage_type		stage_type
)
{
	// LOCALS
	// u32 								num_draw_calls
	// float3 							decal_direction
	// float3 							angles
	// float3 							scale
	// float3 							position
	// float4x4 						geom_world_matrix
	// ******

	return 0;

	// FUNCTION BODY[0x6417b0]: 46
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6417be>|0x00e|+0x214:'348'
	// <0x6419d2>|0x222|-0x192:'348'
	// <0x641840>|0x090|+0x006:'349'
	// <0>
	// <1>
	// <2>
	// <0x641846>|0x096|+0x00e:'353'
	// <0x641854>|0x0a4|+0x03a:'354'
	// <0x64188e>|0x0de|+0x042:'355'
	// <0x6418d0>|0x120|+0x018:'356'
	// <0>
	// <0x6418e8>|0x138|+0x08e:'358'
	// <0>
	// <0x641976>|0x1c6|+0x014:'360'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x64198a>|0x1da|+0x00d:'368'
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
	// <0x641997>|0x1e7|+0x02c:'387'
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
