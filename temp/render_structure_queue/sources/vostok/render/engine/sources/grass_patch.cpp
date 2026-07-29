////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\grass_patch.h"

namespace vostok {
namespace render {

// STATE[STUB]
bool has_surface_by_lod( grass_render_model_ptr mod, const u32 lod_index )
{
	return false;

	// FUNCTION BODY[0x5f43e0]: 7
	// <0x5f43e1>|0x001|+0x060:'61'
	// <0>
	// <0x5f4441>|0x061|-0x01c:'63'
	// <0x5f4425>|0x045|-0x009:'64'
	// <0x5f441c>|0x03c|-0x02d:'65'
	// <0x5f43ef>|0x00f|+0x02a:'66'
	// <0>
	// <0x5f4419>|0x039|+0x056:'68'
	// ******
}

// STATE[STUB]
grass_render_surface* surface_by_lod( grass_render_model_ptr mod, const u32 lod_index )
{
	return NULL;

	// FUNCTION BODY[0x5f4310]: 7
	// <0x5f4311>|0x001|+0x096:'72'
	// <0>
	// <0x5f43a7>|0x097|-0x025:'74'
	// <0x5f4382>|0x072|-0x032:'75'
	// <0x5f4350>|0x040|-0x02b:'76'
	// <0x5f4325>|0x015|+0x028:'77'
	// <0>
	// <0x5f434d>|0x03d|+0x084:'79'
	// ******
}

// STATE[STUB]
 grass_patch::grass_patch(
	collision::space_partitioning_tree* const	in_collision_tree,
	grass_template*		templ,
	float3 const&		in_origin,
	const float			in_size
)
{
	// FUNCTION BODY[0x5f5280]: 19
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5f538c>|0x10c|+0x05b:'106'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5f53e7>|0x167|+0x04a:'114'
	// <0x5f5431>|0x1b1|+0x051:'115'
	// <0>
	// <1>
	// <0x5f5482>|0x202|+0x092:'118'
	// ******
}

// STATE[STUB]
void grass_patch::remove_trample( )
{
	// FUNCTION BODY[0x5f3b10]: 2
	// <0x5f3b10>|0x000|+0x077:'123'
	// <0x5f3b87>|0x077|+0x018:'124'
	// ******
}

// STATE[STUB]
void grass_patch::try_accumulate_trample(
	trample_desc&			desc,
	grass_world*			in_grass_world,
	renderer*				in_renderer,
	renderer_context*		in_context
)
{
	// LOCALS
	// D3D11_VIEWPORT 					view_port
	// float 							linear_radius
	// ******

	// FUNCTION BODY[0x5f5060]: 49
	// <0>
	// <1>
	// <0x5f506c>|0x00c|+0x012:'131'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x5f507e>|0x01e|+0x06e:'141'
	// <0x5f50ec>|0x08c|+0x009:'142'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5f50f5>|0x095|+0x051:'149'
	// <0>
	// <0x5f5146>|0x0e6|+0x006:'151'
	// <0>
	// <0x5f514c>|0x0ec|+0x05c:'153'
	// <0x5f51a8>|0x148|+0x020:'154'
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
	// <0x5f51c8>|0x168|+0x0ac:'176'
	// <0>
	// ******
}

// STATE[STUB]
 grass_patch::~grass_patch( )
{
	// CALL SITE INFO
	// <0x5f4504> -> void < unknown >( collision::object* )
	// ******

	// FUNCTION BODY[0x5f4480]: 11
	// <0x5f4480>|0x000|+0x020:'241'
	// <0>
	// <0x5f44a0>|0x020|+0x020:'243'
	// <0x5f44c0>|0x040|+0x026:'244'
	// <0>
	// <1>
	// <0x5f44e6>|0x066|+0x012:'247'
	// <0x5f44f8>|0x078|+0x014:'248'
	// <0>
	// <0x5f450c>|0x08c|+0x032:'250'
	// <0x5f453e>|0x0be|+0x038:'251'
	// ******
}

// STATE[STUB]
void grass_patch::init_collision( )
{
	// LOCALS
	// float4x4 						transform
	// ******

	// STATICS
	// static math::random32 			s_randomizer = <0x4c2acc4>;
	// ******

	// CALL SITE INFO
	// <0x5f408e> -> void < unknown >( collision::object*, float4x4 const& )
	// ******

	// FUNCTION BODY[0x5f3ba0]: 24
	// <0x5f3ba6>|0x006|+0x024:'256'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5f3bca>|0x02a|+0x01c:'261'
	// <0x5f3be6>|0x046|+0x010:'262'
	// <0x5f3bf6>|0x056|+0x00a:'263'
	// <0>
	// <1>
	// <0x5f3c00>|0x060|+0x0ff:'266'
	// <0>
	// <0x5f3cff>|0x15f|+0x010:'268'
	// <0>
	// <1>
	// <0x5f3d0f>|0x16f|+0x08d:'271'
	// <0>
	// <0x5f3d9c>|0x1fc|+0x0dd:'273'
	// <0>
	// <0x5f3e79>|0x2d9|+0x0b8:'275'
	// <0x5f3f31>|0x391|+0x0c0:'276'
	// <0x5f3ff1>|0x451|+0x087:'277'
	// <0>
	// <0x5f4078>|0x4d8|+0x018:'279'
	// ******
}

// STATE[STUB]
u32 grass_patch::get_stream_1_stride( ) const
{
	return 0;

	// FUNCTION BODY[0x5f3ae0]: 1
	// <0x5f3ae0>|0x000|+0x005:'290'
	// ******
}

// STATE[STUB]
u32 grass_patch::get_index_count( ) const
{
	return 0;

	// FUNCTION BODY[0x5f3ad0]: 1
	// <0x5f3ad0>|0x000|+0x00d:'295'
	// ******
}

// STATE[STUB]
u32 grass_patch::get_valid_lod_index( const u32 lod_index ) const
{
	return 0;

	// FUNCTION BODY[0x5f3ab0]: 5
	// <0x5f3ab0>|0x000|+0x00b:'318'
	// <0>
	// <1>
	// <2>
	// <0x5f3abb>|0x00b|+0x006:'322'
	// <0x5f3ac1>|0x011|-0x007:'322'
	// <0x5f3aba>|0x00a|+0x006:'323'
	// <0x5f3ac0>|0x010|+0x003:'323'
	// ******
}

// STATE[STUB]
void grass_patch::render(
	grass_world*				in_grass_world,
	renderer_context*			context,
	float3 const&				viewer_position,
	enum_render_stage_type		stage_type,
	const u32					tech_index,
	const float					draw_distance,
	res_effect*					debug_effect,
	const u32					cascade_index
)
{
	// LOCALS
	// const float 						to_aabb_center_squared
	// ******

	// FUNCTION BODY[0x5f4df0]: 44
	// <0x5f4df3>|0x003|+0x038:'334'
	// <0>
	// <0x5f4e2b>|0x03b|+0x029:'336'
	// <0>
	// <0x5f4e54>|0x064|+0x05b:'338'
	// <0>
	// <0x5f4eaf>|0x0bf|+0x008:'340'
	// <0>
	// <1>
	// <2>
	// <0x5f4eb7>|0x0c7|+0x03d:'344'
	// <0>
	// <1>
	// <0x5f4ef4>|0x104|+0x02e:'347'
	// <0>
	// <1>
	// <0x5f4f22>|0x132|+0x01c:'350'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5f4f3e>|0x14e|+0x017:'356'
	// <0>
	// <0x5f4f55>|0x165|+0x015:'358'
	// <0x5f4f6a>|0x17a|+0x012:'359'
	// <0x5f4f7c>|0x18c|+0x01f:'360'
	// <0>
	// <0x5f4f9b>|0x1ab|+0x00f:'362'
	// <0>
	// <0x5f4faa>|0x1ba|+0x018:'364'
	// <0x5f4fc2>|0x1d2|+0x018:'365'
	// <0>
	// <1>
	// <0x5f4fda>|0x1ea|+0x00a:'368'
	// <0>
	// <0x5f4fe4>|0x1f4|+0x035:'370'
	// <0>
	// <0x5f5019>|0x229|+0x005:'372'
	// <0x5f501e>|0x22e|+0x007:'373'
	// <0>
	// <0x5f5025>|0x235|+0x01e:'375'
	// <0>
	// <0x5f5043>|0x253|+0x00b:'377'
	// ******
}

// STATE[STUB]
void grass_patch::sort_instances( float3 const& view_position )
{
	// LOCALS
	// u16* 							merged_indices_copy
	// u32 								i
	// ******

	// CALL SITE INFO
	// <0x5f42eb> -> void < unknown >( ID3D11Resource*, u32, D3D11_BOX const*, pcvoid, u32, u32 )
	// ******

	// FUNCTION BODY[0x5f41d0]: 34
	// <0x5f4267>|0x097|-0x094:'382'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5f41d3>|0x003|+0x05d:'390'
	// <0>
	// <0x5f4230>|0x060|+0x018:'392'
	// <0>
	// <1>
	// <0x5f4248>|0x078|+0x073:'395'
	// <0x5f42bb>|0x0eb|-0x050:'395'
	// <0>
	// <0x5f426b>|0x09b|+0x00b:'397'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5f4276>|0x0a6|+0x01a:'403'
	// <0x5f4290>|0x0c0|+0x02f:'404'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5f42bf>|0x0ef|+0x02e:'413'
	// <0>
	// <0x5f42ed>|0x11d|+0x019:'415'
	// ******
}

// STATE[STUB]
math::color transform_packed_normal( float4x4 const& transform_matrix, math::color const& packed_normal )
{
	// LOCALS
	// base_basis 						basis
	// ******

	return vostok::math::color();

	// FUNCTION BODY[0x5f40a0]: 12
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5f40a3>|0x003|+0x014:'425'
	// <0>
	// <0x5f40b7>|0x017|+0x031:'427'
	// <0>
	// <1>
	// <0x5f40e8>|0x048|+0x0b6:'430'
	// <0x5f419e>|0x0fe|+0x022:'431'
	// ******
}

// STATE[STUB]
void grass_patch::merge_instances( )
{
	// LOCALS
	// u32 								lod_index
	// stream_1_type* 					stream_1_data_it
	// grass_source_vertex* 			merged_vertices
	// stream_1_type* 					stream_1_data
	// grass_instance* const* 			end
	// u16* 							merged_indices_it
	// u32 								current_num_indices
	// u32 								current_num_vertices
	// grass_instance* const* 			it
	// grass_patch::sort_info* 			sort_info_it
	// grass_instance& 					current_instance
	// math::color 						color_and_wind
	// const u32 						copy_i_bytes
	// u32 								i
	// ******

	// FUNCTION BODY[0x5f4700]: 201
	// <0x5f4707>|0x007|+0x010:'436'
	// <0>
	// <0x5f4717>|0x017|+0x00d:'438'
	// <0>
	// <1>
	// <0x5f4724>|0x024|+0x00c:'441'
	// <0>
	// <0x5f4730>|0x030|+0x010:'443'
	// <0>
	// <1>
	// <0x5f4740>|0x040|+0x006:'446'
	// <0>
	// <1>
	// <2>
	// <0x5f4746>|0x046|+0x020:'450'
	// <0>
	// <0x5f4766>|0x066|+0x003:'452'
	// <0x5f4769>|0x069|+0x003:'453'
	// <0>
	// <1>
	// <2>
	// <0x5f476c>|0x06c|+0x010:'457'
	// <0>
	// <0x5f477c>|0x07c|+0x002:'459'
	// <0x5f477e>|0x07e|+0x03b:'460'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <8>
	// <0x5f47b9>|0x0b9|+0x00e:'470'
	// <0>
	// <0x5f47c7>|0x0c7|+0x002:'472'
	// <0x5f47c9>|0x0c9|+0x029:'473'
	// <0>
	// <0x5f47f2>|0x0f2|+0x004:'475'
	// <0>
	// <1>
	// <0x5f47f6>|0x0f6|+0x009:'478'
	// <0x5f47ff>|0x0ff|+0x010:'479'
	// <0>
	// <1>
	// <0x5f480f>|0x10f|+0x01e:'482'
	// <0>
	// <0x5f482d>|0x12d|+0x00f:'484'
	// <0>
	// <1>
	// <0x5f483c>|0x13c|+0x01c:'487'
	// <0>
	// <1>
	// <0x5f4858>|0x158|+0x01a:'490'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5f4872>|0x172|+0x031:'496'
	// <0>
	// <0x5f48a3>|0x1a3|+0x022:'498'
	// <0x5f48c5>|0x1c5|+0x004:'499'
	// <0>
	// <0x5f48c9>|0x1c9|+0x36a:'501'
	// <0x5f4c33>|0x533|-0x34f:'501'
	// <0>
	// <0x5f48e4>|0x1e4|+0x002:'503'
	// <0x5f48e6>|0x1e6|+0x033:'504'
	// <0>
	// <0x5f4919>|0x219|+0x008:'506'
	// <0>
	// <1>
	// <0x5f4921>|0x221|+0x016:'509'
	// <0>
	// <1>
	// <0x5f4937>|0x237|+0x06e:'512'
	// <0x5f49a5>|0x2a5|+0x087:'513'
	// <0x5f4a2c>|0x32c|+0x009:'514'
	// <0>
	// <1>
	// <2>
	// <0x5f4a35>|0x335|+0x01c:'518'
	// <0>
	// <0x5f4a51>|0x351|+0x00e:'520'
	// <0x5f4a5f>|0x35f|+0x019:'521'
	// <0>
	// <0x5f4a78>|0x378|+0x007:'523'
	// <0x5f4a7f>|0x37f|+0x02c:'524'
	// <0>
	// <0x5f4aab>|0x3ab|+0x015:'526'
	// <0x5f4ac0>|0x3c0|+0x01c:'527'
	// <0>
	// <0x5f4adc>|0x3dc|+0x015:'529'
	// <0>
	// <0x5f4af1>|0x3f1|+0x007:'531'
	// <0x5f4af8>|0x3f8|+0x00f:'532'
	// <0>
	// <0x5f4b07>|0x407|+0x0a9:'534'
	// <0>
	// <0x5f4bb0>|0x4b0|+0x016:'536'
	// <0x5f4bc6>|0x4c6|+0x013:'537'
	// <0x5f4bd9>|0x4d9|+0x011:'538'
	// <0>
	// <1>
	// <2>
	// <0x5f4bea>|0x4ea|+0x020:'542'
	// <0>
	// <1>
	// <0x5f4c0a>|0x50a|+0x006:'545'
	// <0x5f4c10>|0x510|-0x45c:'546'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x5f47b4>|0x0b4|+0x483:'554'
	// <0x5f4c37>|0x537|+0x0c4:'554'
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
	// <23>
	// <24>
	// <25>
	// <26>
	// <27>
	// <28>
	// <29>
	// <30>
	// <31>
	// <32>
	// <33>
	// <34>
	// <35>
	// <36>
	// <37>
	// <38>
	// <39>
	// <40>
	// <41>
	// <42>
	// <43>
	// <44>
	// <45>
	// <46>
	// <47>
	// <48>
	// <49>
	// <50>
	// <51>
	// <52>
	// <53>
	// <54>
	// <55>
	// <56>
	// <57>
	// <58>
	// <59>
	// <60>
	// <61>
	// <62>
	// <63>
	// <64>
	// <65>
	// <66>
	// <67>
	// <68>
	// <69>
	// <70>
	// <71>
	// <72>
	// <73>
	// <74>
	// <0x5f4cfb>|0x5fb|+0x082:'630'
	// <0>
	// <0x5f4d7d>|0x67d|+0x024:'632'
	// <0x5f4da1>|0x6a1|+0x012:'633'
	// <0>
	// <0x5f4db3>|0x6b3|+0x027:'635'
	// <0>
	// ******
}

// STATE[STUB]
bool grass_patch::is_occluded( ) const
{
	return false;

	// FUNCTION BODY[0x5f3af0]: 1
	// <0x5f3af0>|0x000|+0x01d:'641'
	// <0x5f3b0d>|0x01d|-0x001:'641'
	// <0x5f3b0c>|0x01c|+0x003:'642'
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
