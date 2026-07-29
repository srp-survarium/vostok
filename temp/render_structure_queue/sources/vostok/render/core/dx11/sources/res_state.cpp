////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\res_state.h"

namespace vostok {
namespace render {

// STATE[STUB]
 res_state::res_state(
	ID3D11RasterizerState*		rasterizer_state,
	ID3D11DepthStencilState*	depth_stencil_state,
	ID3D11BlendState*			blend_state,
	u32							stencil_ref
)
{
	// FUNCTION BODY[0x559760]: 1
	// <0>
	// ******
}

// STATE[STUB]
 res_state::~res_state( )
{
	// FUNCTION BODY[0x559750]: 1
	// <0>
	// ******
}

// STATE[STUB]
void res_state::destroy_impl( ) const
{
	// FUNCTION BODY[0x5597f0]: 0
	// ******
}

// STATE[STUB]
void res_state::apply( ) const
{
	// FUNCTION BODY[0x559790]: 4
	// <0x559790>|0x000|+0x018:'40'
	// <0x5597a8>|0x018|+0x012:'41'
	// <0x5597ba>|0x02a|+0x018:'42'
	// <0x5597d2>|0x042|+0x019:'43'
	// ******
}

	// TYPEDEFS
	// typedef
	// 	pbyte
	// 	iterator_type;

	// typedef
	// 	pcvoid*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_buffer_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
