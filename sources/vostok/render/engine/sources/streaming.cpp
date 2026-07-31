#include "pch.h"
// claude@NOTE: legacy-harvest disposition: no temp/render_legacy ancestor (texture streaming is new-in-target) - matcher-phase work.
#include "streaming.h"

namespace vostok {
namespace render {

float calculate_streaming_texture_factor(
	float3 const*,
	float2 const*,
	u32,
	u32,
	u16 const*,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x779a80]
	return 0.f;
}

s32 calculate_needed_texture_mip_levels(
	float4x4 const&,
	float3 const&,
	math::sphere const&,
	u32,
	u32,
	float,
	float& out_distance
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x7798d0]
	out_distance = 0.f;
	return 0;
}

} // namespace render
} // namespace vostok
