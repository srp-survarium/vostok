////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\engine\sources\streaming.h"

namespace vostok {
namespace render {

// STATE[STUB]
float calculate_streaming_texture_factor(
	float3 const*		positions,
	float2 const*		uvs,
	const u32			num_vertices,
	const u32			vertex_stride,
	u16 const*			indices,
	const u32			num_indices
)
{
	// LOCALS
	// float 							streaming_factor
	// vector< float > 					texel_ratios
	// const float 						t1
	// const float 						l1
	// ******

	return 0.0f;

	// FUNCTION BODY[0x779a80]: 36
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x779ad2>|0x052|-0x049:'24'
	// <0x779a89>|0x009|+0x028:'25'
	// <0>
	// <0x779ab1>|0x031|+0x009:'27'
	// <0>
	// <0x779aba>|0x03a|+0x01c:'29'
	// <0>
	// <0x779ad6>|0x056|+0x007:'31'
	// <0x779add>|0x05d|+0x006:'32'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x779ae3>|0x063|+0x007:'37'
	// <0>
	// <0x779aea>|0x06a|+0x057:'39'
	// <0>
	// <1>
	// <0x779b41>|0x0c1|+0x066:'42'
	// <0x779ba7>|0x127|+0x05d:'43'
	// <0>
	// <0x779c04>|0x184|+0x030:'45'
	// <0x779c34>|0x1b4|+0x054:'46'
	// <0>
	// <1>
	// <0x779c88>|0x208|+0x017:'49'
	// <0>
	// <0x779c9f>|0x21f|+0x03c:'51'
	// <0x779cdb>|0x25b|+0x04c:'52'
	// <0>
	// <1>
	// <0x779d27>|0x2a7|+0x01f:'55'
	// ******
}

// STATE[STUB]
s32 calculate_needed_texture_mip_levels(
	float4x4 const&			projection_matrix,
	float3 const&			viewer_position,
	math::sphere const&		object_sphere,
	const u32				screen_size_x,
	const u32				screen_size_y,
	const float				factor,
	float&					out_distance
)
{
	// LOCALS
	// const float 						distance
	// float 							screen_space_size_x
	// float 							screen_space_size_y
	// ******

	// STATICS
	// static const u32 				max_texture_size = <0x10000>;
	// static const u32 				min_streamed_in_mips = <0x10000>;
	// static const u32 				max_streamed_in_mips = <0x10000>;
	// static u32 						quality_index = <0x4c265bc>;
	// ******

	return 0;

	// FUNCTION BODY[0x7798d0]: 24
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x7798d9>|0x009|+0x059:'70'
	// <0>
	// <0x779932>|0x062|+0x00c:'72'
	// <0>
	// <1>
	// <2>
	// <0x77993e>|0x06e|+0x018:'76'
	// <0>
	// <0x779956>|0x086|+0x02d:'78'
	// <0x779983>|0x0b3|+0x013:'79'
	// <0x779996>|0x0c6|+0x020:'80'
	// <0>
	// <0x7799b6>|0x0e6|+0x047:'82'
	// <0x7799fd>|0x12d|+0x04c:'83'
	// <0x779a49>|0x179|+0x014:'84'
	// <0x779a5d>|0x18d|-0x00a:'84'
	// <0>
	// <1>
	// <2>
	// <3>
	// <0x779a53>|0x183|+0x014:'89'
	// <0x779a67>|0x197|-0x00e:'89'
	// <0x779a59>|0x189|+0x014:'90'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	float*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
