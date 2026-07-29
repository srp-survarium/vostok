////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\speedtree_tree_component_leafcard.h"

namespace vostok {
namespace render {

// STATE[STUB]
explicit speedtree_tree_component_leafcard::speedtree_tree_component_leafcard( speedtree_tree& parent )
{
	// LOCALS
	// vector< leafcard_vertex > 		total_vertices
	// SpeedTree::SLeafCards const* 	lods
	// res_declaration_ptr 				decl
	// s32 								num_lods
	// vector< u16 > 					total_indices
	// s32 								lod_index
	// vector< leafcard_vertex > 		vertices
	// vector< u16 > 					indices
	// u16 const* 						it
	// s32 								card_index
	// float const* 					card_pivot_point
	// float 							fMaxDistanceFromPivotPoint
	// const float[4][2] 				card_offsets
	// float const* 					card_dimensons
	// SpeedTree::Vec3 					pivot
	// const float 						c_fStartLodScale
	// leafcard_vertex const* 			it
	// ******

	// STATICS
	// static const float 				c_fUncompressWindScalar = <0x8142b4>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5e5f8b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1201") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5e5f8b }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1205") })
	// ******

	// FUNCTION BODY[0x5f64e0]: 155
	// <0>
	// <1>
	// <0x5f64e6>|0x006|+0x060:'54'
	// <0>
	// <0x5f6546>|0x066|+0x00e:'56'
	// <0>
	// <1>
	// <2>
	// <0x5f6554>|0x074|+0x006:'60'
	// <0x5f655a>|0x07a|+0x00d:'61'
	// <0>
	// <1>
	// <0x5f6567>|0x087|+0x006:'64'
	// <0x5f656d>|0x08d|+0x00f:'65'
	// <0>
	// <0x5f657c>|0x09c|+0x002:'67'
	// <0>
	// <1>
	// <0x5f657e>|0x09e|+0x192:'70'
	// <0x5f6710>|0x230|+0x007:'70'
	// <0>
	// <0x5f6717>|0x237|+0x011:'72'
	// <0>
	// <0x5f6728>|0x248|+0x00f:'74'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5f6737>|0x257|+0x034:'80'
	// <0>
	// <0x5f676b>|0x28b|+0x006:'82'
	// <0x5f6771>|0x291|+0x023:'83'
	// <0>
	// <0x5f6794>|0x2b4|+0x021:'85'
	// <0x5f67b5>|0x2d5|+0x7cc:'86'
	// <0x5f6f81>|0xaa1|-0x6b1:'86'
	// <0>
	// <1>
	// <0x5f68d0>|0x3f0|+0x002:'89'
	// <0x5f68d2>|0x3f2|+0x038:'90'
	// <0>
	// <1>
	// <2>
	// <0x5f690a>|0x42a|+0x4aa:'94'
	// <0x5f6db4>|0x8d4|-0x480:'94'
	// <0>
	// <0x5f6934>|0x454|+0x01c:'96'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x5f6950>|0x470|+0x091:'102'
	// <0>
	// <1>
	// <0x5f69e1>|0x501|+0x006:'105'
	// <0>
	// <0x5f69e7>|0x507|+0x02d:'107'
	// <0>
	// <0x5f6a14>|0x534|+0x016:'109'
	// <0x5f6a2a>|0x54a|+0x054:'110'
	// <0>
	// <1>
	// <0x5f6a7e>|0x59e|+0x2f1:'113'
	// <0x5f6d6f>|0x88f|-0x2cf:'113'
	// <0>
	// <1>
	// <0x5f6aa0>|0x5c0|+0x012:'116'
	// <0>
	// <0x5f6ab2>|0x5d2|+0x102:'118'
	// <0>
	// <1>
	// <0x5f6bb4>|0x6d4|+0x007:'121'
	// <0x5f6bbb>|0x6db|+0x01e:'122'
	// <0x5f6bd9>|0x6f9|-0x002:'122'
	// <0x5f6bd7>|0x6f7|+0x026:'123'
	// <0x5f6bfd>|0x71d|+0x002:'123'
	// <0x5f6bff>|0x71f|+0x005:'124'
	// <0>
	// <1>
	// <0x5f6c04>|0x724|+0x013:'127'
	// <0>
	// <1>
	// <0x5f6c17>|0x737|+0x019:'130'
	// <0>
	// <1>
	// <0x5f6c30>|0x750|+0x013:'133'
	// <0x5f6c43>|0x763|+0x004:'134'
	// <0>
	// <1>
	// <0x5f6c47>|0x767|+0x006:'137'
	// <0>
	// <0x5f6c4d>|0x76d|+0x006:'139'
	// <0x5f6c53>|0x773|+0x00c:'140'
	// <0>
	// <0x5f6c5f>|0x77f|+0x002:'142'
	// <0>
	// <0x5f6c61>|0x781|+0x005:'144'
	// <0x5f6c66>|0x786|+0x010:'145'
	// <0>
	// <1>
	// <2>
	// <0x5f6c76>|0x796|+0x00d:'149'
	// <0x5f6c83>|0x7a3|+0x011:'150'
	// <0x5f6c94>|0x7b4|+0x007:'151'
	// <0x5f6c9b>|0x7bb|+0x00d:'152'
	// <0x5f6ca8>|0x7c8|+0x008:'153'
	// <0x5f6cb0>|0x7d0|+0x002:'154'
	// <0x5f6cb2>|0x7d2|+0x009:'155'
	// <0>
	// <1>
	// <0x5f6cbb>|0x7db|+0x00a:'158'
	// <0x5f6cc5>|0x7e5|+0x005:'159'
	// <0>
	// <0x5f6cca>|0x7ea|+0x02e:'161'
	// <0x5f6cf8>|0x818|+0x015:'162'
	// <0>
	// <0x5f6d0d>|0x82d|+0x002:'164'
	// <0>
	// <0x5f6d0f>|0x82f|+0x01a:'166'
	// <0x5f6d29>|0x849|+0x017:'167'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <7>
	// <0x5f6d40>|0x860|+0x039:'176'
	// <0x5f6d79>|0x899|+0x005:'176'
	// <0>
	// <1>
	// <0x5f6d7e>|0x89e|+0x00c:'179'
	// <0>
	// <0x5f6d8a>|0x8aa|+0x02e:'181'
	// <0>
	// <1>
	// <2>
	// <0x5f6db8>|0x8d8|+0x171:'185'
	// <0x5f6f29>|0xa49|-0x157:'185'
	// <0x5f6dd2>|0x8f2|+0x1a3:'186'
	// <0x5f6f75>|0xa95|-0x048:'186'
	// <0x5f6f2d>|0xa4d|-0x981:'187'
	// <0>
	// <0x5f65ac>|0x0cc|+0x00c:'189'
	// <0x5f65b8>|0x0d8|+0x007:'190'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5f65bf>|0x0df|+0x028:'195'
	// <0>
	// <0x5f65e7>|0x107|+0x01b:'197'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5f6602>|0x122|+0x02b:'204'
	// <0>
	// <0x5f662d>|0x14d|+0x03a:'206'
	// ******
}

// STATE[STUB]
void speedtree_tree_component_leafcard::init_index_buffer(
	SpeedTree::SLeafCards const*	lod,
	vector< u16 >&					out_indices,
	u32								num_accumulated_vertices
)
{
	// LOCALS
	// s32 								mg_index
	// s32 								card_index
	// u16 								c_index
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5e54a3 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN793") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5e54a3 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN794") })
	// ******

	// FUNCTION BODY[0x5f5fd0]: 33
	// <0>
	// <1>
	// <0x5f5fdd>|0x00d|+0x012:'213'
	// <0>
	// <0x5f5fef>|0x01f|+0x48b:'215'
	// <0x5f647a>|0x4aa|-0x475:'215'
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
	// <0x5f6005>|0x035|+0x01b:'232'
	// <0>
	// <0x5f6020>|0x050|+0x008:'234'
	// <0>
	// <0x5f6028>|0x058|+0x471:'236'
	// <0x5f6499>|0x4c9|-0x374:'236'
	// <0x5f6125>|0x155|+0x0fd:'237'
	// <0x5f6222>|0x252|+0x0fd:'238'
	// <0x5f631f>|0x34f|+0x0f3:'239'
	// <0x5f6412>|0x442|+0x029:'240'
	// <0x5f643b>|0x46b|+0x054:'241'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void speedtree_tree_component_leafcard::set_material( material_ptr mtl_ptr )
{
	// FUNCTION BODY[0x5f64b0]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

// STATE[STUB]
void speedtree_tree_component_leafcard::set_default_material( )
{
	// FUNCTION BODY[0x5f5fb0]: 1
	// <0>
	// ******
}

// STATE[STUB]
void speedtree_tree_component_leafcard::render( lod_entry const* lod, renderer_context* context )
{
	// FUNCTION BODY[0x5f5fc0]: 1
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
	// 	vostok::render::geometry_batch*
	// 	iterator_type;

	// typedef
	// 	vostok::render::leafcard_vertex*
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
