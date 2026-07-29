////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "vostok\render\core\sources\event_query.h"

namespace vostok {
namespace render {

// STATE[STUB]
 event_query::event_query( )
{
	// CALL SITE INFO
	// <0x559925> -> HRESULT < unknown >( D3D11_QUERY_DESC const*, ID3D11Query** )
	// ******

	// FUNCTION BODY[0x559900]: 1
	// <0x559907>|0x007|+0x020:'17'
	// ******
}

// STATE[STUB]
 event_query::~event_query( )
{
	// CALL SITE INFO
	// <0x55985c> -> unsigned long < unknown >()
	// ******

	// FUNCTION BODY[0x559850]: 1
	// <0x559850>|0x000|+0x014:'22'
	// ******
}

// STATE[STUB]
void event_query::init_query( )
{
	// LOCALS
	// D3D11_QUERY_DESC 				query_desc
	// ******

	// CALL SITE INFO
	// <0x5598f3> -> HRESULT < unknown >( D3D11_QUERY_DESC const*, ID3D11Query** )
	// ******

	// FUNCTION BODY[0x5598d0]: 8
	// <0>
	// <1>
	// <2>
	// <0x5598d3>|0x003|+0x006:'30'
	// <0x5598d9>|0x009|+0x003:'31'
	// <0x5598dc>|0x00c|+0x019:'32'
	// <0>
	// <1>
	// ******
}

// STATE[STUB]
void event_query::release_query( )
{
	// CALL SITE INFO
	// <0x55983c> -> unsigned long < unknown >()
	// ******

	// FUNCTION BODY[0x559830]: 4
	// <0x559830>|0x000|+0x006:'39'
	// <0x559836>|0x006|+0x008:'40'
	// <0>
	// <0x55983e>|0x00e|+0x006:'42'
	// ******
}

// STATE[STUB]
void event_query::issue( )
{
	// CALL SITE INFO
	// <0x5598c4> -> void < unknown >( ID3D11Asynchronous* )
	// ******

	// FUNCTION BODY[0x5598b0]: 2
	// <0>
	// <0x5598b0>|0x000|+0x016:'48'
	// ******
}

// STATE[STUB]
void event_query::wait( )
{
	// LOCALS
	// u32 								is_rendering_finished
	// ******

	// FUNCTION BODY[0x559870]: 7
	// <0>
	// <1>
	// <0x559877>|0x007|+0x009:'55'
	// <0>
	// <1>
	// <2>
	// <0x559880>|0x010|+0x01f:'59'
	// ******
}

} // namespace render
} // namespace vostok
