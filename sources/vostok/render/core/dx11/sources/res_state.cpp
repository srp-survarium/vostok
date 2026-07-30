#include "pch.h"
#include <vostok/render/core/res_state.h>

namespace vostok {
namespace render {

res_state::res_state(
	ID3D11RasterizerState*,
	ID3D11DepthStencilState*,
	ID3D11BlendState*,
	u32
)
{
	// STATE[STUB]
	// FUNCTION BODY[0x559760]
}

res_state::~res_state( )
{
	// STATE[STUB]
	// FUNCTION BODY[0x559750]
}

void res_state::destroy_impl( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x5597f0]
}

void res_state::apply( ) const
{
	// STATE[STUB]
	// FUNCTION BODY[0x559790]
}

} // namespace render
} // namespace vostok
