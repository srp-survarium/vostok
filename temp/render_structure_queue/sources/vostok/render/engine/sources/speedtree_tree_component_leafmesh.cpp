////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\speedtree_tree_component_leafmesh.h"

namespace vostok {
namespace render {

// STATE[STUB]
explicit speedtree_tree_component_leafmesh::speedtree_tree_component_leafmesh( speedtree_tree& parent )
{
	// LOCALS
	// vector< leafmesh_vertex > 		total_vertices
	// SpeedTree::SIndexedTriangles const* lods
	// res_declaration_ptr 				decl
	// s32 								num_lods
	// vector< u16 > 					total_indices
	// s32 								lod_index
	// vector< leafmesh_vertex > 		vertices
	// vector< u16 > 					indices
	// u16 const* 						it
	// leafmesh_vertex const* 			it
	// ******

	// STATICS
	// static const float 				c_fUncompressWindScalar = <0x10000>;
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5e6991 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1182") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5e6991 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN1184") })
	// ******

	// FUNCTION BODY[0x5f7160]: 115
	// <0x5f7166>|0x006|+0x060:'54'
	// <0>
	// <0x5f71c6>|0x066|+0x00e:'56'
	// <0>
	// <1>
	// <2>
	// <0x5f71d4>|0x074|+0x006:'60'
	// <0x5f71da>|0x07a|+0x00a:'61'
	// <0>
	// <1>
	// <0x5f71e4>|0x084|+0x006:'64'
	// <0x5f71ea>|0x08a|+0x017:'65'
	// <0>
	// <0x5f7201>|0x0a1|+0x006:'67'
	// <0>
	// <1>
	// <0x5f7207>|0x0a7|+0x179:'70'
	// <0x5f7380>|0x220|+0x5e4:'70'
	// <0x5f7964>|0x804|-0x5dd:'70'
	// <0>
	// <0x5f7387>|0x227|+0x012:'72'
	// <0>
	// <0x5f7399>|0x239|+0x00f:'74'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5f73a8>|0x248|+0x002:'79'
	// <0x5f73aa>|0x24a|+0x016:'80'
	// <0>
	// <0x5f73c0>|0x260|+0x015:'82'
	// <0x5f73d5>|0x275|+0x018:'83'
	// <0>
	// <0x5f73ed>|0x28d|+0x023:'85'
	// <0x5f7410>|0x2b0|+0x577:'86'
	// <0x5f7987>|0x827|-0x448:'86'
	// <0>
	// <1>
	// <0x5f753f>|0x3df|+0x002:'89'
	// <0x5f7541>|0x3e1|+0x033:'90'
	// <0>
	// <1>
	// <0x5f7574>|0x414|+0x03c:'93'
	// <0>
	// <1>
	// <0x5f75b0>|0x450|+0x01c:'96'
	// <0>
	// <0x5f75cc>|0x46c|+0x0e8:'98'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5f76b4>|0x554|+0x007:'103'
	// <0x5f76bb>|0x55b|+0x015:'104'
	// <0x5f76d0>|0x570|-0x002:'104'
	// <0x5f76ce>|0x56e|+0x019:'105'
	// <0x5f76e7>|0x587|+0x002:'105'
	// <0x5f76e9>|0x589|+0x004:'106'
	// <0>
	// <1>
	// <0x5f76ed>|0x58d|+0x01c:'109'
	// <0>
	// <1>
	// <0x5f7709>|0x5a9|+0x014:'112'
	// <0>
	// <1>
	// <0x5f771d>|0x5bd|+0x015:'115'
	// <0>
	// <1>
	// <0x5f7732>|0x5d2|+0x014:'118'
	// <0>
	// <1>
	// <0x5f7746>|0x5e6|+0x021:'121'
	// <0>
	// <1>
	// <0x5f7767>|0x607|+0x006:'124'
	// <0>
	// <0x5f776d>|0x60d|+0x006:'126'
	// <0x5f7773>|0x613|+0x00a:'127'
	// <0>
	// <0x5f777d>|0x61d|+0x002:'129'
	// <0>
	// <0x5f777f>|0x61f|+0x005:'131'
	// <0x5f7784>|0x624|+0x010:'132'
	// <0>
	// <1>
	// <2>
	// <0x5f7794>|0x634|+0x00e:'136'
	// <0x5f77a2>|0x642|+0x00b:'137'
	// <0>
	// <1>
	// <0x5f77ad>|0x64d|+0x006:'140'
	// <0>
	// <1>
	// <2>
	// <0x5f77b3>|0x653|+0x029:'144'
	// <0>
	// <1>
	// <0x5f77dc>|0x67c|+0x151:'147'
	// <0x5f792d>|0x7cd|-0x702:'147'
	// <0x5f722b>|0x0cb|+0x5cd:'148'
	// <0x5f77f8>|0x698|+0x17f:'148'
	// <0x5f7977>|0x817|-0x046:'148'
	// <0x5f7931>|0x7d1|-0x702:'149'
	// <0>
	// <0x5f722f>|0x0cf|+0x004:'151'
	// <0x5f7233>|0x0d3|+0x007:'152'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x5f723a>|0x0da|+0x028:'157'
	// <0>
	// <0x5f7262>|0x102|+0x004:'159'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x5f7266>|0x106|+0x02a:'166'
	// <0>
	// <0x5f7290>|0x130|+0x039:'168'
	// ******
}

// STATE[STUB]
void speedtree_tree_component_leafmesh::init_index_buffer( SpeedTree::SIndexedTriangles const* lod, vector< u16 >& out_indices )
{
	// LOCALS
	// s32 								mg_index
	// SpeedTree::SDrawCallInfo const* 	info
	// s32 								i
	// ******

	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5e612c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN211") })
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x5e612c }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN212") })
	// ******

	// FUNCTION BODY[0x5f6fb0]: 33
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
	// <0x5f6fb3>|0x003|+0x150:'197'
	// <0x5f7103>|0x153|-0x133:'197'
	// <0>
	// <0x5f6fd0>|0x020|+0x007:'199'
	// <0>
	// <1>
	// <2>
	// <0x5f6fd7>|0x027|+0x01e:'203'
	// <0x5f6ff5>|0x045|+0x12d:'204'
	// <0x5f7122>|0x172|-0x009:'204'
	// <0>
	// ******
}

// STATE[STUB]
void speedtree_tree_component_leafmesh::set_material( material_ptr mtl_ptr )
{
	// FUNCTION BODY[0x5f7130]: 5
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// ******
}

// STATE[STUB]
void speedtree_tree_component_leafmesh::set_default_material( )
{
	// FUNCTION BODY[0x5f6f90]: 1
	// <0>
	// ******
}

// STATE[STUB]
void speedtree_tree_component_leafmesh::render( lod_entry const* lod, renderer_context* context )
{
	// FUNCTION BODY[0x5f6fa0]: 1
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
	// 	vostok::render::leafmesh_vertex*
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
