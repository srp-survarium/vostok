////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\speedtree_tree_component_branch.h"

namespace vostok {
namespace render {

// STATE[STUB]
explicit speedtree_tree_component_branch::speedtree_tree_component_branch( speedtree_tree& parent )
{
	// LOCALS
	// vector< branch_vertex > 			total_vertices
	// SpeedTree::SIndexedTriangles const* lods
	// res_declaration_ptr 				decl
	// s32 								num_lods
	// vector< u16 > 					total_indices
	// s32 								lod_index
	// vector< branch_vertex > 			vertices
	// vector< u16 > 					indices
	// u16 const* 						it
	// branch_vertex const* 			it
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5e7f82 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1207") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5e7f82 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1209") })
	// ******

	// FUNCTION BODY[0x5f8760]: 109
	// <0x5f8766>|0x006|+0x061:'49'
	// <0>
	// <0x5f87c7>|0x067|+0x00e:'51'
	// <0>
	// <1>
	// <2>
	// <0x5f87d5>|0x075|+0x006:'55'
	// <0x5f87db>|0x07b|+0x00a:'56'
	// <0>
	// <1>
	// <0x5f87e5>|0x085|+0x006:'59'
	// <0x5f87eb>|0x08b|+0x017:'60'
	// <0>
	// <0x5f8802>|0x0a2|+0x006:'62'
	// <0>
	// <1>
	// <0x5f8808>|0x0a8|+0x02f:'65'
	// <0>
	// <0x5f8837>|0x0d7|+0x00e:'67'
	// <0>
	// <0x5f8845>|0x0e5|+0x00f:'69'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5f8854>|0x0f4|+0x002:'74'
	// <0x5f8856>|0x0f6|+0x016:'75'
	// <0>
	// <0x5f886c>|0x10c|+0x015:'77'
	// <0x5f8881>|0x121|+0x018:'78'
	// <0>
	// <0x5f8899>|0x139|+0x037:'80'
	// <0x5f88d0>|0x170|+0x6a8:'81'
	// <0x5f8f78>|0x818|-0x579:'81'
	// <0>
	// <1>
	// <0x5f89ff>|0x29f|+0x002:'84'
	// <0x5f8a01>|0x2a1|+0x034:'85'
	// <0>
	// <1>
	// <0x5f8a35>|0x2d5|+0x1f0:'88'
	// <0x5f8c25>|0x4c5|-0x1d5:'88'
	// <0>
	// <1>
	// <0x5f8a50>|0x2f0|+0x014:'91'
	// <0>
	// <0x5f8a64>|0x304|+0x0e7:'93'
	// <0>
	// <1>
	// <0x5f8b4b>|0x3eb|+0x01c:'96'
	// <0>
	// <1>
	// <0x5f8b67>|0x407|+0x013:'99'
	// <0>
	// <1>
	// <0x5f8b7a>|0x41a|+0x015:'102'
	// <0>
	// <1>
	// <0x5f8b8f>|0x42f|+0x009:'105'
	// <0x5f8b98>|0x438|+0x00a:'106'
	// <0>
	// <1>
	// <0x5f8ba2>|0x442|+0x006:'109'
	// <0>
	// <0x5f8ba8>|0x448|+0x009:'111'
	// <0x5f8bb1>|0x451|+0x00a:'112'
	// <0>
	// <0x5f8bbb>|0x45b|+0x002:'114'
	// <0x5f8bbd>|0x45d|+0x00a:'115'
	// <0>
	// <1>
	// <0x5f8bc7>|0x467|+0x013:'118'
	// <0x5f8bda>|0x47a|+0x004:'119'
	// <0>
	// <1>
	// <0x5f8bde>|0x47e|+0x006:'122'
	// <0>
	// <0x5f8be4>|0x484|+0x006:'124'
	// <0x5f8bea>|0x48a|+0x00c:'125'
	// <0>
	// <0x5f8bf6>|0x496|+0x002:'127'
	// <0>
	// <0x5f8bf8>|0x498|+0x005:'129'
	// <0x5f8bfd>|0x49d|+0x016:'130'
	// <0>
	// <1>
	// <0x5f8c13>|0x4b3|+0x016:'133'
	// <0>
	// <1>
	// <0x5f8c29>|0x4c9|+0x01c:'136'
	// <0x5f8c45>|0x4e5|+0x160:'137'
	// <0x5f8da5>|0x645|+0x059:'138'
	// <0>
	// <0x5f8dfe>|0x69e|+0x004:'140'
	// <0x5f8e02>|0x6a2|+0x007:'141'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5f8e09>|0x6a9|+0x028:'146'
	// <0>
	// <0x5f8e31>|0x6d1|+0x01b:'148'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5f8e4c>|0x6ec|+0x02f:'155'
	// <0>
	// <0x5f8e7b>|0x71b|+0x039:'157'
	// ******
}

// STATE[STUB]
void speedtree_tree_component_branch::init_index_buffer( SpeedTree::SIndexedTriangles const* lod, vector< u16 >& out_indices )
{
	// LOCALS
	// s32 								mg_index
	// const s32 						num_triangles
	// s32 								i
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5e7721 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN580") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5e7721 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN581") })
	// ******

	// FUNCTION BODY[0x5f83b0]: 43
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
	// <0x5f83b3>|0x003|+0x34e:'185'
	// <0x5f8701>|0x351|-0x331:'185'
	// <0>
	// <0x5f83d0>|0x020|+0x009:'187'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5f83d9>|0x029|+0x008:'196'
	// <0>
	// <0x5f83e1>|0x031|+0x31c:'198'
	// <0x5f86fd>|0x34d|-0x30c:'198'
	// <0>
	// <0x5f83f1>|0x041|+0x326:'200'
	// <0x5f8717>|0x367|-0x228:'200'
	// <0x5f84ef>|0x13f|+0x0fe:'201'
	// <0x5f85ed>|0x23d|+0x121:'202'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void speedtree_tree_component_branch::set_material( material_ptr mtl_ptr )
{
	// FUNCTION BODY[0x5f8730]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

// STATE[STUB]
void speedtree_tree_component_branch::set_default_material( )
{
	// FUNCTION BODY[0x5f8390]: 1
	// <0>
	// ******
}

// STATE[STUB]
void speedtree_tree_component_branch::render( lod_entry const* lod, renderer_context* context )
{
	// FUNCTION BODY[0x5f83a0]: 1
	// <0>
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
	// 	vostok::render::branch_vertex*
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
