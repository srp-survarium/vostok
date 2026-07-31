#include "pch.h"
#include "stage_decals_accumulate.h"

#include "decal_instance.h"

#include <vostok/render/core/res_effect.h>

namespace vostok {
namespace render {

stage_decals_accumulate::~stage_decals_accumulate( )
{
	// FUNCTION BODY[0x62c490]
}

bool stage_decals_accumulate::is_effects_ready( ) const
{
	// FUNCTION BODY[0x62c470]
	return m_opaque_geometry_mask_effect.c_ptr() != NULL;
}

void stage_decals_accumulate::debug_render( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62ca90]
	// claude@NOTE: no legacy ancestor - decal debug rendering postdates the legacy corpus
}

void stage_decals_accumulate::execute_disabled( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62c4f0]
}

void stage_decals_accumulate::execute( )
{
	struct sort_by_priority_predicate {
		bool operator()(
			decal_instance const* const,
			decal_instance const* const
		)
		{
			// claude@NOTE: no legacy ancestor - legacy stage_decals_accumulate has no sorting predicate; matcher-phase work.
			// STATE[STUB]
			// FUNCTION BODY[0x62c450]
			return false;
		}
	};

	sort_by_priority_predicate predicate;
	(void)predicate;

	// STATE[STUB]
	// FUNCTION BODY[0x62c5d0]
	// claude@NOTE: legacy execute blocked - shipped execute sorts decals by priority (local
	// predicate) over a diverged scene decal store and adds m_apply_decal_effect; legacy loop kept
	// in temp/render_legacy/engine/sources/stage_decals_accumulate.cpp as the remainder
}

} // namespace render
} // namespace vostok
