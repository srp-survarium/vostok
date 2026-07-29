////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\environment_probe.h"

namespace vostok {
namespace render {

// STATE[STUB]
 environment_probe::environment_probe(
	collision::space_partitioning_tree*		tree,
	environment_probe_properties const&		properties,
	const u32								id
)
{
	// FUNCTION BODY[0x70b550]: 1
	// <0x70b5ea>|0x09a|+0x037:'32'
	// ******
}

// STATE[STUB]
 environment_probe::~environment_probe( )
{
	// FUNCTION BODY[0x70b060]: 1
	// <0x70b061>|0x001|+0x007:'37'
	// ******
}

// STATE[STUB]
void environment_probe::remove_collision( )
{
	// CALL SITE INFO
	// <0x70af64> -> void < unknown >( collision::object* )
	// ******

	// FUNCTION BODY[0x70af40]: 5
	// <0x70af45>|0x005|+0x013:'48'
	// <0x70af58>|0x018|+0x00e:'49'
	// <0>
	// <0x70af66>|0x026|+0x038:'51'
	// <0x70af9e>|0x05e|+0x03b:'52'
	// ******
}

// STATE[STUB]
bool environment_probe::is_occluded( ) const
{
	return false;

	// FUNCTION BODY[0x70b040]: 1
	// <0x70b040>|0x000|+0x01d:'57'
	// <0x70b05d>|0x01d|-0x001:'57'
	// <0x70b05c>|0x01c|+0x003:'58'
	// ******
}

// STATE[STUB]
u32 calc_mip_map_count( u32 width )
{
	return 0;

	// FUNCTION BODY[0x70afe0]: 1
	// <0x70afe9>|0x009|+0x049:'62'
	// ******
}

// STATE[STUB]
void environment_probe::set_properties( environment_probe_properties const& in_properties )
{
	// LOCALS
	// float4x4 						new_transform
	// fixed_string< 260 > 				depth_texture_name
	// float3 							probe_scale3
	// ******

	// CALL SITE INFO
	// <0x70b309> -> void < unknown >( collision::object*, float4x4 const& )
	// <0x70b39b> -> void < unknown >( collision::object*, float4x4 const& )
	// ******

	// FUNCTION BODY[0x70b0a0]: 49
	// <0x70b0b1>|0x011|+0x00e:'67'
	// <0>
	// <0x70b0bf>|0x01f|+0x011:'69'
	// <0>
	// <0x70b0d0>|0x030|+0x00d:'71'
	// <0>
	// <0x70b0dd>|0x03d|+0x01f:'73'
	// <0x70b0fc>|0x05c|+0x01f:'74'
	// <0>
	// <0x70b11b>|0x07b|+0x041:'76'
	// <0>
	// <0x70b15c>|0x0bc|+0x010:'78'
	// <0>
	// <0x70b16c>|0x0cc|+0x009:'80'
	// <0x70b175>|0x0d5|+0x037:'81'
	// <0>
	// <0x70b1ac>|0x10c|+0x048:'83'
	// <0>
	// <1>
	// <2>
	// <0x70b1f4>|0x154|+0x007:'87'
	// <0>
	// <1>
	// <2>
	// <0x70b1fb>|0x15b|+0x007:'91'
	// <0>
	// <0x70b202>|0x162|+0x06e:'93'
	// <0x70b270>|0x1d0|+0x031:'94'
	// <0x70b2a1>|0x201|+0x01c:'95'
	// <0x70b2bd>|0x21d|+0x038:'96'
	// <0x70b2f5>|0x255|+0x016:'97'
	// <0>
	// <0x70b30b>|0x26b|+0x005:'99'
	// <0>
	// <0x70b310>|0x270|+0x031:'101'
	// <0x70b341>|0x2a1|+0x033:'102'
	// <0x70b374>|0x2d4|+0x011:'103'
	// <0x70b385>|0x2e5|+0x018:'104'
	// <0>
	// <0x70b39d>|0x2fd|+0x0ca:'106'
	// <0>
	// <0x70b467>|0x3c7|+0x029:'108'
	// <0>
	// <0x70b490>|0x3f0|+0x00e:'110'
	// <0x70b49e>|0x3fe|+0x01e:'111'
	// <0>
	// <1>
	// <0x70b4bc>|0x41c|+0x071:'114'
	// <0x70b52d>|0x48d|+0x00e:'115'
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
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
