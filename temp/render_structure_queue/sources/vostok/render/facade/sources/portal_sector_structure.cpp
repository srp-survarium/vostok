////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\facade\sources\portal_sector_structure.h"

namespace vostok {
namespace render {
namespace culling {

// STATE[STUB]
 portal_sector_structure::portal_sector_structure(
	memory::base_allocator*		arg_0 /* u32 portals_count */,
	u32							sectors_count,
	u32							arg_2
)
{
	// FUNCTION BODY[0x75e9c0]: 1
	// <0>
	// ******
}

// STATE[STUB]
 portal_sector_structure::~portal_sector_structure( )
{
	// FUNCTION BODY[0x75e8c0]: 7
	// <0x75e8c3>|0x003|+0x035:'38'
	// <0x75e8f8>|0x038|+0x02f:'39'
	// <0x75e927>|0x067|+0x00c:'40'
	// <0x75e933>|0x073|+0x020:'41'
	// <0x75e953>|0x093|+0x00c:'42'
	// <0x75e95f>|0x09f|+0x01e:'43'
	// <0x75e97d>|0x0bd|+0x01e:'44'
	// ******
}

// STATE[STUB]
u32 portal_sector_structure::get_sector_id( memory::base_allocator& allocator, float3 const& pos ) const
{
	// LOCALS
	// u32 								active_sector_id
	// vectora< collision::triangle_result > results
	// math::cuboid 					c
	// float 							min_dist
	// const float 						dist
	// ******

	// CALL SITE INFO
	// <0x75e6e7> -> bool < unknown >( u32, math::aabb const&, vectora< collision::triangle_result >& )
	// <0x75e753> -> bool < unknown >( collision::object const*, math::cuboid const&, vectora< collision::triangle_result >& ) const
	// ******

	return 0;

	// FUNCTION BODY[0x75e620]: 47
	// <0>
	// <0x75e62c>|0x00c|+0x008:'62'
	// <0x75e634>|0x014|+0x078:'63'
	// <0x75e6ac>|0x08c|+0x045:'64'
	// <0>
	// <1>
	// <0x75e6f1>|0x0d1|+0x009:'67'
	// <0x75e6fa>|0x0da|+0x01c:'68'
	// <0>
	// <0x75e716>|0x0f6|+0x00c:'70'
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
	// <0x75e722>|0x102|+0x01a:'83'
	// <0x75e73c>|0x11c|+0x021:'84'
	// <0>
	// <0x75e75d>|0x13d|+0x007:'86'
	// <0x75e764>|0x144|+0x014:'87'
	// <0>
	// <0x75e778>|0x158|+0x0ba:'89'
	// <0x75e832>|0x212|-0x062:'89'
	// <0>
	// <0x75e7d0>|0x1b0|+0x005:'91'
	// <0x75e7d5>|0x1b5|+0x004:'92'
	// <0>
	// <0x75e7d9>|0x1b9|+0x042:'94'
	// <0x75e81b>|0x1fb|+0x00b:'95'
	// <0>
	// <0x75e826>|0x206|+0x003:'97'
	// <0x75e829>|0x209|+0x00d:'98'
	// <0>
	// <1>
	// <0x75e836>|0x216|+0x007:'101'
	// <0x75e83d>|0x21d|+0x032:'102'
	// <0x75e86f>|0x24f|+0x022:'103'
	// <0x75e891>|0x271|+0x002:'103'
	// <0>
	// <1>
	// <2>
	// <0x75e893>|0x273|-0x00b:'107'
	// <0x75e888>|0x268|+0x023:'108'
	// ******
}

// STATE[STUB]
void portal_sector_structure::load( configs::binary_config_value* value_ptr )
{
	// LOCALS
	// configs::binary_config_value 	portals_cfg
	// u32* 							current_portal_ids
	// u32 								sector_id
	// float4x4 						identity_matrix
	// configs::binary_config_value 	sectors_cfg
	// configs::binary_config_value const* const portals_cfg_end
	// configs::binary_config_value const* const sectors_end
	// configs::binary_config_value const* sector_it
	// math::aabb 						sector_aabb
	// configs::binary_config_value 	portal_ids_cfg
	// spatial_sector 					new_sector
	// configs::binary_config_value const* const volumes_end
	// configs::binary_config_value 	volumes_cfg
	// const u32 						sector0
	// float3 							v0
	// configs::binary_config_value 	points_cfg
	// float3 							v2
	// const u32 						sector1
	// configs::binary_config_value 	sectors_cfg
	// float3 							v1
	// float3 							v3
	// ******

	// CALL SITE INFO
	// <0x75eeb7> -> void < unknown >( collision::object*, float4x4 const& )
	// <0x75f63b> -> void < unknown >( collision::object*, float4x4 const& )
	// ******

	// FUNCTION BODY[0x75ebf0]: 74
	// <0>
	// <1>
	// <0x75ec03>|0x013|+0x035:'114'
	// <0x75ec38>|0x048|+0x00d:'115'
	// <0x75ec45>|0x055|+0x02b:'116'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x75ec70>|0x080|+0x01e:'121'
	// <0x75ec8e>|0x09e|+0x02c:'122'
	// <0x75ecba>|0x0ca|+0x011:'123'
	// <0x75eccb>|0x0db|+0x012:'124'
	// <0>
	// <1>
	// <0x75ecdd>|0x0ed|+0x359:'127'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x75f036>|0x446|-0x280:'132'
	// <0>
	// <0x75edb6>|0x1c6|+0x004:'134'
	// <0>
	// <0x75edba>|0x1ca|+0x00a:'136'
	// <0x75edc4>|0x1d4|+0x0dd:'137'
	// <0x75eea1>|0x2b1|+0x018:'138'
	// <0x75eeb9>|0x2c9|+0x008:'139'
	// <0>
	// <0x75eec1>|0x2d1|+0x140:'141'
	// <0>
	// <0x75f001>|0x411|+0x002:'143'
	// <0>
	// <0x75f003>|0x413|+0x024:'145'
	// <0x75f027>|0x437|+0x017:'146'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x75f03e>|0x44e|+0x035:'151'
	// <0>
	// <0x75f073>|0x483|+0x019:'153'
	// <0x75f08c>|0x49c|+0x004:'154'
	// <0>
	// <0x75f090>|0x4a0|+0x007:'156'
	// <0x75f097>|0x4a7|+0x00b:'157'
	// <0>
	// <0x75f0a2>|0x4b2|-0x3c2:'159'
	// <0x75ece0>|0x0f0|+0x404:'160'
	// <0x75f0e4>|0x4f4|+0x042:'160'
	// <0>
	// <1>
	// <0x75f126>|0x536|+0x019:'163'
	// <0x75f13f>|0x54f|+0x011:'164'
	// <0>
	// <0x75f150>|0x560|+0x02c:'166'
	// <0x75f17c>|0x58c|+0x019:'167'
	// <0x75f195>|0x5a5|+0x010:'168'
	// <0x75f1a5>|0x5b5|+0x003:'169'
	// <0x75f1a8>|0x5b8|+0x020:'170'
	// <0>
	// <0x75f1c8>|0x5d8|+0x029:'172'
	// <0>
	// <1>
	// <2>
	// <0x75f1f1>|0x601|+0x24e:'176'
	// <0>
	// <0x75f43f>|0x84f|+0x13c:'178'
	// <0>
	// <1>
	// <0x75f57b>|0x98b|+0x0aa:'181'
	// <0x75f625>|0xa35|+0x025:'182'
	// <0>
	// <1>
	// <0x75f64a>|0xa5a|+0x006:'185'
	// ******
}

// STATE[STUB]
void portal_sector_structure::sort_portal_ids( float const* distances )
{
	// FUNCTION BODY[0x75e5d0]: 5
	// <0x75e5d1>|0x001|+0x00c:'225'
	// <0x75e5dd>|0x00d|+0x013:'226'
	// <0>
	// <0x75e5f0>|0x020|+0x01e:'228'
	// <0>
	// ******
}

// STATE[STUB]
void portal_sector_structure::initialize_portals_geometry( )
{
	// LOCALS
	// buffer_vector< float3 > 			vertices
	// portal const* const 				portals_end
	// const u32 						indices_count
	// portal const* 					it
	// ******

	// FUNCTION BODY[0x75ea80]: 19
	// <0x75ea88>|0x008|+0x024:'234'
	// <0x75eaac>|0x02c|+0x00d:'235'
	// <0x75eab9>|0x039|+0x02d:'236'
	// <0x75eae6>|0x066|+0x009:'237'
	// <0x75eaef>|0x06f|+0x006:'238'
	// <0x75eaf5>|0x075|+0x01e:'239'
	// <0>
	// <0x75eb13>|0x093|+0x019:'241'
	// <0x75eb2c>|0x0ac|+0x01c:'242'
	// <0x75eb48>|0x0c8|+0x01d:'243'
	// <0x75eb65>|0x0e5|+0x02f:'244'
	// <0>
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <6>
	// <0x75eb94>|0x114|+0x040:'252'
	// <0x75ebd4>|0x154|-0x00c:'252'
	// <0x75ebc8>|0x148|+0x014:'253'
	// ******
}

// STATE[STUB]
void portal_sector_structure::update_portals_visability( math::frustum const& f, pcbyte oclusion_results )
{
	// LOCALS
	// vectora< collision::triangle_result > triangles
	// ******

	// CALL SITE INFO
	// <0x75e56e> -> bool < unknown >( collision::object const*, math::cuboid const&, vectora< collision::triangle_result >& ) const
	// ******

	// FUNCTION BODY[0x75e520]: 15
	// <0x75e520>|0x000|+0x006:'257'
	// <0x75e526>|0x006|+0x012:'258'
	// <0>
	// <0x75e538>|0x018|+0x00a:'260'
	// <0>
	// <0x75e542>|0x022|+0x006:'262'
	// <0x75e548>|0x028|+0x02a:'263'
	// <0>
	// <1>
	// <0x75e572>|0x052|+0x033:'266'
	// <0x75e5a5>|0x085|-0x023:'266'
	// <0>
	// <0x75e582>|0x062|+0x003:'268'
	// <0x75e585>|0x065|+0x024:'269'
	// <0>
	// <1>
	// ******
}

	// TYPEDEFS
	// typedef
	// 	vostok::collision::triangle_result*
	// 	iterator_type;

	// ******

} // namespace culling
} // namespace render
} // namespace vostok
