#include "pch.h"
// claude@NOTE: legacy-harvest disposition: no temp/render_legacy ancestor (hierarchical-Z occlusion is new-in-target) - matcher-phase work.
#include "hw_hiz_point_list.h"

namespace vostok {
namespace render {

struct vertex_struct {
	vertex_struct( ) { }

	float4 bound;
	float2 pixel;
};

hw_hiz_point_list::hw_hiz_point_list( ) :
	m_num_points( 0 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7487d0]
}

void hw_hiz_point_list::initialize( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x748680]
}

void hw_hiz_point_list::set_points( float4 const*, u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7485b0]
}

void hw_hiz_point_list::render( u32 )
{
	// STATE[STUB]
	// FUNCTION BODY[0x7486d0]
}

} // namespace render
} // namespace vostok
