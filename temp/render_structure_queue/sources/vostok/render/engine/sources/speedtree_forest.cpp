////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\speedtree_forest.h"

namespace vostok {
namespace render {

// STATE[STUB]
void `dynamic atexit destructor for 's_speedtree_lod_index''( )
{
	// FUNCTION BODY[0x7ef6d0]
	// ******
}

// STATE[STUB]
 speedtree_forest::speedtree_forest( )
{
	// CALL SITE INFO
	// <0x6025a0> -> void* < unknown >( u32 )
	// ******

	// FUNCTION BODY[0x6024b0]: 19
	// <0x60258c>|0x0dc|+0x035:'47'
	// <0>
	// <1>
	// <0x6025c1>|0x111|+0x036:'50'
	// <0x6025f7>|0x147|+0x00b:'51'
	// <0x602602>|0x152|+0x00b:'52'
	// <0x60260d>|0x15d|+0x00e:'53'
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
	// ******
}

// STATE[STUB]
 speedtree_forest::~speedtree_forest( )
{
	// CALL SITE INFO
	// <0x6023d7> -> void* < unknown >( u32 )
	// <0x6023ef> -> void < unknown >( void* )
	// ******

	// FUNCTION BODY[0x6023c0]: 1
	// <0x6023c5>|0x005|+0x040:'70'
	// ******
}

// STATE[STUB]
void speedtree_forest::set_wind_direction( float3 const& wind_direction )
{
	// FUNCTION BODY[0x601590]: 1
	// <0x601593>|0x003|+0x02e:'75'
	// ******
}

// STATE[STUB]
void speedtree_forest::set_wind_strength( float wind_strength )
{
	// FUNCTION BODY[0x601570]: 1
	// <0x601570>|0x000|+0x013:'80'
	// ******
}

// STATE[STUB]
void speedtree_forest::tick( renderer_context* context )
{
	// FUNCTION BODY[0x601650]: 5
	// <0x60165b>|0x00b|+0x042:'127'
	// <0x60169d>|0x04d|+0x017:'128'
	// <0x6016b4>|0x064|+0x00f:'129'
	// <0x6016c3>|0x073|+0x00e:'130'
	// <0x6016d1>|0x081|+0x00e:'131'
	// ******
}

// STATE[STUB]
float4x4 speedtree_forest::get_instance_transform( SpeedTree::CInstance const& in_instance )
{
	return vostok::math::float4x4();

	// FUNCTION BODY[0x6015d0]: 8
	// <0x6015d0>|0x000|+0x021:'136'
	// <0>
	// <0x6015f1>|0x021|+0x002:'138'
	// <0>
	// <0x6015f3>|0x023|+0x054:'140'
	// <0x601647>|0x077|-0x020:'141'
	// <0>
	// <0x601627>|0x057|+0x014:'143'
	// ******
}

// STATE[STUB]
void speedtree_forest::get_visible_tree_components(
	renderer_context*		context,
	float3 const&			lod_reference_point,
	bool					sort_result,
	vector< speedtree_forest::tree_render_info >&	out_tree_render_info_array
)
{
	// LOCALS
	// s32 								base_tree_index
	// SpeedTree::CArray< SpeedTree::SInstanceLod, 1 > const* instance_lods
	// speedtree_tree* 					base_tree
	// ******

	// FUNCTION BODY[0x601ad0]: 139
	// <0>
	// <0x601ad9>|0x009|+0x00e:'152'
	// <0>
	// <1>
	// <0x601ae7>|0x017|+0x3de:'155'
	// <0x601ec5>|0x3f5|-0x3c5:'155'
	// <0>
	// <0x601b00>|0x030|+0x019:'157'
	// <0>
	// <1>
	// <0x601b19>|0x049|+0x02b:'160'
	// <0>
	// <0x601b44>|0x074|+0x043:'162'
	// <0>
	// <1>
	// <0x601b87>|0x0b7|+0x00b:'165'
	// <0>
	// <0x601b92>|0x0c2|+0x027:'167'
	// <0>
	// <0x601bb9>|0x0e9|+0x017:'169'
	// <0>
	// <0x601bd0>|0x100|-0x00a:'171'
	// <0>
	// <1>
	// <2>
	// <0x601bc6>|0x0f6|+0x00e:'175'
	// <0x601bd4>|0x104|+0x06f:'175'
	// <0x601c43>|0x173|-0x06c:'175'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x601bd7>|0x107|+0x00b:'180'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x601be2>|0x112|+0x01f:'186'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x601c01>|0x131|+0x01d:'191'
	// <0x601c1e>|0x14e|+0x029:'192'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x601c47>|0x177|+0x023:'197'
	// <0>
	// <0x601c6a>|0x19a|+0x00f:'199'
	// <0x601c79>|0x1a9|+0x00d:'200'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x601c86>|0x1b6|+0x07d:'206'
	// <0x601d03>|0x233|-0x070:'206'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x601c93>|0x1c3|+0x00b:'211'
	// <0>
	// <1>
	// <0x601c9e>|0x1ce|+0x002:'214'
	// <0>
	// <1>
	// <0x601ca0>|0x1d0|+0x01d:'217'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x601cbd>|0x1ed|+0x01b:'222'
	// <0x601cd8>|0x208|+0x02f:'223'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x601d07>|0x237|+0x023:'228'
	// <0>
	// <0x601d2a>|0x25a|+0x00f:'230'
	// <0x601d39>|0x269|+0x00d:'231'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x601d46>|0x276|+0x07d:'237'
	// <0x601dc3>|0x2f3|-0x070:'237'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x601d53>|0x283|+0x00b:'242'
	// <0>
	// <1>
	// <0x601d5e>|0x28e|+0x002:'245'
	// <0>
	// <1>
	// <0x601d60>|0x290|+0x01d:'248'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x601d7d>|0x2ad|+0x01b:'253'
	// <0x601d98>|0x2c8|+0x02f:'254'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x601dc7>|0x2f7|+0x023:'259'
	// <0>
	// <0x601dea>|0x31a|+0x00f:'261'
	// <0x601df9>|0x329|+0x027:'262'
	// <0>
	// <0x601e20>|0x350|-0x014:'264'
	// <0>
	// <1>
	// <2>
	// <0x601e0c>|0x33c|+0x018:'268'
	// <0x601e24>|0x354|+0x09b:'268'
	// <0x601ebf>|0x3ef|-0x098:'268'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x601e27>|0x357|+0x00b:'273'
	// <0>
	// <1>
	// <0x601e32>|0x362|+0x002:'276'
	// <0>
	// <1>
	// <0x601e34>|0x364|+0x01d:'279'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x601e51>|0x381|+0x01d:'284'
	// <0x601e6e>|0x39e|+0x06c:'285'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
void speedtree_forest::populate_forest( )
{
	// LOCALS
	// SpeedTree::CForest::SCompletePopulation sWholePop
	// SpeedTree::CForest::SPopulationStats sStats
	// SpeedTree::CArray< SpeedTree::CCore*, 1 > base_trees
	// SpeedTree::CArray< SpeedTree::CArray< SpeedTree::CInstance, 1 >, 1 > base_tree_instances
	// SpeedTree::CCore* 				base_tree
	// ******

	// CALL SITE INFO
	// <0x6029ca> -> void < unknown >( void* )
	// <0x602a0a> -> void < unknown >( void* )
	// <0x602a9a> -> void < unknown >( void* )
	// ******

	// FUNCTION BODY[0x602630]: 72
	// <0x602640>|0x010|+0x012:'294'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x602652>|0x022|+0x051:'299'
	// <0x6026a3>|0x073|+0x019:'300'
	// <0>
	// <0x6026bc>|0x08c|+0x014:'302'
	// <0>
	// <0x6026d0>|0x0a0|+0x002:'304'
	// <0x6026d2>|0x0a2|+0x013:'305'
	// <0x6026e5>|0x0b5|+0x00e:'306'
	// <0x6026f3>|0x0c3|+0x055:'307'
	// <0>
	// <1>
	// <2>
	// <0x602748>|0x118|+0x018:'311'
	// <0>
	// <1>
	// <2>
	// <0x602760>|0x130|+0x00e:'315'
	// <0>
	// <0x60276e>|0x13e|+0x002:'317'
	// <0x602770>|0x140|+0x00a:'318'
	// <0>
	// <0x60277a>|0x14a|+0x01b:'320'
	// <0>
	// <1>
	// <2>
	// <0x602795>|0x165|+0x010:'324'
	// <0>
	// <1>
	// <0x6027a5>|0x175|+0x0a4:'327'
	// <0>
	// <0x602849>|0x219|+0x011:'329'
	// <0x60285a>|0x22a|+0x011:'330'
	// <0>
	// <0x60286b>|0x23b|+0x012:'332'
	// <0>
	// <0x60287d>|0x24d|+0x00b:'334'
	// <0x602888>|0x258|+0x019:'335'
	// <0>
	// <1>
	// <0x6028a1>|0x271|-0x00e:'338'
	// <0x602893>|0x263|+0x012:'339'
	// <0>
	// <0x6028a5>|0x275|+0x002:'341'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6028a7>|0x277|+0x009:'349'
	// <0>
	// <1>
	// <0x6028b0>|0x280|+0x00a:'352'
	// <0>
	// <1>
	// <2>
	// <0x6028ba>|0x28a|+0x006:'356'
	// <0x6028c0>|0x290|+0x00b:'357'
	// <0>
	// <1>
	// <2>
	// <0x6028cb>|0x29b|+0x00b:'361'
	// <0>
	// <1>
	// <0x6028d6>|0x2a6|+0x06a:'364'
	// <0x602940>|0x310|+0x01a:'365'
	// ******
}

// STATE[STUB]
float4x4 fix_view_matrix( float4x4 const& in_view_matrix )
{
	return vostok::math::float4x4();

	// FUNCTION BODY[0x6016f0]: 27
	// <0>
	// <1>
	// <2>
	// <0x6016f1>|0x001|+0x0a5:'373'
	// <0x601796>|0x0a6|+0x065:'374'
	// <0x6017fb>|0x10b|+0x01c:'375'
	// <0>
	// <1>
	// <2>
	// <0x601817>|0x127|+0x01f:'379'
	// <0>
	// <0x601836>|0x146|+0x031:'381'
	// <0x601867>|0x177|+0x01c:'382'
	// <0x601883>|0x193|+0x01c:'383'
	// <0>
	// <0x60189f>|0x1af|+0x017:'385'
	// <0x6018b6>|0x1c6|+0x01c:'386'
	// <0x6018d2>|0x1e2|+0x01c:'387'
	// <0>
	// <0x6018ee>|0x1fe|+0x017:'389'
	// <0x601905>|0x215|+0x01c:'390'
	// <0x601921>|0x231|+0x01c:'391'
	// <0>
	// <0x60193d>|0x24d|+0x017:'393'
	// <0x601954>|0x264|+0x01c:'394'
	// <0x601970>|0x280|+0x01e:'395'
	// <0x60198e>|0x29e|-0x005:'395'
	// <0x601989>|0x299|+0x00f:'396'
	// <0x601998>|0x2a8|-0x00c:'396'
	// <0x60198c>|0x29c|+0x00f:'397'
	// ******
}

// STATE[STUB]
void speedtree_forest::cull_and_compute_lod( renderer_context* context, float3 const& lod_reference_point, bool sort_result )
{
	// CALL SITE INFO
	// <0x601ac2> -> void < unknown >( SpeedTree::CView const&, SpeedTree::SForestCullResults&, bool )
	// ******

	// FUNCTION BODY[0x6019a0]: 14
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x6019b2>|0x012|+0x0d2:'408'
	// <0>
	// <0x601a84>|0x0e4|+0x02c:'410'
	// <0x601ab0>|0x110|+0x014:'411'
	// <0>
	// <1>
	// <2>
	// ******
}

// STATE[STUB]
void speedtree_forest::add_instance( speedtree_instance_ptr st_instance_ptr, float4x4 const& transform )
{
	// LOCALS
	// SpeedTree::SLodProfile 			sLodProfile
	// ******

	// FUNCTION BODY[0x602230]: 21
	// <0x602230>|0x000|+0x015:'419'
	// <0x602245>|0x015|+0x00e:'420'
	// <0>
	// <0x602253>|0x023|+0x014:'422'
	// <0>
	// <0x602267>|0x037|+0x00c:'424'
	// <0>
	// <0x602273>|0x043|+0x049:'426'
	// <0>
	// <0x6022bc>|0x08c|+0x030:'428'
	// <0x6022ec>|0x0bc|+0x01e:'429'
	// <0x60230a>|0x0da|+0x01a:'430'
	// <0>
	// <1>
	// <0x602324>|0x0f4|+0x04f:'433'
	// <0>
	// <0x602373>|0x143|+0x00f:'435'
	// <0>
	// <1>
	// <2>
	// <3>
	// ******
}

// STATE[STUB]
void speedtree_forest::remove_instance( speedtree_instance_ptr st_instance_ptr )
{
	// LOCALS
	// SpeedTree::CInstance 			speedtree_instance
	// speedtree_instance_ptr* 			found
	// SpeedTree::CArray< SpeedTree::CInstance, 1 > instances_of_tree
	// ******

	// FUNCTION BODY[0x601fe0]: 43
	// <0>
	// <0x601fe3>|0x003|+0x022:'445'
	// <0x602005>|0x025|+0x006:'446'
	// <0>
	// <0x60200b>|0x02b|+0x020:'448'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x60202b>|0x04b|-0x038:'455'
	// <0>
	// <1>
	// <2>
	// <0x601ff3>|0x013|+0x065:'459'
	// <0x602058>|0x078|+0x01f:'459'
	// <0x602077>|0x097|+0x008:'460'
	// <0>
	// <1>
	// <0x60207f>|0x09f|+0x02c:'463'
	// <0>
	// <0x6020ab>|0x0cb|+0x006:'465'
	// <0>
	// <0x6020b1>|0x0d1|+0x00c:'467'
	// <0>
	// <0x6020bd>|0x0dd|+0x01b:'469'
	// <0x6020d8>|0x0f8|+0x004:'470'
	// <0x6020dc>|0x0fc|+0x009:'471'
	// <0>
	// <1>
	// <0x6020e5>|0x105|+0x00d:'474'
	// <0x6020f2>|0x112|+0x034:'475'
	// <0>
	// <1>
	// <0x602126>|0x146|+0x0c2:'478'
	// <0x6021e8>|0x208|-0x0ed:'479'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x6020fb>|0x11b|+0x005:'486'
	// <0x602100>|0x120|+0x117:'487'
	// ******
}

// STATE[STUB]
void speedtree_forest::set_transform( speedtree_instance_ptr st_instance_ptr, float4x4 const& transform )
{
	// LOCALS
	// SpeedTree::CInstance 			new_speedtree_instance
	// SpeedTree::CInstance 			old_speedtree_instance
	// ******

	// FUNCTION BODY[0x601ef0]: 19
	// <0>
	// <0x601ef0>|0x000|+0x00f:'492'
	// <0x601eff>|0x00f|+0x00e:'493'
	// <0>
	// <0x601f0d>|0x01d|+0x03a:'495'
	// <0x601f47>|0x057|+0x00b:'496'
	// <0x601f52>|0x062|+0x030:'497'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <0x601f82>|0x092|+0x01e:'503'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
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
	// 	vostok::render::speedtree_forest::tree_render_info*
	// 	iterator_type;

	// typedef
	// 	vostok::render::speedtree_instance_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::speedtree_tree_base_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
