////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\dx11\sources\decl_utils.h"

namespace vostok {
namespace render {

// STATE[STUB]
DXGI_FORMAT decl_utils::ConvertVertexFormat( _D3DDECLTYPE dx9FMT )
{
	// FUNCTION BODY[0x6e7a10]: 9
	// <0>
	// <0x6e7a10>|0x000|+0x002:'44'
	// <0>
	// <0x6e7a12>|0x002|+0x012:'46'
	// <0x6e7a24>|0x014|-0x003:'47'
	// <0>
	// <1>
	// <2>
	// <0x6e7a21>|0x011|+0x002:'51'
	// <0x6e7a23>|0x013|+0x008:'52'
	// ******
}

// STATE[STUB]
pcstr decl_utils::ConvertSemantic( _D3DDECLUSAGE Semantic )
{
	return NULL;

	// FUNCTION BODY[0x6e79f0]: 9
	// <0>
	// <0x6e79f0>|0x000|+0x002:'81'
	// <0>
	// <0x6e79f2>|0x002|+0x012:'83'
	// <0x6e7a04>|0x014|-0x003:'84'
	// <0>
	// <1>
	// <2>
	// <0x6e7a01>|0x011|+0x002:'88'
	// <0x6e7a03>|0x013|+0x008:'89'
	// ******
}

// STATE[STUB]
void decl_utils::convert_vertex_declaration(
	vector< _D3DVERTEXELEMENT9 > const&		declIn,
	vector< D3D11_INPUT_ELEMENT_DESC >&		declOut
)
{
	// FUNCTION BODY[0x6e7a30]: 18
	// <0x6e7a77>|0x047|-0x044:'96'
	// <0x6e7a33>|0x003|+0x03e:'97'
	// <0>
	// <0x6e7a71>|0x041|+0x013:'99'
	// <0x6e7a84>|0x054|+0x002:'99'
	// <0>
	// <1>
	// <0x6e7a86>|0x056|+0x006:'102'
	// <0>
	// <0x6e7a8c>|0x05c|+0x06d:'104'
	// <0x6e7af9>|0x0c9|-0x046:'104'
	// <0x6e7ab3>|0x083|+0x007:'105'
	// <0x6e7aba>|0x08a|+0x048:'106'
	// <0x6e7b02>|0x0d2|-0x02e:'106'
	// <0x6e7ad4>|0x0a4|+0x006:'107'
	// <0x6e7ada>|0x0aa|+0x007:'108'
	// <0x6e7ae1>|0x0b1|+0x00b:'109'
	// <0x6e7aec>|0x0bc|+0x009:'110'
	// <0>
	// <1>
	// <2>
	// ******
}

	// TYPEDEFS
	// typedef
	// 	D3D11_INPUT_ELEMENT_DESC*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok
