#include "pch.h"
// claude@NOTE: legacy-harvest disposition: no temp/render_legacy ancestor (batched_geometry family is new-in-target) - matcher-phase work.
#include "shadow_batched_geometry.h"

#include "render_surface.h"

namespace vostok {
namespace render {

shadow_batched_geometry::shadow_batched_geometry( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x70b890]
}

void shadow_batched_geometry::add_vertex(
	batched_vertex_source const&,
	float3 const&
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x70b650]
}

void shadow_batched_geometry::build(
	vector<render_model_instance_impl_ptr>&
)
{
	struct surface_set {
		surface_set( ) :
			surface( 0 )
		{
		}

		float4x4		transform;
		render_surface*	surface;
	};

	struct sort_predicate {
		bool operator()( surface_set const&, surface_set const& ) const
		{
			// STATE[STUB]
			// FUNCTION BODY[0x70b630]
			return false;
		}
	};

	typedef vector<surface_set> surfaces_type;

	(void)sizeof( surfaces_type );
	(void)sizeof( sort_predicate );

	// STATE[STUB]
	// FUNCTION BODY[0x70b6f0]
}

} // namespace render
} // namespace vostok
