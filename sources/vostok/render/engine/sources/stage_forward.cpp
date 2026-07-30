#include "pch.h"
#include "stage_forward.h"

namespace vostok {
namespace render {

bool stage_forward::is_effects_ready( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x60e2c0]
	return false;
}

stage_forward::~stage_forward( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x60e2f0]
}

void stage_forward::accumulate_local_reflections( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x60ea80]
}

void stage_forward::render_forward_models(
	vector<render_surface_instance*>&,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x60e5d0]
}

void stage_forward::render_opaque_models( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x60e380]
}

bool remove_model_if_not_forward_predicate::operator()(
	render_surface_instance const*
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x7ecb0]
	return false;
}

void stage_forward::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x60efa0]
}

} // namespace render
} // namespace vostok
