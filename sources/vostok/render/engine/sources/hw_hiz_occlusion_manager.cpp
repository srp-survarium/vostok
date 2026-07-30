#include "pch.h"
#include "hw_hiz_occlusion_manager.h"

namespace vostok {
namespace render {

hw_hiz_occlusion_manager::~hw_hiz_occlusion_manager( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7096b0]
}

bool hw_hiz_occlusion_manager::is_ready_effects( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x7095f0]
	return false;
}

void hw_hiz_occlusion_manager::process_culling(
	renderer_context*,
	float4 const*,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x70a7f0]
}

void hw_hiz_occlusion_manager::copy_scene_depth( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70a290]
}

void hw_hiz_occlusion_manager::render_occluders( renderer_context* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70a030]
}

void hw_hiz_occlusion_manager::render_debug(
	renderer_context*,
	float4 const*,
	pcbyte,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x709ed0]
}

void hw_hiz_occlusion_manager::downsample_occlusion_buffer( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x709930]
}

void hw_hiz_occlusion_manager::render_model_bounds(
	renderer_context*,
	float4 const*,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x70a4d0]
}

bool hw_hiz_occlusion_manager::quary_and_get_results_if_ready( pbyte, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x709600]
	return false;
}

void hw_hiz_occlusion_manager::check_culling_buffer( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70a320]
}

} // namespace render
} // namespace vostok
