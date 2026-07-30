#include "pch.h"
#include "speedtree_forest.h"

namespace vostok {
namespace render {

speedtree_forest::speedtree_forest( ) :
	m_forest( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6024b0]
}

speedtree_forest::~speedtree_forest( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6023c0]
}

void speedtree_forest::set_wind_direction( float3 const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x601590]
}

void speedtree_forest::set_wind_strength( float )
{
	// STATE[STUB]
	// FUNCTION BODY[0x601570]
}

void speedtree_forest::tick( renderer_context* )
{
	// STATE[STUB]
	// FUNCTION BODY[0x601650]
}

float4x4 speedtree_forest::get_instance_transform( SpeedTree::CInstance const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6015d0]
	return float4x4( );
}

void speedtree_forest::get_visible_tree_components(
	renderer_context*,
	float3 const&,
	bool,
	tree_render_info_array_type&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x601ad0]
}

void speedtree_forest::populate_forest( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x602630]
}

float4x4 fix_view_matrix( float4x4 const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6016f0]
	return float4x4( );
}

void speedtree_forest::cull_and_compute_lod( renderer_context*, float3 const&, bool )
{
	// STATE[STUB]
	// FUNCTION BODY[0x6019a0]
}

void speedtree_forest::add_instance( speedtree_instance_ptr, float4x4 const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x602230]
}

void speedtree_forest::remove_instance( speedtree_instance_ptr )
{
	// STATE[STUB]
	// FUNCTION BODY[0x601fe0]
}

void speedtree_forest::set_transform( speedtree_instance_ptr, float4x4 const& )
{
	// STATE[STUB]
	// FUNCTION BODY[0x601ef0]
}

} // namespace render
} // namespace vostok
