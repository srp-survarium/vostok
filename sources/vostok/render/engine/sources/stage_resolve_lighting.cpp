#include "pch.h"
#include "stage_resolve_lighting.h"

namespace vostok {
namespace render {

bool stage_resolve_lighting::is_effects_ready( ) const
{
	// claude@NOTE: no legacy ancestor - no same-stem legacy; the held dx9 stage_combine reference has no such member; matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x613990]
	return false;
}

void stage_resolve_lighting::render_models(
	vector<render_surface_instance*>&,
	u32&
)
{
	// claude@NOTE: no legacy ancestor - no per-material model pass in any legacy generation (dx9 stage_combine draws a fullscreen quad only); matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x6139a0]
}

void stage_resolve_lighting::execute( )
{
	// claude@NOTE: no legacy ancestor - no same-generation ancestor - held dx9/sources/stage_combine.cpp execute is the D3D9 reading reference only (shader_manager/hw_wrapper era); matcher-phase rewrite.
	// STATE[STUB]
	// FUNCTION BODY[0x613c90]
}

} // namespace render
} // namespace vostok
