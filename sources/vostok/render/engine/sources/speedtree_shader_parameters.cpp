#include "pch.h"
#include "speedtree_shader_parameters.h"

namespace vostok {
namespace render {

speedtree_wind_parameters::speedtree_wind_parameters( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x779210]
}

void speedtree_wind_parameters::set( SpeedTree::CWind const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x778320]
}

speedtree_billboard_parameters::speedtree_billboard_parameters( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x778f90]
}

void speedtree_billboard_parameters::set( renderer_context*, speedtree_tree_component* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x778a80]
}

void speedtree_billboard_parameters::set_billboard_tangents( float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x778870]
}

speedtree_common_parameters::speedtree_common_parameters( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x778e70]
}

void speedtree_common_parameters::set( renderer_context*, speedtree_tree_component*, float3 const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x778160]
}

speedtree_tree_parameters::speedtree_tree_parameters( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x778db0]
}

void speedtree_tree_parameters::set(
	speedtree_tree_component*,
	SpeedTree::CInstance const*,
	SpeedTree::SInstanceLod const*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x778030]
}

} // namespace render
} // namespace vostok
