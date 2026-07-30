#include "pch.h"
#include "stage_decals_accumulate.h"

#include "decal_instance.h"

namespace vostok {
namespace render {

stage_decals_accumulate::~stage_decals_accumulate( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62c490]
}

bool stage_decals_accumulate::is_effects_ready( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x62c470]
	return false;
}

void stage_decals_accumulate::debug_render( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x62ca90]
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
			// STATE[STUB]
			// FUNCTION BODY[0x62c450]
			return false;
		}
	};

	sort_by_priority_predicate predicate;
	(void)predicate;

	// STATE[STUB]
	// FUNCTION BODY[0x62c5d0]
}

} // namespace render
} // namespace vostok
