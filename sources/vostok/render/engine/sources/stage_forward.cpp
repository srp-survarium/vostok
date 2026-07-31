#include "pch.h"
#include "stage_forward.h"

#include <vostok/render/core/res_effect.h>

namespace vostok {
namespace render {

bool stage_forward::is_effects_ready( ) const
{
	// FUNCTION BODY[0x60e2c0]
	return m_opaque_geometry_mask_effect.c_ptr() != NULL;
}

stage_forward::~stage_forward( )
{
	// FUNCTION BODY[0x60e2f0]
}

void stage_forward::accumulate_local_reflections( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x60ea80]
	// claude@NOTE: no legacy ancestor - local reflections accumulation postdates the legacy corpus
}

void stage_forward::render_forward_models(
	vector<render_surface_instance*>&,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x60e5d0]
	// claude@NOTE: legacy stage_forward::execute inlined this loop; the shipped split
	// (render_forward_models/render_opaque_models + forward predicate) plus select_models
	// signature drift blocks a clean port - legacy loop kept in temp/render_legacy remainder
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
	// claude@NOTE: no legacy ancestor - predicate absent from the legacy stage_forward; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x7ecb0]
	return false;
}

void stage_forward::execute( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x60efa0]
	// claude@NOTE: legacy execute blocked - dropped m_dynamic_visuals member, select_models drift,
	// and the shipped stage_type (forward_base/forward_sky) split - matcher-phase
}

} // namespace render
} // namespace vostok
