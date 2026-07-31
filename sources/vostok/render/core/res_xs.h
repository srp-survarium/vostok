// Header guards are omitted intentionally.

#include <vostok/render/core/platform_helper.h>
#include VOSTOK_RENDER_CORE_FILE(res_xs.h)

namespace vostok {
namespace render {

template < typename shader_data >
inline s32 compare( res_xs< shader_data > const&, res_xs< shader_data > const& )
{
	// claude@NOTE: no legacy ancestor - no free two-object compare in the legacy generation (only the descriptor-keyed member equal); matcher-phase work.
	// STATE[STUB]
	// FUNCTION BODY[0x126d10] for vs_data
	return 0;
}

} // namespace render
} // namespace vostok
